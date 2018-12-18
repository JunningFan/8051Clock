#include "DS1302.h"
#include <assert.h>
//public variables
//UI layer
void initDS1302(void){
	P1 = 0;
	writeByte_1302 (WRITE_PROTECT,0X00);
	writeByte_1302 (WRITE_SECOND,0x00); 
	writeByte_1302 (WRITE_MINUTE,0x00); 
	writeByte_1302 (WRITE_HOUR,0x18); 
	writeByte_1302 (WRITE_DATE,0x04);
	writeByte_1302 (WRITE_MONTH,0x01); 
	writeByte_1302 (WRITE_WEEK,0x04); 
	writeByte_1302 (WRITE_YEAR,0x18); 
	writeByte_1302 (WRITE_PROTECT,0x80);
}
void setTime_1302(Time* time){		//read time from DS1302 and store the read data in Time pointer time

}
void readTime_1302(Time *time){		//write the data from mem addr specified by Time pointer time to DS1302 
	time->second = BCDtoBIN(readByte_1302(READ_SECOND), 7);
	time->minute = BCDtoBIN(readByte_1302(READ_MINUTE), 7);
	time->hour = BCDtoBIN(readByte_1302(READ_HOUR), 6);
	//time->hourMode = readByte_1302(READ_SECOND);
	time->date = BCDtoBIN(readByte_1302(READ_DATE), 6);
	time->month = BCDtoBIN(readByte_1302(READ_MONTH), 6);
	time->year = BCDtoBIN(readByte_1302(READ_YEAR), 8);
	time->weekday = readByte_1302(READ_WEEK);
}
//internal implementation
void writeByte_1302(uchar addr, uchar dat){	//write dat to specified addr
	RST_DS1302 = 0;
	_nop_();
	SCLK_DS1302 = 0;
	_nop_();
	RST_DS1302 = 1;
	_nop_();
	sendCom(addr);
	_nop_();
	sendCom(dat);
	_nop_();
	SCLK_DS1302=1;
	_nop_();
	RST_DS1302 = 0;			 	
}
uchar readByte_1302(uchar addr){	//read a byte from specified DS1302
	uchar tempUc;
	RST_DS1302 = 0;
	_nop_();
	RST_DS1302 = 1;
	_nop_();
   	SCLK_DS1302 = 0;
	_nop_();
	sendCom(addr);
	_nop_();
	tempUc = 0;
	//only if DS1302 is connected to P1
  	for(; tempUc < 8; tempUc++){
		ACC=ACC>>1;
		ACC_7=IO_DS1302;
		SCLK_DS1302=1;
		SCLK_DS1302=0;
	}
	addr = ACC; 
	SCLK_DS1302 = 1;
	_nop_();
	RST_DS1302 = 0;
	return ACC;	
}
void sendCom(uchar addr){
	uchar tempUc;
	tempUc = 0;
	ACC = addr;
	for(; tempUc < 8; tempUc++){
		IO_DS1302 = ACC_0;
		_nop_();
		SCLK_DS1302 = 1;
		_nop_();
		SCLK_DS1302 = 0;
		ACC >>= 1;
	}	
}
uchar BCDtoBIN(uchar bcd, uchar length){
	uchar bin;
	bin = bcd & 0x0F;
	bin += (((bcd << (8 - length)) >> (4 + (8 - length))) & 0x0F) * 10; 
	return bin;
}
uchar* BINtoASCII(uchar bin, uchar* output){   //output: uchar[2]
	output[0] = bin % 10 + 48;
	output[1] = bin / 10 + 48;
	return output;
}
void displayStr(uchar* str, uchar length){
	for(; length > 0; length--){
		SBUF = str[length - 1];
		while(!TI);
		TI = 0;
	}
}

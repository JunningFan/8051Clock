#include "DS1302.h"
sbit wela = P2^7 ;
sbit dula = P2^6 ;
sbit s1 = P3^4; //independent buttons
sbit s2 = P3^5;
sbit s3 = P3^6;
sbit s4 = P3^7;	//
uchar code charLib[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,0x6D, 0x7D, 
0x07, 0x7F, 0x6F, 0x80, 0x40};
void delay(uint z);
void baudRateGenerator(void);
void display(uchar* input, uchar dotMode);
uchar getButton(void);
int main(void){	
	Time time;
	uchar ASCIIStr[2];
	uchar toDisplay[6];
	uchar displayMode = 0;
	//EA = 1;
	//ES = 1; //interrupt enabled(Serial)
	//init buttons
	s1 = 1; 
	s2 = 1;
	s3 = 1;
	s4 = 1;	
	baudRateGenerator();
	SBUF = 0x0F;
	initDS1302();
	SBUF = 0x0F;
	TI = 0;	
   	while(1){
		readTime_1302(&time);
		//displayStr(BINtoASCII(time.second, ASCIIStr), 2);
		if(getButton() == 1)
			displayMode = !displayMode;
		if(displayMode){
			BINtoASCII(time.second, ASCIIStr);	
			toDisplay[5] = ASCIIStr[0] - 48;
			toDisplay[4] = ASCIIStr[1] - 48;
			BINtoASCII(time.minute, ASCIIStr);
			toDisplay[3] = ASCIIStr[0] - 48;
			toDisplay[2] = ASCIIStr[1] - 48;
			BINtoASCII(time.hour, ASCIIStr);
			toDisplay[1] = ASCIIStr[0] - 48;
			toDisplay[0] = ASCIIStr[1] - 48;
			display(toDisplay, 1);
		} else {
		   	BINtoASCII(time.date, ASCIIStr);	
			toDisplay[5] = ASCIIStr[0] - 48;
			toDisplay[4] = ASCIIStr[1] - 48;
			BINtoASCII(time.month, ASCIIStr);
			toDisplay[3] = ASCIIStr[0] - 48;
			toDisplay[2] = ASCIIStr[1] - 48;
			BINtoASCII(time.year, ASCIIStr);
			toDisplay[1] = ASCIIStr[0] - 48;
			toDisplay[0] = ASCIIStr[1] - 48;
			display(toDisplay, 1);
		}
	}
	return 0;
}
//config BaudRateGenerator
void baudRateGenerator(void){
	//baudRate: 2400
	TMOD = 0x20; //timer0: 8_bit auto reload
	TL1 = 0xF3;	//
	TH1 = 0xF3;
	TR1 = 1; //enable T0
	// set serial output to mode 1
	SM0 = 0;
	SM1 = 1;	// 9-bit var baudRate
	REN = 1;	// enable serial reciever
}
void delay(uint z){
	uint x,y;
	for(x=z;x>0;x--)
		for(y=1320;y>0;y--);	
}
void display(uchar* input, uchar dotMode){
	uchar i = 0; 	
	for(; i < 6; i++){
		wela = 1;
		P0 = 1 << i; //specify which LED to control
		P0 = ~P0;
		//j = 0;
		wela = 0;
		dula = 1;
		P0 = charLib[input[i]]; //decode ref from the wordStock
		if(dotMode == 1 && ((i == 1) || (i == 3 ))){
			delay(1);
			P0 =  charLib[10];
		} else if(dotMode == 2 && i == 2){
			delay(1);
			P0 =  charLib[10];
		}
		dula = 0;
		delay(1);
	}
}

uchar getButton(void){		 //detect which button is pressed; no button = 0; otherwise return 1~4
	uchar button = 0;
	if((!s1) || (!s2) || (!s3) || (!s4)){
	 	delay(10);
	}
	if(s1 == 0){
		button = 1;
	} else if(s2 == 0){
	 	button = 2;
	} else if(s3 ==0) {
	 	button = 3;
	}  else if(s4 == 0){
	 	button = 4;
	}
	if(button)
		delay(200);
	return button;
} 
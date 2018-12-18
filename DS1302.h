#include <reg52.h>
#include <intrins.h>
//Head File for driver for DS1302
#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
//specify physical addr of DS1302
sbit SCLK_DS1302 = P1^0;
sbit IO_DS1302 = P1^1;
sbit RST_DS1302 = P1^2; 
//define DS1302 registers' addrs
//дָ��궨���ַ
#define WRITE_SECOND 0x80//����
#define WRITE_MINUTE 0x82//����
#define WRITE_HOUR 0x84//Сʱ
#define WRITE_DATE 0x86//��
#define WRITE_MONTH 0x88//��
#define WRITE_WEEK 0x8a//����
#define WRITE_YEAR 0x8c//��
//��ָ��궨���ַ
#define READ_SECOND 0x81//����
#define READ_MINUTE 0x83//����
#define READ_HOUR 0x85//Сʱ
#define READ_DATE 0x87//��
#define READ_MONTH 0x89//��
#define READ_WEEK 0x8b//����
#define READ_YEAR 0x8d//��
//��д�����궨���ַ
#define WRITE_PROTECT 0x8e//д����
#define READ_PROTECT 0x8f//д����
//λѰַ�Ĵ�������
sbit ACC_7 = ACC^7;
sbit ACC_0 = ACC^0;
//costume data type
typedef struct time_struct {
	uchar second;
	uchar minute;
	uchar hour;
	uchar hourMode;
	uchar date;
	uchar month;
	uint year;
	uchar weekday;
} Time;	//all data stored as binary
//UI layer (competability safe)
void initDS1302(void);    //init the DS1302 for use
void setTime_1302(Time* time);	//read time from DS1302 and store the read data in Time pointer time
void readTime_1302(Time *time);	//write the data from mem addr specified by Time pointer time to DS1302 
//internal implementation(may change over updates)
void writeByte_1302(uchar addr, uchar dat);	//write dat to specified addr
uchar readByte_1302(uchar addr);	//read a byte from specified DS1302
uchar BCDtoBIN(uchar bcd, uchar length);
void sendCom(uchar addr);
uchar* BINtoASCII(uchar bin, uchar* output);
void displayStr(uchar* str, uchar length);
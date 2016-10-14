/************************************************************/
/* PROJECT NAME: Interface JHD162A LCD with LPC2148	        */
/* Device:       LPC2148                										*/
/* Filename:     lcd.c                                   		*/
/* Language:     C                      	                	*/
/* Compiler:     Keil ARM				                						*/
/* For more detail visit www.binaryupdates.com		        	*/
/************************************************************/
#include <LPC214x.h>

void LcdInit (void);
void DisplayRow (int row, char *str);
void Send_Command(unsigned char cmd);
void Send_Data(unsigned char dat);
void LcdCmd(unsigned char cmd);
void LcdDat(unsigned char dat);

void SmallDelay(void);
void Delay250(void);
void DelayMs(int n);

int main(void)
{
	LcdInit();	// Initialize LCD
	DisplayRow (1,"    Wel-Come  ");	// Display on 1st row	
	DisplayRow (2,"  BINARYUPDATES  ");	// Display on 2nd row

	while(1)
	{
	}
}

void LcdInit(void)
{
	IO1DIR = 0x03000000;		// Configure P1.24(EN) and P1.25(RS) as Output
	IO1CLR = 0x03000000;		// CLEAR(0) P1.24(EN) and P1.25(RS)

	IO0DIR = 0x40428000;		// Configure P0.15(D4), P0.17(D5), P0.22(D6) and P0.30(D7) as Output
	IO0CLR = 0x40428000;		// CLEAR(0) P0.15(D4), P0.17(D5), P0.22(D6) and P0.30(D7)

	DelayMs(6);
	Send_Command(0x03);
	
	DelayMs(6);
	Send_Command(0x03);
	Delay250();

	Send_Command(0x03);
	Delay250();

	Send_Command(0x02);
	Delay250();

	LcdCmd(0x28); //Function Set: 4-bit, 2 Line, 5x7 Dots 
	LcdCmd(0x08); //Display Off, cursor Off
	LcdCmd(0x0c); //Display On, cursor Off
	LcdCmd(0x06); //Entry Mode
}

/* send command on on data lines (D4 to D7) */
void Send_Command(unsigned char cmd)
{
	if (cmd & 0x01)  
		IO0SET = (1<<15);
	else
		IO0CLR = (1<<15);	

	if (cmd & 0x02)	
		IO0SET = (1<<17);
	else
		IO0CLR = (1<<17);
		
	if (cmd & 0x04) 
		IO0SET = (1<<22);
	else
		IO0CLR = (1<<22);	

	if (cmd & 0x08) 
		IO0SET = (1<<30);
	else
		IO0CLR = (1<<30);
		
	IO1CLR = 0x03000000;	// RS is LOW, for Instruction Register and EN is LOW 
	SmallDelay();
	IO1SET = 0x01000000;	// SET(1) EN 
	SmallDelay();
	IO1CLR = 0x01000000;	// CLEAR(0) EN 
	SmallDelay();
}

/* send data on on data lines (D4 to D7) */
void Send_Data(unsigned char dat)
{
	if (dat & 0x01)
		IO0SET = (1<<15);
	else
		IO0CLR = (1<<15);
		
	if (dat & 0x02)
		IO0SET = (1<<17);
	else
		IO0CLR = (1<<17);
		
	if (dat & 0x04)
		IO0SET = (1<<22);
	else
		IO0CLR = (1<<22);

	if (dat & 0x08)
		IO0SET = (1<<30);
	else
		IO0CLR = (1<<30);
		
	IO1SET = 0x02000000;	// SET(1) RS is set HIGH for Data Register 
	SmallDelay() ;	
	IO1CLR = 0x01000000;	// CLEAR(0) EN 
	SmallDelay() ;		
	IO1SET = 0x01000000;	// SET(1) EN 
	SmallDelay() ;
	IO1CLR = 0x01000000;	// CLEAR(0) EN 
	SmallDelay() ;	
}

void LcdCmd(unsigned char cmd)
{
	Send_Command(cmd >> 4);
	Send_Command(cmd);
	Delay250();
	Delay250();
}
void LcdDat(unsigned char dat)
{
	Send_Data(dat >> 4);
	Send_Data(dat);
	Delay250();
	Delay250();
}

void DisplayRow (int row, char *str)
{
/*
	pass pointer to 16 character string
	displayes the message on line1 or line2 of LCD, depending on whether row is 1 or 2.
*/
	int k ;

	if (row == 1)
		LcdCmd(0x80) ;  // Force cursor to beginning of 1st line
	else
		LcdCmd(0xc0) ;  // Force cursor to beginning of 2nd line
	for(k = 0 ; k < 16 ; k ++)
	{
		if (str[k])
			LcdDat(str[k]) ;
		else
			break ;
	}
	while(k < 16)
	{
		LcdDat(' ') ;
		k ++ ;
	}
}

void SmallDelay(void)
{
	int i;
	for(i=0;i<100;i++);	
}

void Delay250(void)
{
	int k,j ;
	j =200 ;
	for(k = 0 ; k < 100 ; k ++)
	{
		j-- ;
	}
}
void DelayMs(int n)
{
	int k ;
	for(k = 0 ; k < n ; k ++)
	{
		Delay250() ;
		Delay250() ;
	}
}

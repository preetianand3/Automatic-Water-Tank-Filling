#include <lpc17xx.h>
#define IR_SENSOR_PIN  (1 << 26)  // P0.26 for IR sensor
#define MOTOR        (1 << 25)  // P0.25 for LED
#define BUZZER       (1 << 24)  // P0.24 for BUZZER
#define TOUCH       (1 << 23)  // P0.23 for TOUCH

void delay(unsigned int time);


#include "lcd.h"

unsigned long int temp1=0, temp2=0 ;
unsigned char Msg1[14] = {"MOTOR ON"};
unsigned char Msg2[15] = {"WATER FLOW"};
unsigned char Msg3[14] = {"MOTOR OFF"};
unsigned char Msg4[20] = {"NO WATER FLOW"};
unsigned char Msg5[20] = {"SYSTEM OFF"};
unsigned char Msg0[20] = {"SYSTEM ON"};
int main(void)
{
	SystemInit();
	SystemCoreClockUpdate();
   
	 // Initialize IR sensor (P0.26 as input)
    LPC_PINCON->PINSEL1 &= ~(0x3 << 20);    // Configure P0.26 as GPIO
    LPC_GPIO0->FIODIR &= ~IR_SENSOR_PIN;    // Set P0.26 as input

    // Initialize LED (P0.25 as output)
    LPC_PINCON->PINSEL1 &= ~(0x3 << 18);    // Configure P0.25 as GPIO
    LPC_GPIO0->FIODIR |= MOTOR;           // Set P0.25 as output
	
    LPC_PINCON->PINSEL1 &= ~(0x3 << 16);    // Configure P0.24 as GPIO
    LPC_GPIO0->FIODIR |= BUZZER;           // Set P0.24 as output
	
	  LPC_PINCON->PINSEL1 &= ~(0x3 << 14);    // Configure P0.23 as GPIO
    LPC_GPIO0->FIODIR &= ~TOUCH;           // Set P0.23 as input
	
	    while (1)
    {
		if((LPC_GPIO0->FIOPIN & TOUCH) != 0){
			//delay(900000);
			clr_disp();
        // Check if IR sensor is triggered (active low)
        if ((LPC_GPIO0->FIOPIN & IR_SENSOR_PIN) != 0) // If sensor is not active
        {
            LPC_GPIO0->FIOCLR = MOTOR;  // Turn on LED
            LPC_GPIO0->FIOCLR = BUZZER;  // Turn on LED
					
			 	    delay(10000);
					
					  LPC_GPIO0->FIOSET = BUZZER;  // Turn on LED
			 	    delay(100000);
            LPC_GPIO0->FIOCLR = BUZZER;  // Turn on LED
					
			 	    delay(90000);
					
					  LPC_GPIO0->FIOSET = BUZZER;  // Turn on LED
			 	    delay(100000);
            LPC_GPIO0->FIOCLR = BUZZER;  // Turn on LED
					
			 	    delay(10000);
					
					  LPC_GPIO0->FIOSET = BUZZER;  // Turn on LED
			 	    delay(100000);					
				clr_disp();
				lcd_init();				//initialise LCD
				delay_lcd(3200);

				temp1 = 0x80;			//1st message on LCD 1st line
				lcd_com();
				delay_lcd(800);
				lcd_puts(Msg1);

				temp1 = 0xC0;			//Msg2 on LCD 2nd line
				lcd_com();
				delay_lcd(800);
				lcd_puts(Msg2);
				delay_lcd(5000);
				delay(100000);
				}else if((LPC_GPIO0->FIOPIN & IR_SENSOR_PIN) == 0){
          LPC_GPIO0->FIOSET = MOTOR;  // Turn off LED
					LPC_GPIO0->FIOSET = BUZZER;
					clr_disp();
					lcd_init();				//initialise LCD
					delay_lcd(3200);

					temp1 = 0x80;			//1st message on LCD 1st line
					lcd_com();
					delay_lcd(800);
					lcd_puts(Msg3);

					temp1 = 0xC0;			//Msg2 on LCD 2nd line
					lcd_com();
					delay_lcd(800);
					lcd_puts(Msg4);
					delay_lcd(5000);
					
					delay(1000000);
        }
}
		else{
			    LPC_GPIO0->FIOSET = MOTOR;  // Turn on LED
          LPC_GPIO0->FIOSET = BUZZER; 
					clr_disp();
					lcd_init();				//initialise LCD
					delay_lcd(3200);

					temp1 = 0x80;			//1st message on LCD 1st line
					lcd_com();
					delay_lcd(800);
					lcd_puts(Msg5);
					
					delay(1000000);
					clr_disp();}

				}}

//lcd initialization
void lcd_init()
{
	/* Ports initialized as GPIO */
    LPC_PINCON->PINSEL3 &= 0xFFFF00FF;  //P1.20 to P1.23
	LPC_PINCON->PINSEL7 &= 0XFFF3FFFF;  //P3.25
    LPC_PINCON->PINSEL7 &= 0xFFCFFFFF;  //P3.26
	LPC_PINCON->PINSEL9 &= 0xFCFFFFFF;  //P4.28

	/* Setting the directions as output */
    LPC_GPIO1->FIODIR |= DT_CTRL;	// data lines - P1.20 to P1.23
	LPC_GPIO3->FIODIR |= RS_CTRL;	// RS - P3.25
    LPC_GPIO3->FIODIR |= RW_CTRL;	// RW - P3.26
	LPC_GPIO4->FIODIR |= EN_CTRL;	// P4.28 
        
    clear_ports();
	delay_lcd(3200);

	temp2=0x30;		   
	wr_cn();	   
	delay_lcd(30000); 
		
	temp2=0x30;
	wr_cn();
	delay_lcd(30000);	 
		
	temp2=0x30;
	wr_cn();
	delay_lcd(30000);

	temp2=0x20;
	wr_cn();
	delay_lcd(30000);

	temp1 = 0x28;
	lcd_com();
	delay_lcd(30000);
		
	temp1 = 0x0c;		
	lcd_com();
	delay_lcd(800);
	
	temp1 = 0x06;
	lcd_com();
	delay_lcd(800);
	
	temp1 = 0x01;
	lcd_com();
 	delay_lcd(10000);
	
	temp1 = 0x80;
	lcd_com();
	delay_lcd(800);
    return;
}

void lcd_com(void)
{
	temp2= temp1 & 0xf0;
	temp2 = temp2 << 16;				//data lines from 20 to 23
	wr_cn();
	temp2 = temp1 & 0x0f;
	temp2 = temp2 << 20; 
	wr_cn();
	delay_lcd(1000);
    return;
}

// command nibble o/p routine
void wr_cn(void)                        //write command reg
{ 	 
	clear_ports();
	LPC_GPIO1->FIOPIN = temp2;		// Assign the value to the data lines    
	LPC_GPIO3->FIOCLR = RW_CTRL;		// clear bit RW
    LPC_GPIO3->FIOCLR = RS_CTRL;		// clear bit RW
	LPC_GPIO4->FIOSET = EN_CTRL;   	// EN=1
	delay_lcd(25);
	LPC_GPIO4->FIOCLR  = EN_CTRL;		 		// EN =0
    return;
    
 }

// data o/p routine which also outputs high nibble first
// and lower nibble next
 void lcd_data(void)
{             
    temp2 = temp1 & 0xf0;
    temp2 = temp2 << 16;
    wr_dn();
    temp2= temp1 & 0x0f;	
    temp2= temp2 << 20;
    wr_dn();
    delay_lcd(1000);	
    return;
} 

// data nibble o/p routine
void wr_dn(void)
{  	  
	clear_ports();

	LPC_GPIO1->FIOPIN = temp2;			// Assign the value to the data lines    
	LPC_GPIO3->FIOSET = RS_CTRL;		// set bit  RS
	LPC_GPIO3->FIOCLR = RW_CTRL;		// clear bit  RW
	LPC_GPIO4->FIOSET = EN_CTRL;   	// EN=1
	delay_lcd(25);
	LPC_GPIO4->FIOCLR  = EN_CTRL;	// EN =0
    return;
 }

void delay_lcd(unsigned int r1)
{
  	unsigned int r;
  	for(r=0;r<r1;r++);
    return;
}

void delay(unsigned int r1)
{
  	unsigned int r;
  	for(r=0;r<r1;r++);
    return;
}

void clr_disp(void)
{
	temp1 = 0x01;
	lcd_com();
 	delay_lcd(10000);
    return;
}
void clear_ports(void)
{
    /* Clearing the lines at power on */
	LPC_GPIO1->FIOCLR = DT_CTRL; //Clearing data lines
	LPC_GPIO3->FIOCLR = RS_CTRL;  //Clearing RS line
    LPC_GPIO3->FIOCLR = RW_CTRL;  //Clearing RW line
	LPC_GPIO4->FIOCLR = EN_CTRL; //Clearing Enable line
        
    return;
}

void lcd_puts(unsigned char *buf1)
{
    unsigned int i=0;

    while(buf1[i]!='\0')
    {
        temp1 = buf1[i];
     	lcd_data();
		i++;
        if(i==16)
		{
           	temp1 = 0xc0;
			lcd_com();
		}
         
       }
    return;
}

///final
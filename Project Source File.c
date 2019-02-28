/*
 * File:   Final Project.c
 * Author: Serkan Kavak
 *
 * Created on 13 November 2016 Sunday, 23:28
 */


/** INCLUDES **********************/ 
#include <xc.h> 
 
/** CONFIGURATION *****************/ 
#pragma config FOSC = INTIO67       //Internal oscillator 
#pragma config WDTEN = OFF          //Watch dog timer disabled 
#pragma config LVP = OFF            //LVP Off 

/** 7-segment display truth table *******************/ 
const unsigned char table_7_segm[] = 
{ 
 0x3F,  //0 
 0x06,  //1 
 0x5B,  //2 
 0x4F,  //3 
 0x66,  //4 
 0x6D,  //5 
 0x7D,  //6 
 0x07,  //7 
 0x7F,  //8 
 0x6F  //9 
}; 
 
/** F U N C T I O N S *******************************/ 
void delay (unsigned int ms) 
{ 
 unsigned int i; 
 while (ms--) { 
  for (i = 0; i < 398; i++) 
  ; 
 } 
} 
 
unsigned int Convert_Voltage (char channel) 
{ 
 
 switch (channel) 
 { 
 case 0: 
   ADCON0 = 0b00000001; //AN0 
 break;
 } 
 ADCON0bits.GO = 1;  //Start conversion 
 while (ADCON0bits.GO == 1); //Wait for end 
 return (((unsigned int)ADRESH << 8) | (unsigned int)ADRESL); 
}

/** M A I N *****************************************/ 
void main(void) { 
   float number = 0;    //float number to display 
   char indicator = 0;    //variable to index 7-segment display 
   char i = 0;
 
    OSCCONbits.IRCF2 = 1;    //internal RC 16 MHz 
    OSCCONbits.IRCF1 = 1;  
    OSCCONbits.IRCF0 = 1; 
 
    ANSELA = 0b00000001;        //RA1/AN1 ? analog input 
    ANSELB = 0; 
    ANSELC = 0; 
 
    LATA = 0; 
    TRISA = 0b00000001;         //RA1 - input 
    LATB = 0; 
    TRISB = 0;                  //PORTB - outputs 
    LATC = 0; 
    TRISC = 0;                  //PORTC - outputs 
 
    //ADC nustatymai 
    ADCON1 = 0b00000000;        //Vref+ = Vdd, Vref- = Vss 
    ADCON2 = 0b10101101;        //right justify, Tad=Fosc/16, Tacq=12Tad 
    ADCON0 = 0b00000101;        //AN1 channel, ADC enabled 
 
    while (1) 
        { 
            //Call function to make conversion. 
            //Function returns result after conversion to variable number 
            number = Convert_Voltage (0);
             
            //Display 
            number = number +0.5;               //rounding! 
            number = number * 0.0488;
            indicator = 0x80;
 
            for (i=0; i<3; i++) { 
                LATC = table_7_segm[(int) number]; 
                number = number - (int) number; 
                number = number * 10; 
                LATB = indicator;               //indicator ON 
                
                    if (i == 1)
                    LATCbits.LATC7 = 1;
                
                delay (3);                      //delay 3 ms 
                LATB = 0;          //indicator OFF 
                indicator = indicator >> 1;     //shift to next indicator 
                
            } //end for()
            delay(30);
 
 } //end while() 
}
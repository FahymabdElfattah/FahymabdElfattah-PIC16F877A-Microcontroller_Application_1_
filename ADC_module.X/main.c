#include <xc.h>
#include <pic16f877a.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#define _XTAL_FREQ 20000000
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

#define RS PORTBbits.RB0
#define RW PORTBbits.RB1
#define EN  PORTBbits.RB2

void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_init();
void lcd_caracter(const unsigned char *str);
void lcd_number(int nb);

int adc0,adc1,adc2,adc3,adc4,adc5,adc6,adc7 ;


int R1,L1,in1,R2,L2,in2;

void interrupt adc_conv();
void int_Port();
void int_ADC();
int ReadData_from(const unsigned char *SLC);




void main(void) {
    int temp0,temp1,temp2,temp3,temp4 ;
    int_Port();
    int_ADC();
    lcd_init();
    while (1){
        temp0 = ReadData_from("AN6");
        temp0 = (int)(temp0/2.046);
        
        temp1 = ReadData_from("AN7");
        temp1 = (int)(temp1/2.046);
        lcd_cmd(0x80);
        lcd_caracter("T0 = ");
        lcd_number(temp0);
        lcd_caracter(" T1 = ");
        lcd_number(temp1);
        
        temp2 = ReadData_from("AN5");
        temp2 = (int)(temp2/2.046);
        lcd_cmd(0xC0);
        lcd_caracter("T2 = ");
        lcd_number(temp2);     
    }
    return;
}
void int_Port(){
    TRISB = 0x00;
    TRISA = 0xFF;
    TRISE =0xFF;
}
void int_ADC(){
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.ADIE = 1 ;
    ADCON0 = 0x41 ; // b'01 000 0 01'
    ADCON1 = 0x80 ; // b'1000 0000'
}
int ReadData_from(const unsigned char *SLC){
    int analog ;
    if(SLC == "AN0"){
        ADCON0 = 0x45; //b'01 000 1 01'
        analog = adc0;
    }else if (SLC == "AN1") {
        ADCON0 = 0x4D; //b'01 001 1 01'
        analog = adc1;
    }else if (SLC == "AN2") {
        ADCON0 = 0x55; //b'01 010 1 01'
        analog = adc2;
    }else if (SLC == "AN3") {
        ADCON0 = 0x5D; //b'01 011 1 01'
        analog = adc3;
    }else if (SLC == "AN4") {
        ADCON0 = 0x65; //b'01 100 1 01'
        analog = adc4;
    }else if (SLC == "AN5") {
        ADCON0 = 0x6D; //b'01 101 1 01'
        analog = adc5;
    }else if (SLC == "AN6") {
        ADCON0 = 0x75; //b'01 110 1 01'
        analog = adc6;
    }else if (SLC == "AN7") {
        ADCON0 = 0x7D; //b'01 111 1 01'
        analog = adc7;
    } 
   return analog ; 
}


void lcd_cmd(unsigned char cmd){
    // 0x80 0xC0 0x94 0xD4
    PORTB = (cmd & 0xF0);
    EN = 1;
    RW = 0;
    RS = 0;
    __delay_ms(2);
    EN = 0 ;
    PORTB = ((cmd<<4)& 0xF0);
    EN = 1;
    RW = 0;
    RS = 0;
    __delay_ms(2);
    EN = 0;
}
void lcd_data(unsigned char data){
    PORTB = (data & 0xF0);
    EN = 1;
    RW = 0;
    RS = 1;
    __delay_ms(2);
    EN = 0 ;
    PORTB = ((data<<4)& 0xF0);
    EN = 1;
    RW = 0;
    RS = 1;
    __delay_ms(2);
    EN = 0;
}
void lcd_init(){
    lcd_cmd(0x02);
    lcd_cmd(0x28);
    lcd_cmd(0x0C);
    lcd_cmd(0x04);
    lcd_cmd(0x01);
}
void lcd_caracter(const unsigned char *str){
    unsigned char i ,num = strlen(str) ;
    for(i=0;i<num;i++){
        lcd_data(str[i]);
    }
}
void lcd_number(int nb){
    unsigned char strNb[10];
    sprintf(strNb, "%d", nb) ;
    lcd_caracter(strNb);
}


void interrupt adc_conv(){
    if(PIR1bits.ADIF = 1 && (ADCON0bits.CHS0 == 0 && ADCON0bits.CHS1 == 0 && ADCON0bits.CHS2 == 0)){
        adc0 = (ADRESH <<8);
        adc0 = adc0 + ADRESL ;
        PIR1bits.ADIF = 0;
    }
    if(PIR1bits.ADIF = 1 && (ADCON0bits.CHS0 ==1 && ADCON0bits.CHS1 == 0 && ADCON0bits.CHS2 == 0)){
        adc1 = (ADRESH <<8);
        adc1 = adc1 + ADRESL ;
        PIR1bits.ADIF = 0;
    }
    if(PIR1bits.ADIF = 1 && (ADCON0bits.CHS0 ==0 && ADCON0bits.CHS1 == 1 && ADCON0bits.CHS2 == 0)){
        adc2 = (ADRESH <<8);
        adc2 = adc2 + ADRESL ;
        PIR1bits.ADIF = 0;
    }
    if(PIR1bits.ADIF = 1 && (ADCON0bits.CHS0 ==1 && ADCON0bits.CHS1 == 1 && ADCON0bits.CHS2 == 0)){
        adc3 = (ADRESH <<8);
        adc3 = adc3 + ADRESL ;
        PIR1bits.ADIF = 0;
    }
    if(PIR1bits.ADIF = 1 && (ADCON0bits.CHS0 ==0 && ADCON0bits.CHS1 == 0 && ADCON0bits.CHS2 == 1)){
        adc4 = (ADRESH <<8);
        adc4 = adc4 + ADRESL ;
        PIR1bits.ADIF = 0;
    }
    if(PIR1bits.ADIF = 1 && (ADCON0bits.CHS0 ==1 && ADCON0bits.CHS1 == 0 && ADCON0bits.CHS2 == 1)){
        adc5 = (ADRESH <<8);
        adc5 = adc5 + ADRESL ;
        PIR1bits.ADIF = 0;
    }
    if(PIR1bits.ADIF = 1 && (ADCON0bits.CHS0 ==0 && ADCON0bits.CHS1 == 1 && ADCON0bits.CHS2 == 1)){
        adc6 = (ADRESH <<8);
        adc6 = adc6 + ADRESL ;
        PIR1bits.ADIF = 0;
    }
    if(PIR1bits.ADIF = 1 && (ADCON0bits.CHS0 ==1 && ADCON0bits.CHS1 == 1 && ADCON0bits.CHS2 == 1)){
        adc7 = (ADRESH <<8);
        adc7 = adc7 + ADRESL ;
        PIR1bits.ADIF = 0;
    }  
}


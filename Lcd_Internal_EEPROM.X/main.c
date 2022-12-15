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

unsigned char byte ;

void int_Port();
void ReadByteFromEEPROM(const unsigned adress);
void WriteByteToEEPROM(unsigned char data ,const unsigned adress);

void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_init();
void lcd_caracter(const unsigned char *str);
void lcd_number(int nb);




void main(void) {
    int_Port();
    lcd_init();
    lcd_caracter(" Fahym");
    WriteByteToEEPROM(7,0xAA);
    ReadByteFromEEPROM(0xAA);
    __delay_ms(1000);
    while (1){
        __delay_ms(1000);
        WriteByteToEEPROM(7,0xAA);
        ReadByteFromEEPROM(0xAA);
        
        lcd_cmd(0x80);
        lcd_caracter("data = ");
        lcd_number(byte);
        __delay_ms(1000);
        WriteByteToEEPROM(12,0xAA);
        ReadByteFromEEPROM(0xAA);
        lcd_cmd(0x80);
        lcd_caracter("data = ");
        lcd_number(byte);
        }
    
    return;
}
void int_Port(){
    TRISB = 0x00;
    TRISC = 0x78; //b'0111 1000'
    
    
};

void ReadByteFromEEPROM(const unsigned adress){
    EEADR = adress ;
    EECON1bits.EEPGD = 0;
    EECON1bits.RD = 1 ;
    byte = EEDATA ;
}
void WriteByteToEEPROM(unsigned char data ,const unsigned adress){
    EEADR = adress ;
    EEDATA = data ;
    EECON1bits.EEPGD = 0;
    EECON1bits.WREN = 1 ;
    EECON2 = 0x55 ;
    EECON2 = 0xAA ;
    EECON1bits.WR = 1;
    while(PIR2bits.EEIF == 0) ;
        PIR2bits.EEIF == 0 ;
    
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

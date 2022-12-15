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

#define C1 PORTCbits.RC0
#define C2 PORTCbits.RC1
#define C3 PORTCbits.RC2
#define R1 PORTCbits.RC3
#define R2 PORTCbits.RC4
#define R3 PORTCbits.RC5
#define R4 PORTCbits.RC6

void int_Port();
void lcd_cmd(unsigned char cmd);
void lcd_data(unsigned char data);
void lcd_init();
void lcd_caracter(const unsigned char *str);
void lcd_number(int nb);
void keybad_4x3();

unsigned char i,j;
unsigned char column[3]={0},c;
unsigned char row[4]={0},r;
unsigned char Keypad[3][4]={{1,2,3},{4,5,6},{7,8,9},{"*",0,"#"}};



void main(void) {
    int_Port();
    lcd_init();
    lcd_caracter("T= ");
    while (1){
        keybad_4x3();
        lcd_cmd(0x80);  //1100 0000
        lcd_caracter("keypad= ");
        lcd_number(Keypad[i][j]);
        if(i == 3 && j== 2){
            lcd_caracter("#");
        }
        if(i == 3 && j == 0){
            lcd_caracter("*");
        }
        
        __delay_ms(200);
        lcd_cmd(0x01); // 0000 0001
        
        
        
    }
    
    return;
}
void int_Port(){
    TRISB = 0x00;
    TRISC = 0x78; //b'0111 1000'
    C1 =C2 =C3 =0;
    
};
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
void keybad_4x3(){
    for(c=0;c<3;c++){
        column[0] = 0;
        column[1] = 0;
        column[2] = 0;
        column[c] = 1;
        C1 = column[0];
        C2 = column[1];
        C3 = column[2];
        row[0] = R1;
        row[1] = R2;
        row[2] = R3;
        row[3] = R4;
        for(r=0;r<4;r++){
            if(row[r] == 1){
              i=r;
              j=c;
            }
        }
    }
}

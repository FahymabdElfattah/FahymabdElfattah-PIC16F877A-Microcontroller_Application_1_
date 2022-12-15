#include <xc.h>
#include <pic16f877a.h>

#define _XTAL_FREQ 20000000
#pragma config FOSC = EXTRC     // Oscillator Selection bits (RC oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON       // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = ON         // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)


unsigned char byte ;

void int_Port();
void ReadByteFromEEPROM(const unsigned adress);
void WriteByteToEEPROM(unsigned char data ,const unsigned adress);
void main(void) {
    int_Port();
    
    while (1){
        }
    return;
}
void int_Port(){
    //TRISB0 = 0;
}

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

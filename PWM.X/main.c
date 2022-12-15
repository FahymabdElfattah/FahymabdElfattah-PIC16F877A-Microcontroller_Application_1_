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



unsigned char timer_count = 0;

void int_Port();
void int_PWM();
void setPWMDutyCycle1(unsigned int DutyCycle);


void main(void) {
    
    int_Port();
    int_PWM();
    
    setPWMDutyCycle1(512);
    __delay_ms(1000);
    CCP1CON = 0x00 ;
    PORTCbits.RC2 = 0;
    while (1){
        CCP1CON = 0x0C ;
        setPWMDutyCycle1(512);
        __delay_ms(1000);
        CCP1CON = 0x00 ;
        PORTCbits.RC2 = 0;
        __delay_ms(1000);
        CCP1CON = 0x0C ;
        setPWMDutyCycle1(512);
        __delay_ms(1000);
        CCP1CON = 0x00 ;
        PORTCbits.RC2 = 0;
        __delay_ms(1000);
        }
    
    return;
}
void int_Port(){
    TRISC = 0x00;
}
void int_PWM(){
    /*                            1       1      x        x
     * ---  ---  CCPxX  CCPxY  CCPxM3  CCPxM2  CCPxM1  CCPxM0
     */
    CCP1CON = 0x0C ;
    PR2 = 0xFF ;
    /*  0     0        0        0        0         1       0        0
     * ---  TOUTPS3  TOUTPS2  TOUTPS1  TOUTPS0  TMR2ON  T2CKPS1  T2CKPS0 */
    T2CON = 0x04 ; 
}
void setPWMDutyCycle1(unsigned int DutyCycle){
    CCPR1L = (DutyCycle >> 2);
    CCP1CON = CCP1CON & 0xCF ;
    CCP1CON = CCP1CON | (0x30&(DutyCycle <<4)) ;
}
/*
81/PWM frequency = (PR2 + 1) x {Machine cycle x (TMR2 Prescalar Value)}
Machine cycle = 200 ns

 * PIR2 [0 , 255]
 
 
 
DUTY CYCLE VALUE IS A 10 BIT VALUE 
The maximum value is 1023
The minimum value is 0

100 %         1023
75   %         768
50   %         512
25   %         256
 */
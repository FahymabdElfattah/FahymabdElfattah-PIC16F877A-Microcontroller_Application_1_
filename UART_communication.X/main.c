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

unsigned char rx_data;

void int_Port();
void int_Uart();
void TX_uart(const unsigned char *data);
void interrupt RX_uart();

void main(void) {
    int_Port();
    int_Uart();
    while (1){
       
        __delay_ms(1000);
        __delay_ms(1000);
        __delay_ms(1000);
       TX_uart(rx_data);
       __delay_ms(1000); 
       }
    
    return;
}
void int_Port(){
    TRISB0 = 0;
}
void int_Uart(){
    //RC7/RX/DT  RC6/TX/CK
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    PIE1bits.RCIE = 1;
    TXSTA = 0x24;
    RCSTA = 0x90;
    SPBRG = 129; // baud rate =1900 Fosc =20 Mhz
    
}
void TX_uart(const unsigned char *data){
    unsigned int i,num = strlen(data);
    for (i = 0; i <  num; i++) {
        if (PIR1bits.TXIF == 1) {
        TXREG = data[i] ;
        PIR1bits.TXIF = 0 ;
        }
        __delay_ms(10);
    }
}
void interrupt RX_uart(){
    if (PIR1bits.RCIF == 1) {
        rx_data = RCREG ;
        PIR1bits.RCIF = 0 ;
    }

}

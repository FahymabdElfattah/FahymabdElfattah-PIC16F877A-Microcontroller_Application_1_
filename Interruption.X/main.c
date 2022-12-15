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





void int_Port();
void int_Interruption();
void interrupt fct_Interruption(){
    if(INTCONbits.INTF == 1){
        PORTCbits.RC0 = !(PORTCbits.RC0);
        INTCONbits.INTF = 0 ;
    }
    
}

void main(void) {
    int_Port();
    int_Interruption();
    while (1){
       
    }
    
    return;
}
void int_Port(){
    TRISC0 = 0;
};
void int_Interruption(){
    //-----RB0----------
    INTCONbits.GIE = 1;
    INTCONbits.PEIE = 1;
    INTCONbits.INTE = 1;
    OPTION_REGbits.INTEDG =1;
}


/*
TRISB0 = 1; 
TRISB = 0x00;
 
PORTC = 0x1 ;
PORTBbits.RB1 = 0;
 
if (PORTBbits.RB0 == 1) {
            PORTBbits.RB1 = 1;
    }
  
*/
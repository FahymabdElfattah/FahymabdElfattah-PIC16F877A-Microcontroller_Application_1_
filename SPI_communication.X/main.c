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

unsigned char spi_data ;


void int_Port();
void int_spi();
void spi_write(unsigned int value);
void spi_send(unsigned char data);
unsigned char spi_read();


void main(void) {
    int_Port();
    int_spi();
    
    spi_send(0x05);
    __delay_ms(1000);
    spi_send(0x06);
    spi_data = spi_read();
    __delay_ms(1000);
    spi_send(spi_data);
    while (1){
        
    }
    return;
}
void int_Port(){
    TRISBbits.TRISB0 = 0 ; //SLAVE SELECT PIN
    TRISCbits.TRISC3 = 0 ; //SERIAL CLOCK
    TRISCbits.TRISC4 = 1 ; //MISO
    TRISCbits.TRISC5 = 0 ; //MOSI
}
void int_spi(){
    SSPCON = 0x20;
    SSPSTAT = 0x00;
}
void spi_write(unsigned int value){
    SSPBUF = value ;
    while(!SSPSTATbits.BF);
}

// Recieve data by spi communication
unsigned char spi_read(){
    SSPBUF = 0x00 ; //initiate bus cycle
    while(!PIR1bits.SSPIF); // wait until cycle complete
    return SSPBUF ; //return byte read
}
// Send Data by spi communication
void spi_send(unsigned char data){
    PORTBbits.RB0 = 0 ;
    spi_write(data);
    PORTBbits.RB0 = 1 ;
}



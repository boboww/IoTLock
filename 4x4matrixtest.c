#include <xc.h>
//#include <stdio.h>
//#include <stdlib.h>
#include <string.h>
//#include <pic18f45k50.h>

#define _XTAL_FREQ 1000000

#pragma config FOSC = INTOSCIO// Oscillator Selection bits (Internal oscillator, port
//function on RA6, EC used by USB (INTIO))
#pragma CONFIG DEBUG = ON;// Enable Debug Mode
#pragma config WDTEN = OFF // Watchdog Timer Enable bit (WDT disabled (control is
//placed on the SWDTEN bit))
#pragma config PBADEN = OFF // PORTB A/D Enable bit (PORTB&lt;4:0&gt; pins are
// configured as digital I/O on Reset)
#pragma config MCLRE = ON // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin
//disabled)
#pragma config LVP = ON // Single-Supply ICSP Enable bit (Single-Supply ICSP
//LCD Control pins

#define rs PORTCbits.RC0        //LCD Control pins
#define rw PORTCbits.RC1
#define en PORTCbits.RC2

#define lcdport PORTD           //LCD Data pins

void lcd_ini();

void lcdcmd(unsigned char);
void lcddata(unsigned char);
void writeString(unsigned char sendData);

unsigned char readKeyboard(int i);
unsigned char findKey(unsigned short a, unsigned short b);

unsigned char data[30]="Test";
unsigned int i=0;

void main(void) {
    TRISA   = 0xF0;    // Configure Port A as output port
    LATA    = 0x00;
    PORTA   = 0x00;
    TRISB   = 0x00;    // configure Port B as input port
    LATB    = 0x00;
    PORTB   = 0x00;
    TRISC   = 0x00;    // Configure Port C as output port
    LATC    = 0x00;
    PORTC   = 0x00; 
    TRISD   = 0x00;    // Configure Port D as output port
    LATD    = 0x00;
    PORTD   = 0x00;
    TRISE   = 0x00;    // Configure Port E as output port
    LATE    = 0x00;
    PORTE   = 0x00;
        
    lcd_ini(); // LCD initialization //    
    unsigned char r;
    int i = 0;
    
    while(1){    
        for(i = 0; i <= 4; i++){
            if(i == 4){
                i = 0;
                break;
            }
            r = readKeyboard(i);
            if(r != ' ')
                writeString(r);
        //__delay_ms(500);
        //lcdcmd(0x01); // Clear display screen
        }
    }
    return;
}

// This function writes a string to LCD 1 byte at time
// Pass variable data to this function)
void writeString(unsigned char sendData){
    unsigned int i = 0;
    //while(sendData[i]!='\0'){
        lcddata(sendData);//[i]); // Call lcddata function to send characters
        // one by one from "data" array
        //i++;
        __delay_ms(20);
    //}
}

void lcd_ini(){
    lcdcmd(0x38); // Configure the LCD in 8-bit mode, 2 line and 5x7 font
    lcdcmd(0x0C); // Display On and Cursor Off
    lcdcmd(0x01); // Clear display screen
    lcdcmd(0x06); // Increment cursor
    lcdcmd(0x80); // Set cursor position to 1st line, 1st column
}

void lcdcmd(unsigned char cmdout){
    lcdport=cmdout; //Send command to lcdport=PORTB
    rs=0;
    rw=0;
    en=1;
    __delay_ms(1);
    en=0;
}
void lcddata(unsigned char dataout){
    lcdport=dataout; //Send data to lcdport=PORTB
    rs=1;
    rw=0;
    en=1;
    __delay_ms(1);
    en=0;
}

unsigned char readKeyboard(int i){
    LATA = 0b00000000;
    if(i == 0)
        LATA = 0b00000001;
    if(i == 1)
        LATA = 0b00000010;
    if(i == 2)
        LATA = 0b00000100;
    if(i == 3)
        LATA = 0b00001000;    

    if(PORTAbits.RA4 == 1)
        return findKey(i,0);
    else if(PORTAbits.RA5 == 1)
        return findKey(i,1);
    else if(PORTAbits.RA6 == 1)
        return findKey(i,2);
    else if(PORTAbits.RA7 == 1)
        return findKey(i,3);
    else if(LATA == 0b00000000)
        return ' ';
    return ' ';
}

unsigned char findKey(unsigned short a, unsigned short b){
    if(b == 0 && a == 3)
        return '*';
    else if(b == 0 && a == 2)
        return '7';
    else if(b == 0 && a == 1)
        return '4';
    else if(b == 0 && a == 0)
        return '1';
    else if(b == 1 && a == 1)
        return '0';
    else if(b == 1 && a == 2)
        return '8';
    else if(b == 1 && a == 1)
        return '5';
    else if(b == 1 && a == 0)
        return '2';
    else if(b == 2 && a == 3)
        return '#';
    else if(b == 2 && a == 2)
        return '9';
    else if(b == 2 && a == 1)
        return '6';
    else if(b == 2 && a == 0)
        return '3';
    else if(b == 3 && a == 3)
        return 'D';
    else if(b == 3 && a == 2)
        return 'C';
    else if(b == 3 && a == 1)
        return 'B';
    else if(b == 3 && a == 0)
        return 'A';
    else
        return '_';
}

#include <xc.h>
#include <string.h>
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
void MSDelay(unsigned int itime);
void writeString(unsigned char sendData);

unsigned char readKeyboard();
unsigned char findKey(unsigned short a, unsigned short b);

unsigned char data[30]="Test";
unsigned int i=0;

void main(void) {
    TRISA=0; // Configure Port D as output port
    LATA=0;
    PORTA=0;
    //ADCON1 = 15;
    TRISB=0x0F; // configure upper nibble PortB as output, lower nibble as input
    LATB = 0;
    PORTB = 0;
    TRISC=0; // Configure Port C as output port
    LATC=0;
    PORTC=0; // Configure Port D as output port
    TRISD = 0;
    LATD = 0;
    PORTD = 0;

    lcd_ini(); // LCD initialization //
    // write initial display message
    
    unsigned char r;
    TRISB = 0xF0;
    TRISD = 0x00;
    
    while(1){
        // Main code execution here
        // loop until button press is found
        r = readKeyboard();
        //if(r != ' ')
            writeString(r);
        //__delay_ms(500);
        //lcdcmd(0x01); // Clear display screen
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
void MSDelay(unsigned int itime){
    unsigned int i, j;
    for(i=0;i<itime;i++){
        for(j=0;j<10000;j++){
        }
    }
}

unsigned char readKeyboard(){
    unsigned int i = 0;
    
    for(i=0;i<4;i++) {
        if(i == 0)
            PORTB = 1;
        else if(i == 1)
            PORTB = 2;
        else if(i == 2)
            PORTB = 4;
        else if(i == 3)
            PORTB = 8;

        if(PORTBbits.RB4)
            return findKey(i,0);
        if(PORTBbits.RB5)
            return findKey(i,1);
        if(PORTBbits.RB6)
            return findKey(i,2);
        if(PORTBbits.RB7)
            return findKey(i,3);
    }
    return ' ';
}

unsigned char findKey(unsigned short a, unsigned short b){
    if(b == 0) {
        if(a == 3)
            return '*';
        else if(a == 2)
            return '7';
        else if(a == 1)
            return '4';
        else if(a == 0)
            return '1';
    }else if(b == 1){
        if(a == 3)
            return '0';
        else if(a == 2)
            return '8';
        else if(a == 1)
            return '5';
        else if(a == 0)
            return '2';
    }else if(b == 2){
        if(a == 3)
            return '#';
        else if(a == 2)
            return '9';
        else if(a == 1)
            return '6';
        else if(a == 0)
            return '3';
    }else if(b == 3){
        if(a == 3)
            return 'D';
        else if(a == 2)
            return 'C';
        else if(a == 1)
            return 'B';
        else if(a == 0)
            return 'A';
    }
    return 'Z';
}

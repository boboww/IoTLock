#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pic18f45k50.h>

#define _XTAL_FREQ 1000000

#pragma config FOSC = INTOSCIO      // Oscillator Selection bits (Internal oscillator, port function on RA6, EC used by USB (INTIO))
#pragma CONFIG DEBUG = ON;          // Enable Debug Mode
#pragma config WDTEN = OFF          // Watchdog Timer Enable bit (WDT disabled (control is placed on the SWDTEN bit))
#pragma config PBADEN = OFF         // PORTB A/D Enable bit (PORTB&lt;4:0&gt; pins are configured as digital I/O on Reset)
#pragma config MCLRE = ON           // MCLR Pin Enable bit (MCLR pin enabled; RE3 input pin disabled)
#pragma config LVP = ON             // Single-Supply ICSP Enable bit (Single-Supply ICSP LCD Control pins)

#define rs PORTCbits.RC0            // LCD Control pins
#define rw PORTCbits.RC1
#define en PORTCbits.RC2

#define lcdport PORTD               // LCD Data pins

void lcd_ini();                     // Function prototypes

void lcdcmd(unsigned char);
void lcddata(unsigned char);
void writeChar(unsigned char sendData);
void writeInt(unsigned int sendData);
void writeString(unsigned char sendData[]);

unsigned char readKeypad(int i);
unsigned char lookup(unsigned short a, unsigned short b);

unsigned char data[10] = "";        // Global variables
unsigned char string1[16] = " Motor Speed:";
unsigned int i = 0;
unsigned char test1[10] = "";
unsigned char percent = '%';


void main(void) {
    TRISA   = 0xF0;         // Configure Port A as output/input
    LATA    = 0x00;
    PORTA   = 0x00;
    TRISB   = 0x00;         // configure Port B as input port
    LATB    = 0x00;
    PORTB   = 0x00;
    TRISC   = 0x00;         // Configure Port C as output port
    LATC    = 0x00;
    PORTC   = 0x00; 
    TRISD   = 0x00;         // Configure Port D as output port
    LATD    = 0x00;
    PORTD   = 0x00;
    TRISE   = 0x00;         // Configure Port E as output port
    LATE    = 0x00;
    PORTE   = 0x00;
    
    ANSELA  = 0x00;         // RA5 column wasn't working for keypad
        
    lcd_ini();              // LCD initialization   
    
    unsigned char r = ' ';
    int i = 0;
    int j = 0;
    int k = 0;
    int num = 0;
    
    while(1){            
        for(i = 0; i <= 4; i++){
            if(i == 4){
                i = 0;
                break;
            }
            r = ' ';     
            r = readKeypad(i);            // Read keypad when not space
            if(r != ' '){
                writeChar(r);
                data[j] = r;                // Copy keypad input into char array data
                j++;  
                __delay_ms(700);            // Delay to stop repeated key presses
            }
        
            if(r == 'D'){                   // If press button D
                data[j-1] = '\0';
                lcdcmd(0b00000001);         // Clear display screen
                writeString(string1);       // Write "Motor Speed:"
                lcdcmd(0b11000000);         // LCD row 2, column 1
                writeString(data);          // Write data     
                writeChar(percent);         // Write "Motor Speed:"
                
                num = atoi(data);           // num is int for motor values (converted from keypad)
                                        
                lcdcmd(0b10000000);         // LCD row 1, column 1
                j = 0;                
                *data = '\0';
            }if(r == 'C'){                  // If press D
                j = 0;
                lcdcmd(0b00000001);         // Clear display screen                
                *data = '\0';
            }
        }      
    }
    return;
}

void writeChar(unsigned char sendData){         // Writes string to LCD 1 byte at time
    lcddata(sendData);
}

void writeString(unsigned char sendData[]){     // Writes string to LCD 1 byte at time
    int i = 0;
    while(sendData[i] != '\0'){
        lcddata(sendData[i]);                   // Call lcddata function to send characters 
        i++;                                    // one by one from char array
        //__delay_ms(20);
    }
}

void lcd_ini(){
    __delay_ms(15);
    lcdcmd(0b00111000);         // Configure the LCD in 8-bit mode, 2 line and 5x7 font
    lcdcmd(0b00000001);         // Clear display screen
    lcdcmd(0b00001101);         // Display On and Cursor Off
    lcdcmd(0b00000110);         // Increment cursor
    lcdcmd(0b10000000);         // Set cursor position to 1st line, 1st column
}

void lcdcmd(unsigned char cmdout){
    lcdport=cmdout;             //Send command to LCD
    rs=0;
    rw=0;
    en=1;
    __delay_ms(1);
    en=0;
}
void lcddata(unsigned char dataout){
    lcdport=dataout;            // Send data to LCD
    rs=1;
    rw=0;
    en=1;
    __delay_ms(1);
    en=0;
}
            
unsigned char readKeypad(int i){
    LATA = 0b00000000;          // Clear Latch A
    PORTA = 0x00000000;         // Clear PORTB
        
    if(i == 0){                 // Check for loop from main for i values
        LATA = 0b00000001;      // Set Columns high when i value matches
    }if(i == 1){
        LATA = 0b00000010;
    }if(i == 2){
        LATA = 0b00000100;
    }if(i == 3){
        LATA = 0b00001000;    
    }    
    
    if(PORTAbits.RA4 == 1){     // Check for row high
        return lookup(i,0);     // If high, lookup press in lookup table
    }if(PORTAbits.RA5 == 1){
        return lookup(i,1);
    }if(PORTAbits.RA6 == 1){
        return lookup(i,2);
    }if(PORTAbits.RA7 == 1){
        return lookup(i,3);
    }else{
        return ' ';
    }
}

unsigned char lookup(unsigned short a, unsigned short b){
    if(b == 0 && a == 3){       // Lookup table for row/column combinations
        return '*';
    }if(b == 0 && a == 2){
        return '7';
    }if(b == 0 && a == 1){
        return '4';
    }if(b == 0 && a == 0){
        return '1';
    }if(b == 1 && a == 3){
        return '0';
    }if(b == 1 && a == 2){
        return '8';
    }if(b == 1 && a == 1){
        return '5';
    }if(b == 1 && a == 0){
        return '2';
    }if(b == 2 && a == 3){
        return '#';
    }if(b == 2 && a == 2){
        return '9';
    }if(b == 2 && a == 1){
        return '6';
    }if(b == 2 && a == 0){
        return '3';
    }if(b == 3 && a == 3){
        return 'D';
    }if(b == 3 && a == 2){
        return 'C';
    }if(b == 3 && a == 1){
        return 'B';
    }if(b == 3 && a == 0){
        return 'A';
    }else{
        return '_';
    }
}

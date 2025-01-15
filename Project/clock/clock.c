/*
 * clock.c
 *
 *  Created on: 15 Jan 2025
 *      Author: Salih
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.c"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/sysctl.c"
#include "clock.h"

void LcdEnable(){  // Toggle the LCD Enable pin (signal on-off)
    GPIOPinWrite(LCDPORT,EN,0x02);
    Beklet(1000);
    GPIOPinWrite(LCDPORT,EN,0x00);
}

void send_common(char c) {   // For LCD settings, set RS to 0, write to data ports, and toggle enable
    GPIOPinWrite(LCDPORT,ALL_DATA,(c & 0xf0));
    GPIOPinWrite(LCDPORT,RS,0x00);
    LcdEnable();
    Beklet(50000);

    GPIOPinWrite(LCDPORT,ALL_DATA,(c & 0x0f)<<4); // Mask lower nibble values
    GPIOPinWrite(LCDPORT,RS,0x00);
    LcdEnable();
    Beklet(50000);
}

void write_char(char c){  // Set RS to 1, write to data ports, and toggle the enable pin
    GPIOPinWrite(LCDPORT,ALL_DATA,(c & 0xf0));
    GPIOPinWrite(LCDPORT,RS,0x01);
    LcdEnable();
    Beklet(50000);

    GPIOPinWrite(LCDPORT,ALL_DATA,(c & 0x0f)<<4);
    GPIOPinWrite(LCDPORT,RS,0x01);
    LcdEnable();
    Beklet(50000);
}

void home(){  // Return LCD cursor to home position
    send_common(0x02);
    Beklet(1000);
}

void Lcd_clear(void){ // Clear the LCD screen and return cursor to home position
    send_common(0x01);   // Clear LCD
    send_common(0x02);   // Return home
    Beklet(1000);
}

void LcdDisplayOn(){  // Turn on the LCD screen, cursor off, and blink off
    send_common(0x0c);  // Display on only
    Beklet(1000);
}

void LcdCursorOn(){ // Turn on the LCD screen and cursor
    send_common(0x0e); // Both display and cursor on
    Beklet(1000);
}

void LcdBlinkOn(){ // Turn on the LCD screen, cursor, and blink
    send_common(0x0f);  // Display, cursor, and blink on
    Beklet(1000);
}

void Gotox_y(char row, char col){ // Set LCD row and column position
    char adress;    // Starting point is set to 0,0
    if(row == 0)
        adress=0;
    else if(row==1)
        adress=0x40;
    else
        adress=0;
    adress|=col;
    send_common(0x80 | adress);  // Addressing in LCD starts from 0x80h, (row 0 -> 80h+column, row 1 -> 80h+40h+column)
}

void Lcdwritestring(const char dizi[], int dizisayisi){ // Write multiple characters to LCD using a loop
    char i=0;
    while(dizisayisi--){ // Write characters to the screen until the array size reaches 0
        write_char(dizi[i++]);
    }
}

void Lcd_dec(){ // LCD initialization settings
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);    // Send 30h three times
    GPIOPinTypeGPIOOutput(LCDPORT, 0xFF);
    Beklet(50000);
    GPIOPinWrite(LCDPORT,RS,0x00);
    GPIOPinWrite(LCDPORT, ALL_DATA,  0x30 );
    LcdEnable();
    Beklet(50000);
    GPIOPinWrite(LCDPORT, ALL_DATA,  0x30 );
    LcdEnable();
    Beklet(50000);
    GPIOPinWrite(LCDPORT, ALL_DATA,  0x30 );
    LcdEnable();
    Beklet(50000);

    GPIOPinWrite(LCDPORT, ALL_DATA,  0x20 ); // 0B0010 // 4BIT LCD 0B001DL
    Beklet(50000);
    GPIOPinWrite(LCDPORT, ALL_DATA,  0xF0);  // 0BNF** // N=1 for 2 rows, F=1 for 5x10 dots
    Beklet(50000);

    send_common(0x0e); // Screen on, cursor on
    send_common(0x01); // Clear screen
    send_common(0x06); // Entry mode set 0b0I/DS0 // ID=1 for automatic cursor movement, S=0 for static screen
    Lcd_clear();
}

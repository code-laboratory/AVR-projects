#include <mega32a.h>
#include <delay.h>

unsigned char seven_seg_ca[16] = { 0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90 };
unsigned char seven_seg_cc[16] = { 0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F };
unsigned short int color = 0; // 0: red, 1: yellow, 2: green

void main(void)
{ 
    DDRA = 0xFF;
    DDRC = 0xFF;

    while (1) {
        int i, countdown_init;
        switch (color) {
            case 0:
                PORTC.0 = 1;
                PORTC.1 = 0;
                PORTC.2 = 0;
                countdown_init = 5;
                break;
            case 1:
                PORTC.0 = 0;
                PORTC.1 = 1;
                PORTC.2 = 0;
                countdown_init = 4;
                break;
            case 2:
                PORTC.0 = 0;
                PORTC.1 = 0;
                PORTC.2 = 1;
                countdown_init = 9;
                break;
        }
        
        for (i = countdown_init; i >= 0; i--) {
            PORTA = seven_seg_cc[i];
            delay_ms(500);
        }
        
        if (color == 0) color = 2;
        else if (color == 1) color = 0;
        else if (color == 2) color = 1;
    }
}

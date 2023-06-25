#include <mega32a.h>
#include <stdio.h>
#include <alcd.h>
#include <delay.h>
#include <stdlib.h>
#include <string.h>

#define ENTER 0
#define RESET 1
#define OPEN  2

void keypad(void);
unsigned char key;
char output[32];
char passcode[5] = "1234";
char buffer[5];
char counter = 0;
char mode = 0;

void main(void)
{
    DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
    PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0); 
    DDRB=(1<<DDB7) | (1<<DDB6) | (1<<DDB5) | (1<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
    PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (1<<PORTB3) | (1<<PORTB2) | (1<<PORTB1) | (1<<PORTB0);
    DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
    PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);
    DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
    PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);
    TCCR0=(0<<WGM00) | (0<<COM01) | (0<<COM00) | (0<<WGM01) | (0<<CS02) | (0<<CS01) | (0<<CS00);
    TCNT0=0x00;
    OCR0=0x00;
    TCCR1A=(0<<COM1A1) | (0<<COM1A0) | (0<<COM1B1) | (0<<COM1B0) | (0<<WGM11) | (0<<WGM10);
    TCCR1B=(0<<ICNC1) | (0<<ICES1) | (0<<WGM13) | (0<<WGM12) | (0<<CS12) | (0<<CS11) | (0<<CS10);
    TCNT1H=0x00;
    TCNT1L=0x00;
    ICR1H=0x00;
    ICR1L=0x00;
    OCR1AH=0x00;
    OCR1AL=0x00;
    OCR1BH=0x00;
    OCR1BL=0x00;
    ASSR=0<<AS2;
    TCCR2=(0<<PWM2) | (0<<COM21) | (0<<COM20) | (0<<CTC2) | (0<<CS22) | (0<<CS21) | (0<<CS20);
    TCNT2=0x00;
    OCR2=0x00;
    TIMSK=(0<<OCIE2) | (0<<TOIE2) | (0<<TICIE1) | (0<<OCIE1A) | (0<<OCIE1B) | (0<<TOIE1) | (0<<OCIE0) | (0<<TOIE0);
    MCUCR=(0<<ISC11) | (0<<ISC10) | (0<<ISC01) | (0<<ISC00);
    MCUCSR=(0<<ISC2);
    UCSRB=(0<<RXCIE) | (0<<TXCIE) | (0<<UDRIE) | (0<<RXEN) | (0<<TXEN) | (0<<UCSZ2) | (0<<RXB8) | (0<<TXB8);
    ACSR=(1<<ACD) | (0<<ACBG) | (0<<ACO) | (0<<ACI) | (0<<ACIE) | (0<<ACIC) | (0<<ACIS1) | (0<<ACIS0);
    SFIOR=(0<<ACME);
    ADCSRA=(0<<ADEN) | (0<<ADSC) | (0<<ADATE) | (0<<ADIF) | (0<<ADIE) | (0<<ADPS2) | (0<<ADPS1) | (0<<ADPS0);
    SPCR=(0<<SPIE) | (0<<SPE) | (0<<DORD) | (0<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);
    TWCR=(0<<TWEA) | (0<<TWSTA) | (0<<TWSTO) | (0<<TWEN) | (0<<TWIE);

    // Alphanumeric LCD initialization
    // Connections are specified in the
    // Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
    // RS - PORTA Bit 0
    // RD - PORTA Bit 1
    // EN - PORTA Bit 2
    // D4 - PORTA Bit 3
    // D5 - PORTA Bit 4
    // D6 - PORTA Bit 5
    // D7 - PORTA Bit 6
    // Characters/line: 16
    lcd_init(16);
    lcd_clear();
    lcd_puts("LOCKED");

    while (1) {
        keypad();
        
        if (PIND.0 == 0 && (mode == OPEN || mode == RESET)) {
            key = 12;
            counter = 0;
            strcpy(output, "LOCKED");
            mode = ENTER;
        }

        if (counter == 0) buffer[0] = '\0';

        if (mode == OPEN) {
            if (key == '#') {
                mode = RESET;
                counter = 0;
                strcpy(output, "ENTER NEW\nPASS CODE");
            }
        } else if (key != 0 && key != '#' && key != 12) {
            buffer[counter] = key;
            buffer[counter + 1] = '\0';
            counter++;
            strcpy(output, buffer);
        }
            
        if (counter == 4) {
            if (mode == RESET) {
                strcpy(passcode, buffer);
                strcpy(output, "PASS CODE\nCHANGED.");
                mode = OPEN;
            } else {    
                if (strcmp(passcode, buffer) == 0) {
                    mode = OPEN;
                    strcpy(output, "OPEN");
                } else {
                    strcpy(output, "WRONG PASS CODE");
                }
            }
            counter = 0;
        }
        
        if (key != 0) {
            lcd_clear();
            lcd_puts(output);
        }
    }
}

void keypad (void) {
    PORTB = 0xF0;
    delay_ms(2);
    key = 0;

    /* ------ ROW 1 ------ */
    PORTB.4 = 0;
    delay_ms(2);
    while (PINB.0 == 0) key = '*';
    while (PINB.1 == 0) key = '7';
    while (PINB.2 == 0) key = '4';
    while (PINB.3 == 0) key = '1';
    PORTB.4 = 1;
    
    /* ------ ROW 2 ------ */
    PORTB.5 = 0;
    delay_ms(2);
    while (PINB.0 == 0) key = '0';
    while (PINB.1 == 0) key = '8';
    while (PINB.2 == 0) key = '5';
    while (PINB.3 == 0) key = '2';
    PORTB.5 = 1;
    
    /* ------ ROW 3 ------ */
    PORTB.6 = 0;
    delay_ms(2);
    while (PINB.0 == 0) key = '#';
    while (PINB.1 == 0) key = '9';
    while (PINB.2 == 0) key = '6';
    while (PINB.3 == 0) key = '3';
    PORTB.6 = 1;
}
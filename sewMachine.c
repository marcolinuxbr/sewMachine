#define F_CPU 1000000UL //for delay.h

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h> //rand
#include <avr/eeprom.h>
#include <avr/interrupt.h>


#define HIGH 1 // 5V
#define LOW 0 // 0V

#define dirPort0 DDRD
#define outPort0 (*((volatile bits_t*)(&PORTD)))
#define outPin0 PD2
#define outPin1 PD3
#define outPin2 PD4
#define outPin3 PD5
#define buzzerPin  PD6
#define  out0 outPort0.bit2
#define  out1 outPort0.bit3
#define  out2 outPort0.bit4
#define  out3 outPort0.bit5
#define  buzzer outPort0.bit6


#define dirPort1 DDRB
#define port1 PORTB
#define outPort1 (*((volatile bits_t*)(&PORTB)))
#define inPort1 PINB //(*((volatile bits_t*)(&PINB)))
#define sensorPin  PB0
#define motorPin  PB2
#define outMotor  outPort1.bit2 //PB2
//#define sensor  inPort1.bit0 //PB0



typedef struct {
    unsigned char bit0:1;
    unsigned char bit1:1;
    unsigned char bit2:1;
    unsigned char bit3:1;
    unsigned char bit4:1;
    unsigned char bit5:1;
    unsigned char bit6:1;
    unsigned char bit7:1;
} bits_t;


void setup(void){

//2 leds
    dirPort0 |=(1<<outPin2);
    dirPort0 |=(1<<outPin3);

    out2 = LOW;
    out3 = LOW;
 // input pin
    inPort1 &= ~(1 << sensorPin);    // switch on pin PBx
    port1 |= (1 << sensorPin);    // enable pull-up resistor

//beep
   dirPort0 |=(1<<buzzerPin);

//motor pwm
  dirPort1 |= (1<<motorPin);    // make OC0A an output
  TCCR0B = 0;                                    // stop timer 0
  TCCR0A = (1<<WGM01)|(1<<WGM02);                // select fast pwm mode 3
  TCCR0A |= (1<<COM0A1);                         // clear OC0A on compare match
  OCR0A = 0x7e;                                  // 0x7f: 50% duty cycle
  TCCR0B = (1<<CS00);                            // prescale = 1.  freq = clock / 256
//TCCR0B = (1 << CS01);   // clock source = CLK/8, start PWM
}

void sleep(uint8_t millisec){
    while(millisec){
        _delay_ms(1);
        millisec--;
    }
}

void debug(uint8_t zeroUm){

    uint8_t sleepTime;
    if(zeroUm==0){
        sleepTime = 255; //zero, blink slow
    }else{
        sleepTime = 75; //1, blink fast
    }

        //blink LED 0
        sleep(sleepTime);
        out2 = LOW;
        out3 = HIGH;

        sleep(sleepTime);
        out3 = LOW;
        out2 = HIGH;

        sleep(sleepTime);
        out3 = LOW;
        out2 = LOW;

}


void pinMode(uint8_t pin, uint8_t inOut){
//debug(inOut);
    if(inOut==0){
        dirPort0 &=~(1<<pin);//input (tristate)
    }else{
        dirPort0 |=(1<<pin);//output

    }
}


void beep(uint8_t milliSec){
    buzzer =  HIGH;
    sleep(milliSec);
    sleep(milliSec);
    buzzer =  LOW;
}


int main(void){

setup();


debug(0);
beep(25);
debug(1);
//outMotor = HIGH;

while(1){
//      if ((PINB & (1 << PB0)) == 0){
        if ((inPort1 & (1 << sensorPin)) == 0){
            out3 = HIGH;
        }else{
            out3 = LOW;
        }

    }

}


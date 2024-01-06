#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

int S1A = 0;
int S1B = 1;
int S2A = 2;
int S2B = 3;
int S3A = 4;
int S3B = 5;

int pin_H1 = 2;
int pin_H2 = 3;
int pin_H3 = 8;
int pin_Dir = 5;
int pin_Brake = 6;

bool Hall_1, Hall_2, Hall_3, Dir, S;

ISR(INT0_vect)
{
  //External Interrupt in pin 2

  if(PIND&(1<<2))
  {
    Hall_1 = true;
  }
  else
  {
    Hall_1 = false;
  }
}

ISR(INT1_vect)
{
  //External Interrupt in pin 3

  if(PIND&(1<<3))
  {
    Hall_2 = true;
  }
  else
  {
    Hall_2 = false;
  }
}

ISR(PCINT0_vect)
{
  //Excecutes Instructions for PCINT0 to PCINT7 (PORT B)

  if(PINB&(1<<0))
  {
    Hall_3 = true;
  }
  else
  {
    Hall_3 = false;
  } 
}

ISR(PCINT2_vect)
{
  //Excecutes Instructions for PCINT16 to PCINT23 (PORT D)
 
  if(PIND&(1<<5))
  {
    Dir = true;
  }
  else
  {
    Dir = false;
  }
}

int main()
{
  DDRC = 0x3f; //Setting A0 - A5 as output pins//
  
  DDRD = DDRD&(~(1<<pin_H1));    //Setting pin 2 as input
  DDRD = DDRD&(~(1<<pin_H2));    //Setting pin 3 as input
  DDRD = DDRD&(~(1<<pin_Dir));   //Setting pin 5 as input
  DDRD = DDRD&(~(1<<pin_Brake)); //Setting pin 6 as input
  DDRB = DDRB&(~(1<<pin_H3));    //Setting pin 8 as input

  PORTC = B00010101; //Initializing the A0 - A5 as high-low-high-low-high-low state so all the three phases are in non connected state//

  PORTD = PORTD | (1<<pin_Dir);   //Setting pin 5 as an input pullup
  PORTD = PORTD | (1<<pin_Brake); //Setting pin 6 as an input pullup
  
  PCICR = PCICR | (1<<PCIE2);     //Enable PCMSK2 (Group 2: PCINT16-PCINT23)
  PCICR = PCICR | (1<<PCIE0);     //Enable PCMSK0 (Group 0: PCINT0-PCINT7)

  EICRA = EICRA | (1<<ISC00) | (1<<ISC10);    //Enabaling External Interrupts(When logic level toggles)

  EIMSK = EIMSK | (1<<INT0) | (1<<INT1);      //Enabaling Enternal Interrupts in pin 2&3
  
  PCMSK2 = PCMSK2 | (1<<PCINT21);    		  //Enabaling pin change inpterrupt in pin 5
  PCMSK0 = PCMSK0 | (1<<PCINT0);              //Enabaling pin change inpterrupt in pin 8
  
  while(1)
  {
    if(!(PIND&(1<<6)))
    {
      sei();
      E1A(Dir, Hall_1, Hall_2, Hall_3);
      E1B(Dir, Hall_1, Hall_2, Hall_3);
      E2A(Dir, Hall_1, Hall_2, Hall_3);
      E2B(Dir, Hall_1, Hall_2, Hall_3);
      E3A(Dir, Hall_1, Hall_2, Hall_3);
      E3B(Dir, Hall_1, Hall_2, Hall_3);
    }
    else
    {
      /Locks the commutation in the phase just before the brake was applied untill the brake is not released/
      cli();
    }

    _delay_ms(10);
  }
}

void E1A(bool D, bool H1, bool H2, bool H3)
{
  bool temp;
  temp = (D)&(~H2)&(H1) | (~H3)&(H2)&(H1) | (H3)&(~H2)&(~H1) | (~D)&(H2)&(~H1);

  if(temp == true)
  {
    PORTC = PORTC | (1<<S1A);
  }
  else
  {
    PORTC = PORTC & (~(1<<S1A));
  }
}

void E1B(bool D, bool H1, bool H2, bool H3)
{
  bool temp;
  temp = (D)&(~H2)&(H1) | (~D)&(H2)&(~H1);

  if(temp == true)
  {
    PORTC = PORTC | (1<<S1B);
  }
  else
  {
    PORTC = PORTC & (~(1<<S1B));
  }
}

void E2A(bool D, bool H1, bool H2, bool H3)
{
  bool temp;
  temp = (~H3)&(~H2)&(H1) | (D)&(~H3)&(H2) | (H3)&(H2)&(~H1) | (~D)&(H3)&(~H2);

  if(temp == true)
  {
    PORTC = PORTC | (1<<S2A);
  }
  else
  {
    PORTC = PORTC & (~(1<<S2A));
  }
}

void E2B(bool D, bool H1, bool H2, bool H3)
{
  bool temp;
  temp = (D)&(~H3)&(H2) | (~D)&(H3)&(~H2);

  if(temp == true)
  {
    PORTC = PORTC | (1<<S2B);
  }
  else
  {
    PORTC = PORTC & (~(1<<S2B));
  }
}

void E3A(bool D, bool H1, bool H2, bool H3)
{
  bool temp;
  temp = (D)&(H3)&(~H1) | (H3)&(~H2)&(H1) | (~D)&(~H3)&(H1) | (~H3)&(H2)&(~H1);

  if(temp == true)
  {
    PORTC = PORTC | (1<<S3A);
  }
  else
  {
    PORTC = PORTC & (~(1<<S3A));
  }
}

void E3B(bool D, bool H1, bool H2, bool H3)
{
  bool temp;
  temp = (D)&(H3)&(~H1) | (~D)&(~H3)&(H1);

  if(temp == true)
  {
    PORTC = PORTC | (1<<S3B);
  }
  else
  {
    PORTC = PORTC & (~(1<<S3B));
  }
}

#include "io430.h"
//Função de tratamento de interrupção da porta P2.0
#pragma vector = PORT2_VECTOR   //Define o endereço da RTI da porta 2
__interrupt void PORT2 (void)
{
  P1OUT ^= 0X20;  // Inverte o estado de P1.5
  __delay_cycles(100000);
  P2IFG &= ~0x01; // desliga o flag de interrupção
  
}
// Função do repique
char DebounceSwitch2(void)
{
  static unsigned int State= 0; // Current debounce status
  State=(State<<1) | P6IN_bit.P5                 | 0xe000;
  if(State==0xf000)return 1;
  return 0;
}

int PISCA_LED ( )
{
    if(DebounceSwitch2())//le tecla 5
    {
      P1OUT ^= BIT6; //
    __delay_cycles(10000); 
  }  
  return 0;
}


void main( void )
{
  
  WDTCTL = WDTPW + WDTHOLD;        // Desliga watchdog timer
  P1DIR = 0XF0;                    // Define P1.7,6,5,4 como saída
  P6DIR |= 0x10;                   // Define P3.0 como saída
  P1OUT = 0;                       // Apaga todos os leds
  P6OUT_bit.P4 |= 1;               // Coloca nível lógico 1 na linha da tecla
  P2IES &= 0XFE;                   // Define interrupção de P2.0 pela borda de subida.
  P2IE |= 0X01;                    // Habilita interrupção da porta P2.0.
  
  P2DIR |= 0X02;                   // Define P2.1 como saída
  P6DIR &= 0XDF;                   // Define P6.5 como entrada
 __enable_interrupt();            // Habilita interrupção geral.
  while(1)
 {
   if (DebounceSwitch2()){
     PISCA_LED();
     P6OUT ^= BIT6;
 }
 __delay_cycles(3000);
}
}

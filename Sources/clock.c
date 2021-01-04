
/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Electronicos Programables
*******************************************************************
* nombre fichero : clock.c 
*******************************************************************
* proyecto       : Realizacion de un reloj en el S08SV16
* programador    : JL Villarroel 
* lenguaje       : ANSI C para CodeWarrior 
* fecha          : 29 mayo 2018
********************************************************************/


/*******************************************************************

Suponemos Internal Reference Clock
FLL engaged
BDIV = 1 => /2
BUSCLK = 5 MHz
El tick del reloj de sistema se fija en 1 ms

********************************************************************/

#include "derivative.h"
#include "clock.h"


/* Variables del modulo */

volatile static unsigned int tick_counter ;
static unsigned int Timer;
static char TO=0;
static char Active_Timer=0;


void (*timer_function)(void);  


/* Implementacion servicios */

void interrupt 6 Tick (void) {

  if (MTIMSC_TOF) MTIMSC_TOF = 0 ;     /* clear TOF */
  tick_counter ++ ;
  if (Active_Timer)
    if (Timer == tick_counter){
      TO = 1 ;
      Active_Timer = 0 ;
      if (timer_function) (*timer_function)();
     }
  return ;
}



void Init_Clock (void) {

  asm  sei ;
 
  tick_counter = 0 ;
  
  MTIMCLK_PS = 5;        // PS = 0101, ClockSource/32, 5 MHz/32 (6.4 us)
  MTIMCLK_CLKS = 0;      // CLKS = 00, ClockSource = BUSCLK 
  MTIMMOD = 155 ;        // tick = 1 ms (aprox) */
  
  MTIMSC_TRST = 1 ;      // reset MTIM
  MTIMSC_TOIE = 1 ;      // enable MTIM int
  MTIMSC_TSTP = 0 ;      // start MTIM

  asm cli ;
  
  return;
}


void Start_Clock (void) {

  MTIMSC_TOIE=1;
  return ;
}

void Stop_Clock (void) {

  MTIMSC_TOIE=0;
  return;

}

unsigned int Get_Time (void) {

  unsigned int Aux_TC ;

  asm sei ;
  Aux_TC = tick_counter ;
  asm cli ;
  return Aux_TC ;
}

void delay_until(unsigned int T){

  while(1)  {
    asm sei ;
    if  (T==tick_counter) {asm cli ; return ; } 
    else asm WAIT ; 
  }     
}

void Set_Timer (unsigned int Ticks, void (*p)(void)) {

  Timer = Get_Time() + Ticks ;
  timer_function = p ;
  TO = 0 ;
  Active_Timer = 1 ;
}


char Time_Out (void) {
  return TO ;
}


void Remove_Timer (void) {
  Active_Timer = 0 ;
  TO = 0 ;
}

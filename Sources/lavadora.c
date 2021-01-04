/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Empotrados I
*******************************************************************
* nombre fichero : lavadora.c  
*******************************************************************
* proyecto       : Motor LEGO
* descripcion    : Control de una lavadora simulada
* programador    : JL Villarroel (JLV) 
* fecha          : 06 / 05 / 2020
*******************************************************************/

/******************************************************************
*                     INCLUDES                                    *
*******************************************************************/

#include "derivative.h"

#include "motor.h"
#include "clock.h"

/******************************************************************
*                     PROTOTIPOS FUNCIONES                        *
*******************************************************************/

static void InitSystem (void) ;  
void Control_Motor (void) ;
void Control_Discreto (void) ;
void Visualizacion (void) ;
unsigned char LeerEntrada(void) ;

void clrLEDs(unsigned char mask);
void addLEDs(unsigned char mask);
void setLEDs(unsigned char mask);

/******************************************************************
*                     SIMBOLOS                                    *
*******************************************************************/

/* Las entradas se leen del puerto A
     PTA1 --> B0
     PTA2 --> B1
     PTA3 --> B2
     PTA5 --> S0
     PTA6 --> S1
     PTA7 --> S2  */
     

// Pulsadores
#define M       0x02     // 0000_0010
#define S1      0x20     // 0010_0000
#define S2      0x40     // 0100_0000

// LEDs
#define LEDM    0x04     // 0000_0100
#define LED1    0x08     // 0000_1000
#define LED2    0x10     // 0001_0000


#define ESCALAR_W  1.0 // Conversion de rad/s
/******************************************************************
*                     VARIABLES GLOBALES                          *
*******************************************************************/

unsigned char marco = 0, num_marcos = 10; // 10 marcos
unsigned int siguiente, m = 10; // cada 10ms

enum {Prog1, Prog2, Prog1Seleccionado, Prog2Seleccionado, Stop} estado ;
unsigned char Entrada ;

float W, W_ref, U=0.0, U_ant=0.0, E, E_ant = 0.0 ;
volatile unsigned char Paso = 0, Programa = 1 ;
unsigned char V_PASO = 0, V_PROG = 1 ;

float buffer [150] ;

char j = 0 ;


/******************************************************************
*                     MAIN                                        *
*******************************************************************/


void main(void) {
 
 InitSystem () ;
 
 Init_Clock () ;
 
 j = 0 ;
 W_ref = 0; // Vel angular inicial (parado)
 estado = Prog1Seleccionado; // seleccionado pero no activado (marcha no pulsado)

 siguiente = Get_Time () ;
  
 while(1) {
 
    marco = (marco % num_marcos)+1 ;
    switch (marco) {
      case 1: Visualizacion(); Control_Discreto(); Control_Motor(); break;
      case 2: Visualizacion(); break;
      case 3: Visualizacion(); break;
      case 4: Visualizacion(); break;
      case 5: Visualizacion(); break;
      
      case 6: Visualizacion(); Control_Discreto(); break;
      case 7: Visualizacion(); break;
      case 8: Visualizacion(); break;
      case 9: Visualizacion(); break;
      case 10: Visualizacion(); break;
    }
    
    siguiente += m ;
    delay_until (siguiente) ;
   }
}


/******************************************************************
*                     FUNCIONES                                   *
*******************************************************************/

void InitSystem (void) {

  /* Internal Clock Source Configuration */
	if(NVICSTRM == 0xFF)			// If programmed trim value is blank
		ICSTRM = 0x80;				  // load default value
	else
		ICSTRM = NVICSTRM;	  	// else load programmed trim value
  ICSC2_BDIV = 1 ;          // Divides selected clock by 2		
	                          // Bus clock = 5 MHz, CPU clock = 10MHz

  
  SOPT1_COPT = 0 ;          // WATCHDOG disable 
  
  PTBD  = 0b00001000 ;
  PTBDD = 0b00011100 ;      // PTB0..6 as outputs
  // ... 2,3,4
  

}

/******************************************************************/

void Control_Motor (void) {
   
   W = Read_W (); // vel angular actual
   E = W_ref - W; // error
   
   buffer[j] = W ;  
   //bufferError[j] = E;
   j = (j + 1)%150 ;

   // u(k) = (2.86*T+0.286)e(k) - 0.286 e(k-1) + u(k-1)
   // T = 100ms = 0.1s
   // (2.86*T + 0.286)*e(k) = 0.572*e(k)
   U = 0.572*E - 0.286*E_ant + U_ant;
   
   // Rangos de U
   if (U > 5) 
      U = 5;
   else if (U<-5) U = -5;
   
   Set_U (U) ;
   
   E_ant = E;
   U_ant = U;
   
}

/******************************************************************/

void Visualizacion (void) {
  
  V_PROG = Programa ;
  V_PASO = Paso ;
  
}

/******************************************************************/


void Control_Discreto (void) {
  
  Entrada = LeerEntrada();
    
  switch(estado){
    case Prog1Seleccionado: 
      setLEDs(LED1); // solo LED1
      if(Entrada & M) {
        estado = Prog1;
        Paso = 5;
        Set_Timer(4000,0); // paso 5: 4s
        W_ref = 1 * ESCALAR_W;
      } else if (Entrada & S2) { // cambiar a P2
        estado = Prog2Seleccionado;
        Programa = 2;
      } // Pulsar S1 no hace nada
      break;
    case Prog2Seleccionado: 
      setLEDs(LED2); // Solo LED2
      if(Entrada & M) {
        estado = Prog2;
        Paso = 4;
        Set_Timer(2000,0); // paso 4: 2s
        W_ref = -0.5 * ESCALAR_W;
      } else if (Entrada & S1) { // cambiar a P1
        estado = Prog1Seleccionado;
        Programa = 1;
      } // Pulsar S1 no hace nada
      break;
    case Prog1: // Prog 1 en marcha
      addLEDs(LEDM); // Se añade el LED de marcha
      if (Time_Out()){ // Se debe terminar el ciclo anterior en cualquier caso
        if (Entrada & M) { 
          // Si se presiona M de nuevo, se para y se vuelve a la seleccion
          // (esta dentro del Time_Out para permitir un margen de pulsacion)
          Paso = 0;
          W_ref = 0;
          estado = Prog1Seleccionado;
        } else {
          // Automata de los Pasos:
          switch (Paso) {
            case 5:
              Paso = 4; 
              Set_Timer(4000,0);
              W_ref = -1 * ESCALAR_W;
              break;
            case 4:
              Paso = 3; 
              Set_Timer(4000,0);
              W_ref = 1 * ESCALAR_W;
              break;    
            case 3:
              Paso = 2; 
              Set_Timer(4000,0);
              W_ref = 0;
              break;
            case 2:  
              Paso = 1; 
              Set_Timer(10000,0); // 10 s
              W_ref = 3 * ESCALAR_W;
              break;
            case 1:
              Paso = 0;
              W_ref = 0;
              estado = Prog1Seleccionado; // Volvemos a la seleccion de Prog1
              break;
          } 
        }
      }
      break;
    case Prog2: // Prog 2 en marcha
      addLEDs(LEDM); // Se añade el LED de marcha
      if (Time_Out()){ // Se debe terminar el ciclo anterior en cualquier caso
        if (Entrada & M) { 
          // Si se presiona M de nuevo, se para y se vuelve a la seleccion
          Paso = 0;
          W_ref = 0;
          estado = Prog1Seleccionado;
        } else {
          // Automata de los Pasos:
          switch (Paso) {
            case 4:  
              Paso = 3; 
              Set_Timer(4000,0);
              W_ref = 1 * ESCALAR_W;
              break;    
            case 3:  
              Paso = 2; 
              Set_Timer(2000,0); // 2 s
              W_ref = 0;
              break;
            case 2:  
              Paso = 1; 
              Set_Timer(6000,0); // 6 s
              W_ref = 2 * ESCALAR_W;
              break;
            case 1:
              Paso = 0;
              W_ref = 0;
              estado = Prog1Seleccionado; // Volvemos a la seleccion de Prog1
              Programa = 1;
              break;
          } 
        }
      }
      break;
    }
}
/**************************************************************/

unsigned char LeerEntrada(void)
{
   return (PTAD)&0b01100010 ;
}

// Apaga los LEDs en mask, dejando el resto como estaban
void clrLEDs(unsigned char mask) {
   PTBD &= ~mask;
}

// Enciende los LEDs en mask, dejando el resto como estaban
void addLEDs(unsigned char mask) {
   PTBD |= mask;
}

// Enciende los LEDs de mask, apagando el resto
void setLEDs(unsigned char mask) {
   PTBD = mask;
}

  
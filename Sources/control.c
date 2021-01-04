/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Empotrados I
*******************************************************************
* nombre fichero : lavadora.c  
*******************************************************************
* proyecto       : Motor LEGO
* descripcion    : Control de velocidad de un motor LEGO
* programador    : JL Villarroel (JLV) 
* fecha          : 06 / 05 / 2020
*******************************************************************/

/******************************************************************
*                     INCLUDES                                    *
*******************************************************************/

#include "derivative.h"

#include "motor.h"
//#include "clock.h"

/******************************************************************
*                     PROTOTIPOS FUNCIONES                        *
*******************************************************************/

static void InitSystem (void) ;  
void display (unsigned char queDisplay, unsigned char valor) ;


/******************************************************************
*                     VARIABLES GLOBALES                          *
*******************************************************************/


unsigned int siguiente, periodo = 100 ;

float W, W_ref, U=0.0, E, E_ant = 0.0 ;

float buffer [150] ;
char i = 0, j = 0 ;


/******************************************************************
*                     MAIN                                        *
*******************************************************************/


void main(void) {
 
 InitSystem () ;
 
// Init_Clock () ;
 
// siguiente = Get_Time () ;

 
 i = 0 ;
 j = 0 ;
 W_ref = 2.0 ;

  
 while(1) {

   Control_Motor () ;


//   siguiente += periodo ;
//   delay_until (siguiente) ;
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
  
       PTCD = 0x70 ;
     PTCDD = 0x7F ;         // PTC0..6 as outputs

}


void Control_Motor (void) {

   W = Read_W () ;
   
   buffer[j] = W ;  
   j = (j + 1)%150 ;

   E = W_ref - W ;
   U = 0.5*E - 0.4524*E_ant + U ;
   
   Set_U (U) ;
   
   E_ant = E ;
   
}

  
  
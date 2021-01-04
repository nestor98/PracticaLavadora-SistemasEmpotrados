/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Electronicos Programables
*******************************************************************
* nombre fichero : clock.h 
*******************************************************************
* descripcion    : Fichero de cabecera modulo reloj de sistema
*                  Tick del reloj 1 ms
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C para CodeWarrior
* fecha          : 29 mayo 2018
********************************************************************/

#ifndef clock_h
#define clock_h

void Init_Clock (void) ;
   /* Instalacion del reloj y puesta a cero*/
   
void Start_Clock (void) ;
   /* Puesta en marcha del reloj */
   
void Stop_Clock (void) ;
   /* Parada del reloj */
   
unsigned int Get_Time (void) ;
   /* Obtención de la cuenta del reloj*/
   
void delay_until(unsigned int T);
    /* Espera bloqueante hasta que la cuenta del reloj llegue
       a T */
       
void Set_Timer (unsigned int Ticks, void (*p)(void)) ;
    /* Arranque de un timer avisa en t+Ticks y ejecuta p */

char Time_Out (void) ;
    /* Consulta si el timer ha expirado */

void Remove_Timer (void) ;
    /* Elimina un timer */


#endif
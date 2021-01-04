/******************************************************************
* Area Ingenieria de Sistemas y Automatica
* Sistemas Electronicos Programables
*******************************************************************
* nombre fichero : motor.c 
*******************************************************************
* descripcion    : Simulación dinámica motor corriente continua
* programador    : JL Villarroel (JLV) 
* lenguaje       : ANSI C para CodeWarrior
* fecha          : 1 mayo 2020
********************************************************************/

#include "derivative.h"
#include "motor.h"

static float Vel = 0.0, Vel_1 = 0.0 ;
static float Uk = 0.0, Uk_1 = 0.0 ;


void Init_Motor (void) {


}

void Set_U (float U) {

   Uk = U ;
   Vel = 0.9048*Vel_1 + 0.2855*Uk_1 ;
   Uk_1 = Uk ;
   Vel_1 = Vel ;
}

float Read_W (void) {
   return Vel ;
}
   

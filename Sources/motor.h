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

#ifndef motor_h
#define motor_h

void Init_Motor (void)  ;

void Set_U (float U) ;

float Read_W (void) ;

#endif
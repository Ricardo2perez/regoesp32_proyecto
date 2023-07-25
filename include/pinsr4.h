 /*
 * Configuracion de entadas saidas para ReGo 4.00
 * xdesig 2023
 */

#ifndef _PINSR4_H
#define _PINSR4_H


//*** Entradas ***
#define KEY A0      //SPV Entrada teclado

// sensores humidade
#define IN_1 26      //Entrada sensor 1
#define IN_2 39      //Entrada sensor 2
#define IN_3 35      //Entrada sensor 3
#define IN_4 33      //Entrada sensor 4

// finais de carrera
#define IL_1 18      //Entrada Activo baixo 1
#define IL_2 19      //Entrada Activo baixo 2
#define IH_1 23      //Entrada Activo alto 1
#define IH_2 34      //Entrada Activo alto 2

//*** Salidas ***
// DTH
#define DTHpin 17        //Entrada sensor humidade temperatura DTH

// RTC PCF8523 soldado PCF8563!
#define INT 4         //Interrupción RTC

// Electrovalvúlas
#define SAL_1 27       // Electrovalvúla 1
#define SAL_2 25       // Electrovalvúla 2
#define SAL_3 32       // Electrovalvúla 3
#define SAL_4 12       // Electrovalvúla 4

// Motores
#define ACT_1 0       // Acitivación 1
#define ACT_2 2       // Acitivación 2

// LED Auto/Manual
#define RED 10        // RED   (RED = 1 GREEN = 0)
#define GREEN 9       // GREEN (RED = 0 GREEN = 1)

#endif // _PINSR4_H

// EOF
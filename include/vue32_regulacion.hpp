// vue32_regulacion.hpp

/* -------------------------------------------------------------------
   ReGo V4 
     Programado por: ricardo2perez@gmail.com
     Proyecto hardware: https://github.com/xdesig/ReGo
 --------------------------------------------------------------------

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 USA.

  See LICENSE.txt for details
  */

#include <Arduino.h>
//--------------------------------------
// Regulación de temperatura y humedad
//--------------------------------------



//-------------------------------------------
// Declaración de variables
//-------------------------------------------
long tickRegulacion = 0;
/* // -------------------------------------------------------------------
    TEMP_AMB = 0;      // valor de la temperatura ambiente
    HUM_AMB = 0;       // valor de la humedad ambiente
    HUM_SUE = 0;       // valor del sensor 1 de humedad del suelo
    HUM_SUE2 = 0;      // valor del sensor 2 de humedad del suelo
    HUM_SUE3 = 0;      // valor del sensor 3 de humedad del suelo
    HUM_SUE4 = 0;      // valor del sensor 4 de humedad del suelo
    TEMP_AMB_OFFS = 0; // offset para correción temperatura ambiente
    HUM_AMB_OFFS = 0;  // offset para correción humedad ambiente
    HUM_SUE_OFFS = 0;  // offset para la corrección del valor del sensor 1 de la humedad el suelo
    HUM_SUE2_OFFS = 0; // offset para la corrección del valor del sensor 2 de la humedad el suelo
    HUM_SUE3_OFFS = 0; // offset para la corrección del valor del sensor 3 de la humedad el suelo
    HUM_SUE4_OFFS = 0; // offset para la corrección del valor del sensor 4 de la humedad el suelo
    // ------------------------------------------------------------------------
    // Valores ajustes regulación settings.json
    //-------------------------------------------------------------------------
    TEMP_CONEXION = 28;       // valor de temperatura para apertura trampilla
    TEMP_DESCONEXION = 20;    // valor de temperatura para cierre de la trampilla
    HUMEDAD_CONEXION = 30;    // valor humedad para conexión electroválvula
    HUMEDAD_DESCONEXION = 80; // valor humedad para conexión electroválvula
*/

//-------------------------------------------------
// Control de actuadores para bajar la temperatura
//-------------------------------------------------
void controlActuadores(){
    if (TEMP_AMB > TEMP_CONEXION){
        setOffSingle(ACT_2);
        setOnSingle(ACT_1);
    }
     if (TEMP_AMB < TEMP_DESCONEXION){
        setOffSingle(ACT_1);
        setOnSingle(ACT_2);
    }
}
//-------------------------------------------------
// Control de electroválvulas según la humedad
//-------------------------------------------------
void controlElectrovavulas(){
    if (HUM_SUE < HUMEDAD_CONEXION){
        setOnSingle(SAL_1);
    }
    if (HUM_SUE > HUMEDAD_DESCONEXION  || HUM_SUE == 100){
        setOffSingle(SAL_1);
    }
     if (HUM_SUE2 < HUMEDAD_CONEXION){
        setOnSingle(SAL_2);
    }
    if (HUM_SUE2> HUMEDAD_DESCONEXION || HUM_SUE2 == 100){
        setOffSingle(SAL_2);
    }
     if (HUM_SUE3 < HUMEDAD_CONEXION){
        setOnSingle(SAL_3);
    }
    if (HUM_SUE3 > HUMEDAD_DESCONEXION || HUM_SUE3 == 100){
        setOffSingle(SAL_3);
    }
     if (HUM_SUE4 < HUMEDAD_CONEXION){
        setOnSingle(SAL_4);
    }
    if (HUM_SUE4 > HUMEDAD_DESCONEXION || HUM_SUE4 == 100){
        setOffSingle(SAL_4);
    }
    
}


   


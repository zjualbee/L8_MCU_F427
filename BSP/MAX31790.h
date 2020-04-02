


#ifndef __MAX31790_H__
#define __MAX31790_H__
/* Define to prevent recursive inclusion -------------------------------------*/

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"



/* Includes ------------------------------------------------------------------*/
//#include "main.h"



typedef uint8_t (*Max_Bsp_Read)(uint8_t dev_addr , uint8_t reg);
typedef uint8_t (*Max_Bsp_Write)(uint8_t dev_addr , uint8_t reg , uint8_t value);


typedef struct _MAX31790_OBJ
{
    uint8_t  dev_addr; 
//////////////do command/////////////

     Max_Bsp_Read  iic_read;  
     Max_Bsp_Write iic_write; 
}MAX31790_OBJ,*pMAX31790_OBJ;





int Max31790_Pwm_Set(pMAX31790_OBJ pObj,uint8_t id, uint16_t pwm);

int Max31790_Rpm_Get(pMAX31790_OBJ pObj,uint8_t id,uint16_t *pRpm);



int Max31790_On(pMAX31790_OBJ pObj);

int Max31790_Full_Speed(pMAX31790_OBJ pObj);

int Max31790_Off(pMAX31790_OBJ pObj);



int Max31790_Init(pMAX31790_OBJ pObj,uint8_t dev_addr,Max_Bsp_Read  iic_read,Max_Bsp_Write iic_write);
void Max31790_List_Reg(pMAX31790_OBJ pObj);


#endif




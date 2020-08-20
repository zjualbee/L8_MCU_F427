#ifndef __MAX31790_H__
#define __MAX31790_H__
/* Define to prevent recursive inclusion -------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "main.h"


#define FAN_NUM     6
extern uint16_t init_pwm;


typedef uint8_t (*Max_Bsp_Read)(uint8_t dev_addr , uint8_t reg);
typedef uint8_t (*Max_Bsp_Write)(uint8_t dev_addr , uint8_t reg , uint8_t value);


typedef struct _MAX31790_OBJ
{
    uint8_t  dev_addr; 


    uint16_t pwm_value[FAN_NUM];  //定义为最大为100%的占空比数值
    uint16_t rpm_value[FAN_NUM];  //定义为风扇转速

//////////////do command/////////////

     Max_Bsp_Read  iic_read;  
     Max_Bsp_Write iic_write; 
}MAX31790_OBJ,*pMAX31790_OBJ;





int Max31790_Pwm_Set(pMAX31790_OBJ pObj,uint8_t id, uint16_t pwm);

int Max31790_Rpm_Get(pMAX31790_OBJ pObj,uint8_t id,uint16_t *pRpm);



int Max31790_On(pMAX31790_OBJ pObj, uint16_t pwm_value);

int Max31790_Full_Speed(pMAX31790_OBJ pObj);

int Max31790_Off(pMAX31790_OBJ pObj);



int Max31790_Init(pMAX31790_OBJ pObj,uint8_t dev_addr,Max_Bsp_Read  iic_read,Max_Bsp_Write iic_write, uint16_t pwm);
void Max31790_List_Reg(pMAX31790_OBJ pObj);
int Max31790_Pwm_Set_All(pMAX31790_OBJ pObj, uint16_t pwm);





int Max31790_Update(pMAX31790_OBJ p);

void print_max31790_fan_rpm(pMAX31790_OBJ p);


#endif




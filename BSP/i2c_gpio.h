/******************** (C) COPYRIGHT 2014 APPOTRONICS ***************************
* File Name          : i2c_gpio.h
* Author             : WWZ
* Version            : V1.0.0
* Date               : 2014-10-11
* Description        : Header for i2c_gpio.c file. 
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, APPOTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_GPIO_H
#define __I2C_GPIO_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"

/* Exported types ------------------------------------------------------------*/

typedef struct I2cGpio
{
    int (*scl_0)(void);         // 私有接口,0正常，非0异常
    int (*scl_1)(void);         // 私有接口,0正常，非0异常
    int (*scl_read)(void);      // 私有接口,0低电平,1高电平
    int (*sda_0)(void);         // 私有接口,0正常，非0异常
    int (*sda_1)(void);         // 私有接口,0正常，非0异常
    int (*sda_read)(void);      // 私有接口,0低电平,1高电平
    void (*delay)(void);        // 私有接口,操作GPIO后的延时,影响模拟i2c的通讯速率
    int (*mutex_take)(void);    // 私有接口,获取信号量
    int (*mutex_give)(void);    // 私有接口,释放信号量

    int (*start)(struct I2cGpio *thiz);                     // 私有接口,线程不安全接口
    int (*stop)(struct I2cGpio *thiz);                      // 私有接口,线程不安全接口
    int (*ack)(struct I2cGpio *thiz);                       // 私有接口,线程不安全接口
    int (*nack)(struct I2cGpio *thiz);                      // 私有接口,线程不安全接口
    int (*wait_ack)(struct I2cGpio *thiz);                  // 私有接口,线程不安全接口
    int (*send_byte)(struct I2cGpio *thiz, uint8_t byte);   // 私有接口,线程不安全接口
    uint8_t (*read_byte)(struct I2cGpio *thiz);             // 私有接口,线程不安全接口

    int (*reset)(struct I2cGpio *thiz);
    int (*transmit)(struct I2cGpio *thiz, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    int (*receive)(struct I2cGpio *thiz, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    int (*mem_write)(struct I2cGpio *thiz, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
    int (*mem_read)(struct I2cGpio *thiz, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout);
}struct_I2cGpio;

/* Exported constants --------------------------------------------------------*/

#define I2C_GPIO_MEMADDR_SIZE_8BIT  (1)
#define I2C_GPIO_MEMADDR_SIZE_16BIT (2)

/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
/* Exported functions --------------------------------------------------------*/

int i2c_gpio_init(struct_I2cGpio *thiz, 
                  int (*scl_0)(void), 
                  int (*scl_1)(void), 
                  int (*scl_read)(void), 
                  int (*sda_0)(void), 
                  int (*sda_1)(void), 
                  int (*sda_read)(void),
                  void (*delay)(void),
                  int (*mutex_take)(void), 
                  int (*mutex_give)(void));
    
#endif
/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/

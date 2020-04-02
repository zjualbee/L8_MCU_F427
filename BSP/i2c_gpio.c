
/* Includes ------------------------------------------------------------------*/
#include "i2c_gpio.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : i2c_gpio_start
* Description    : 产生IIC起始信号
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int i2c_gpio_start(struct I2cGpio *thiz)
{
    thiz->sda_1();
    thiz->scl_1();
    thiz->delay();
    if(!thiz->sda_read())return 1;	//SDA线为低电平则总线忙,退出
    thiz->sda_0();
    thiz->delay();
    if(thiz->sda_read()) return 1;	//SDA线为高电平则总线出错,退出
    thiz->sda_0();
    thiz->delay();
    return 0;
}	

/*******************************************************************************
* Function Name  : i2c_gpio_stop
* Description    : 产生IIC停止信号
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int i2c_gpio_stop(struct I2cGpio *thiz)
{
	thiz->scl_0();
    thiz->delay();
    thiz->sda_0();
    thiz->delay();
    thiz->scl_1();
    thiz->delay();
    thiz->sda_1();
    thiz->delay();
    return 0;
}

/*******************************************************************************
* Function Name  : i2c_gpio_ack
* Description    : 产生ACK应答
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int i2c_gpio_ack(struct I2cGpio *thiz)
{
    thiz->scl_0();
    thiz->delay();
    thiz->sda_0();
    thiz->delay();
    thiz->scl_1();
    thiz->delay();
    thiz->scl_0();
    thiz->delay();
    return 0;
}

/*******************************************************************************
* Function Name  : i2c_gpio_nack
* Description    : 不产生ACK应答
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/		    
static int i2c_gpio_nack(struct I2cGpio *thiz)
{
    thiz->scl_0();
    thiz->delay();
    thiz->sda_1();
    thiz->delay();
    thiz->scl_1();
    thiz->delay();
    thiz->scl_0();
    thiz->delay();
    return 0;
}

/*******************************************************************************
* Function Name  : i2c_gpio_wait_ack
* Description    : 等待应答信号到来
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int i2c_gpio_wait_ack(struct I2cGpio *thiz)
{
    thiz->scl_0();
    thiz->delay();
    thiz->sda_1();
    thiz->delay();
    thiz->scl_1();
    thiz->delay();
    if(thiz->sda_read()){
        thiz->scl_0();
        return 1;
    }
    thiz->scl_0();  
	return 0;  
}

/*******************************************************************************
* Function Name  : i2c_gpio_send_byte
* Description    : IIC发送一个字节
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/		    
static int i2c_gpio_send_byte(struct I2cGpio *thiz, uint8_t byte)
{                        
    uint8_t i = 8;
    
    while(i--)
    {
        thiz->scl_0(); 
        thiz->delay();
        if (byte & 0x80)
            thiz->sda_1();  
        else 
            thiz->sda_0();   
        byte <<= 1;
        thiz->delay();
        thiz->scl_1();
        thiz->delay();
    }
    thiz->scl_0();
    return 0;
}

/*******************************************************************************
* Function Name  : i2c_gpio_read_byte
* Description    : 读1个字节
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/	
static uint8_t i2c_gpio_read_byte(struct I2cGpio *thiz)
{
    uint8_t i = 8;
    uint8_t byte = 0;

    thiz->sda_1();				
    while(i--)
    {
        byte <<= 1;      
        thiz->scl_0();
        thiz->delay();
        thiz->scl_1();
        thiz->delay();	
        if(thiz->sda_read())
            byte |= 0x01;
    }
    thiz->scl_0();
    return byte;
}

/*******************************************************************************
* Function Name  : i2c_gpio_reset
* Description    : reset
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int i2c_gpio_reset(struct I2cGpio *thiz)
{
    thiz->mutex_take();
    // thiz->start(thiz);  // start
    thiz->stop(thiz);   // stop
    thiz->mutex_give();
    return 0;
}

/*******************************************************************************
* Function Name  : i2c_gpio_transmit
* Description    : transmit
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int i2c_gpio_transmit(struct I2cGpio *thiz, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    thiz->mutex_take();
    // start
    if(thiz->start(thiz))
        goto ERR_i2c_gpio_transmit;

    // DeviceAddress
    thiz->send_byte(thiz, DevAddress & 0xFE);
    if(thiz->wait_ack(thiz)){
        thiz->stop(thiz); 
        goto ERR_i2c_gpio_transmit;
    }

    // data
    while(Size--)
    {
      thiz->send_byte(thiz, *pData);
      thiz->wait_ack(thiz);
      pData++;
    }

    // stop
    thiz->stop(thiz); 
    thiz->mutex_give();
    return 0;

ERR_i2c_gpio_transmit:
    thiz->mutex_give();
    return 1;
}

/*******************************************************************************
* Function Name  : i2c_gpio_receive
* Description    : receive
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
static int i2c_gpio_receive(struct I2cGpio *thiz, uint16_t DevAddress, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    thiz->mutex_take();
    // start
    if(thiz->start(thiz))
        goto ERR_i2c_gpio_receive;

    // DeviceAddress Write
    thiz->send_byte(thiz, DevAddress | 0x01);
    if(thiz->wait_ack(thiz)){
        thiz->stop(thiz); 
        goto ERR_i2c_gpio_receive;
    }

    // data
    while(Size){
        *pData = thiz->read_byte(thiz);
        if (Size == 1)
            thiz->nack(thiz);
        else 
            thiz->ack(thiz);
        pData++;
        Size--;
    }

    // stop
    thiz->stop(thiz);
    thiz->mutex_give();
    return 0;

ERR_i2c_gpio_receive:
    thiz->mutex_give();
    return 1;
}

/*******************************************************************************
* Function Name  : i2c_gpio_mem_write
* Description    : mem write
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/	
static int i2c_gpio_mem_write(struct I2cGpio *thiz, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    thiz->mutex_take();
    // start
    if(thiz->start(thiz))
        goto ERR_i2c_gpio_mem_write;

    // DeviceAddress
    thiz->send_byte(thiz, DevAddress & 0xFE);
    if(thiz->wait_ack(thiz)){
        thiz->stop(thiz); 
        goto ERR_i2c_gpio_mem_write;
    }
    
    // MemAddress
    if (I2C_GPIO_MEMADDR_SIZE_8BIT == MemAddSize){
        thiz->send_byte(thiz, MemAddress & 0xFF);
        thiz->wait_ack(thiz);
    }
    else{
        thiz->send_byte(thiz, (MemAddress>>8) & 0xFF);
        thiz->wait_ack(thiz);
        thiz->send_byte(thiz, MemAddress & 0xFF);
        thiz->wait_ack(thiz);
    }

    // data
    while(Size--)
    {
      thiz->send_byte(thiz, *pData);
      thiz->wait_ack(thiz);
      pData++;
    }

    // stop
    thiz->stop(thiz);
    thiz->mutex_give();
    return 0;

ERR_i2c_gpio_mem_write:
    thiz->mutex_give();
    return 1;
}

/*******************************************************************************
* Function Name  : i2c_gpio_mem_read
* Description    : mem read
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/	
static int i2c_gpio_mem_read(struct I2cGpio *thiz, uint16_t DevAddress, uint16_t MemAddress, uint16_t MemAddSize, uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
    thiz->mutex_take();
    // start
    if(thiz->start(thiz))
        goto ERR_i2c_gpio_mem_read;

    // DeviceAddress Write
    thiz->send_byte(thiz, DevAddress & 0xFE);
    if(thiz->wait_ack(thiz)){
        thiz->stop(thiz); 
        goto ERR_i2c_gpio_mem_read;
    }

    // MemAddress
    if (I2C_GPIO_MEMADDR_SIZE_8BIT == MemAddSize){
        thiz->send_byte(thiz, MemAddress & 0xFF);
        thiz->wait_ack(thiz);
    }
    else{
        thiz->send_byte(thiz, (MemAddress>>8) & 0xFF);
        thiz->wait_ack(thiz);
        thiz->send_byte(thiz, MemAddress & 0xFF);
        thiz->wait_ack(thiz);
    }

    // start
    thiz->start(thiz);

    // DeviceAddress Read
    thiz->send_byte(thiz, DevAddress | 0x01);
    thiz->wait_ack(thiz);

    // data
    while(Size){
        *pData = thiz->read_byte(thiz);
        if (Size == 1)
            thiz->nack(thiz);
        else 
            thiz->ack(thiz);
        pData++;
        Size--;
    }

    // stop
    thiz->stop(thiz);
    thiz->mutex_give();
    return 0;

ERR_i2c_gpio_mem_read:
    thiz->mutex_give();
    return 1;
}

/*******************************************************************************
* Function Name  : i2c_gpio_init
* Description    : init
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int i2c_gpio_init(struct_I2cGpio *thiz, 
                    int (*scl_0)(void), int (*scl_1)(void), int (*scl_read)(void), 
                    int (*sda_0)(void), int (*sda_1)(void), int (*sda_read)(void), 
                    void (*delay)(void), 
                    int (*mutex_take)(void), int (*mutex_give)(void))
{
    memset(thiz, 0, sizeof(struct_I2cGpio));
    thiz->scl_0         = scl_0;
    thiz->scl_1         = scl_1;
    thiz->scl_read      = scl_read;
    thiz->sda_0         = sda_0;
    thiz->sda_1         = sda_1;
    thiz->sda_read      = sda_read;
    thiz->delay         = delay;
    thiz->mutex_take    = mutex_take;
    thiz->mutex_give    = mutex_give;

    thiz->start     = i2c_gpio_start;
    thiz->stop      = i2c_gpio_stop;
    thiz->ack       = i2c_gpio_ack;
    thiz->nack      = i2c_gpio_nack;
    thiz->wait_ack  = i2c_gpio_wait_ack;
    thiz->send_byte = i2c_gpio_send_byte;
    thiz->read_byte = i2c_gpio_read_byte;

    thiz->reset     = i2c_gpio_reset;
    thiz->transmit  = i2c_gpio_transmit;
    thiz->receive   = i2c_gpio_receive;
    thiz->mem_write = i2c_gpio_mem_write;
    thiz->mem_read  = i2c_gpio_mem_read;
    return 0;
}

/******************* (C) COPYRIGHT 2014 APPOTRONICS ************END OF FILE****/

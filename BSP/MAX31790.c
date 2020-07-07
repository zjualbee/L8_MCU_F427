#include "MAX31790.h"

#define MAX_PWM 100
uint16_t init_pwm=50;
extern uint8_t temp_ce;


int Max31790_Pwm_Set(pMAX31790_OBJ pObj,uint8_t id, uint16_t duty)
{

    // bits 15:7 reg            PWM     
    //0x000                            0%
    //0x0C8                           39%
    //0x12C                           59%
    //0x190                           78%
    //0x1E0                           94%
    //0x1FF                            100%
    uint32_t pwmout = 0;
	
    if(id >= 6)return 0;
    if(duty>MAX_PWM)
		duty=MAX_PWM;

    pObj->pwm_value[id]=duty;
    printf("Pwm_Set %d\r\n",duty);
    
    pwmout = 511 * duty /100;
    pObj->iic_write(pObj->dev_addr,0x40+(id*2), (pwmout >> 1) & 0xFF);
    pObj->iic_write(pObj->dev_addr,0x41+(id*2), (pwmout & 0x01) << 7 & 0xFF);
    return 0;
}



int Max31790_Pwm_Set_All(pMAX31790_OBJ pObj, uint16_t pwm)
{
    int i=0;
    for(i=0; i<6; i++)
        {
            Max31790_Pwm_Set(pObj,i,pwm);
        }
}


/*******************************************************************************
* Function Name  : MAX31790_rpm_get
* Description    : speed_get
* Input          : id: 0~5 for fan1~fan6
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/




static int Max31790_Rpm_Get(pMAX31790_OBJ p,uint8_t id,uint16_t *pRpm)
{
    uint16_t recv[2] = {0};
    uint16_t tach = 0;
    //bits 15:5 reg  PWM
    if(id >= FAN_NUM)return 0;
    recv[0] = p->iic_read(p->dev_addr,(0x18+id*2));
    recv[1] = p->iic_read(p->dev_addr,(0x19+id*2));
    tach = (recv[0] << 3) | (recv[1] >> 5);
    *pRpm = (60 * 4 * 8192) / tach / 2;
	// printf("ID %d FAN %d\r\n",id,*pRpm);
	//printf("11:%x  22:%x  23:%x 51:%x  5b:%x  07:%x \r\n",MA_ReadOneByte(0x11,DRIVE_ADRR1),MA_ReadOneByte(0x22,DRIVE_ADRR1),MA_ReadOneByte(0x23,DRIVE_ADRR1),MA_ReadOneByte(0x5a,DRIVE_ADRR1),MA_ReadOneByte(0x5b,DRIVE_ADRR1),MA_ReadOneByte(0x07,DRIVE_ADRR1));
    return 1;
}



static int Max31790_Rpm_Get_All(pMAX31790_OBJ p)
{
    int i=0;
    for(i = 0; i < FAN_NUM ; i++)
    {
        Max31790_Rpm_Get(p,i,&(p->rpm_value[i]));
    }
    return FAN_NUM;
}


int Max31790_Update(pMAX31790_OBJ p)
{
    Max31790_Rpm_Get_All(p);
    return 0;
}


void print_max31790_fan_rpm(pMAX31790_OBJ p)
{
    int i=0;
    for(i=0 ; i<FAN_NUM ; i++)
    {   
        printf("rpm%d:%d\r\n",i,p->rpm_value[i]);
    }
}




/*******************************************************************************
* Function Name  : MAX31790_on
* Description    : on
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int Max31790_On(pMAX31790_OBJ pObj, uint16_t pwm_value)
{

    int i = 0;
    uint8_t window  = 0;
    for(i=0; i<6; i++)
    {
        pObj->iic_write(pObj->dev_addr,0x60+i, window);
    }

    for(i=0 ; i < 6; i++)
    {
    	Max31790_Pwm_Set(pObj, i,pwm_value);
    }

    return 0;
		
}

/*******************************************************************************
* Function Name  : MAX31790_on_full_speed
* Description    : full speed
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int Max31790_Full_Speed(pMAX31790_OBJ pObj)
{
    int i = 0;
    uint16_t pwmout = 0;
    
    pwmout = 511;   // 全速
    for (i = 0; i < FAN_NUM; i++){
        pObj->iic_write(pObj->dev_addr,0x40+(i*2), (pwmout >> 1) & 0xFF);
        pObj->iic_write(pObj->dev_addr,0x41+(i*2), (pwmout & 0x01) << 7);
    }
    //FULL_SPEED_RESET;     // 全速
    return 0;
}

/*******************************************************************************
* Function Name  : MAX31790_off
* Description    : off
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/
int Max31790_Off(pMAX31790_OBJ pObj)
{
    int i = 0;
    uint16_t pwmout = 0;

    // 设置目标PWM
    pwmout = 0;
    for (i = 0; i < FAN_NUM; i++)
        {
        pObj->iic_write(pObj->dev_addr,0x40+(i*2), (pwmout >> 1) & 0xFF);
        pObj->iic_write(pObj->dev_addr,0x41+(i*2), (pwmout & 0x01) << 7);
    }
    //FULL_SPEED_SET;     // 关全速
    
    return 0;
}

/*******************************************************************************
* Function Name  : MAX31790_init
* Description    : init
* Input          : None
* Output         : None
* Return         : 0正常，非0异常
*******************************************************************************/


int Max31790_Init(pMAX31790_OBJ pObj,uint8_t dev_addr,Max_Bsp_Read  iic_read,Max_Bsp_Write iic_write)
{
	uint16_t pwmout = 0;
	pObj->dev_addr = dev_addr;
	pObj->iic_read = iic_read;
	pObj->iic_write = iic_write;

	printf("init31790 %02X\r\n",dev_addr);
	//reset
	//FULL_SPEED1_SET;
	pObj->iic_write(pObj->dev_addr,0x00, 0x60);    // RESET
	HAL_Delay(20);                       // > 1ms
	pObj->iic_write(pObj->dev_addr,0x00, 0x20);
	pObj->iic_write(pObj->dev_addr,0x01, 0xBB);    // PWM Hz



	// fan config
	pObj->iic_write(pObj->dev_addr,0x02, 0x08);
	pObj->iic_write(pObj->dev_addr,0x03, 0x08);
	pObj->iic_write(pObj->dev_addr,0x04, 0x08);
	pObj->iic_write(pObj->dev_addr,0x05, 0x08);
	pObj->iic_write(pObj->dev_addr,0x06, 0x08);
	pObj->iic_write(pObj->dev_addr,0x07, 0x08);

	// fan Dynamics 设置动态
	pObj->iic_write(pObj->dev_addr,0x08, 0x40);//PWMOUT1
	pObj->iic_write(pObj->dev_addr,0x09, 0x40);//PWMOUT2
	pObj->iic_write(pObj->dev_addr,0x0a, 0x40);//PWMOUT3
	pObj->iic_write(pObj->dev_addr,0x0b, 0x40);//PWMOUT4
	pObj->iic_write(pObj->dev_addr,0x0c, 0x40);//PWMOUT5
	pObj->iic_write(pObj->dev_addr,0x0D, 0x40);//PWMOUT6


	// PWMOUT 0~511
	pwmout = init_pwm;
	int i=0;
	for(i=0; i<6; i++)
	{
		pObj->iic_write(pObj->dev_addr,0x40+(2*i), (pwmout >> 1) & 0xFF);
		pObj->iic_write(pObj->dev_addr,0x41+(2*i), (pwmout & 0x01) << 7);
	}

	//printf("init  fan ok\r\n");
	return 1;
}


void Max31790_List_Reg(pMAX31790_OBJ pObj)
{
    int i=0;

    uint8_t temp=0;

    printf("==========%02X=========",pObj->dev_addr);
    for(i=0;i<128;i++)
        {
            if(i%8==0)printf("\r\n%02X:",i);
            temp = pObj->iic_read(pObj->dev_addr,i);
            printf("%02X ",temp);
            
        }
    printf("\r\n");
}


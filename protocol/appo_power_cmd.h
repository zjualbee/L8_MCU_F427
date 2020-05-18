






#ifndef __APPO_POWER_CMD_H__
#define __APPO_POWER_CMD_H__



#define APPO_POWER_CMD_ON_OFF  0X22


#pragma pack(1)

/////////////////////////////////////////////////////////


typedef struct _APPO_POWER_CMD_HEAD
{
    uint8_t  dev_id;
    uint8_t  cmd;
}APPO_POWER_CMD_HEAD,*pAPPO_POWER_CMD_HEAD;


//////////////////////////////////////////////////////////


typedef struct _APPO_POWER_PARAM_ON_OFF
{
    uint8_t  on_off_flag;   //  0 off     1  on
    uint16_t current_r;
    uint16_t current_g;
    uint16_t current_b;
}APPO_POWER_PARAM_ON_OFF,*pAPPO_POWER_PARAM_ON_OFF;


typedef struct _APPO_POWER_ON_OFF
{
    uint8_t  dev_id;
    uint8_t  cmd;
    union param
    {
        uint8_t  param_buf[sizeof(APPO_POWER_PARAM_ON_OFF)];
        APPO_POWER_PARAM_ON_OFF power_on_off;
    }Param;

}APPO_POWER_ON_OFF,*pAPPO_POWER_ON_OFF;




//////////////////////////////////////////////////////////////








#pragma pack()







#endif




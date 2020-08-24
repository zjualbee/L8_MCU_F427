





/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __V_DEBUG_H__
#define __V_DEBUG_H__

/* Includes ------------------------------------------------------------------*/
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"

#include "main.h"
#include "cmsis_os.h"



#define VDEBUG_BUF_LEN  50


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/

void bubble_sort(int *pData, unsigned int Count);

void print_buf(uint8_t* buf,int len);

void print_buf_c(uint8_t* buf,int len);







#endif






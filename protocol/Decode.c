#include "Decode.h"
#include "main.h"
DECODE_TABLE Decode_PMU;
DECODE_TABLE Decode_DLP_P3;
DECODE_TABLE Decode_DLP_P4;
DECODE_TABLE Decode_DLP_P6;
DECODE_TABLE Decode_PC;


//---------------------------------------------------------------------------------------------------
#define PRINT_DECODE   printf

/************************************************************
@info: for appo2.0 protocol encode
@function name:appoProtocolEncode
@params:
  cmd:      command
  indata:   buffer for input data
  len:      data length input
  flag:     flag 
  seria:    seria number for message check
  outData:  buffer after encode, NULL if no need
  fnSend:   outdata callbak task, NULL if no need
@return:
  int:      data length after encode, 0 is error
************************************************************/

unsigned char Make_5AA5_Sum_Ext(unsigned char start_value,unsigned char * buf ,int len)
{
	uint16_t sum=0;
	int i=0;

    sum = start_value;
	for(i=0;i<len;i++)
	{
		sum = buf[i] ^ sum;
	}
      
	 //printf("\r\nsum:%x \r\n",sum);
	return sum&0xFF;
}


uint8_t Check_Xor_5AA5(uint8_t * buf ,int len)
{
	int sum=0;
	int i=0;
	
	for(i=0;i<len;i++)
	{
		sum = buf[i] ^ sum;
	}
	return sum&0xFF;
}

uint32_t ppfifo_in(pDECODE_TABLE decode_table ,uint8_t *buf ,uint32_t len)
{
    unsigned int i = 0;
    
    if((decode_table == 0)||(buf == 0)||(len == 0))
        {
            return 0;
        }
    
    if(decode_table->switch_channel == RECV_CHANNEL_A)
    {
        if( (decode_table->index_a + len) > CMD_SIZE)
        {
            return 0;
        }
        for(i = 0; i < len; i ++)
        {
            decode_table->buf_channel_a[decode_table->index_a] = buf[i];
            
            decode_table->index_a++;
        }
    }
    else
    {
        if( (decode_table->index_b + len) > CMD_SIZE)
           {
               return 0;
           }

        
        for(i = 0; i < len; i ++)
        {
            decode_table->buf_channel_b[decode_table->index_b] = buf[i];
            
            decode_table->index_b++;
        }
    }
    
    return len;
}


void Decode_Handle(pDECODE_TABLE decode_table)
{

    // int i=0;
    static uint16_t pre_index=0;
    if(decode_table->switch_channel == RECV_CHANNEL_B)
        {

        for( pre_index=0;  pre_index < decode_table->index_a ; pre_index++ )
            { 
       //         PRINT_DECODE("A pre:%d / %d\r\n",pre_index,decode_table->index_a);
                if(0!=CMD_5AA5_Decode(decode_table,decode_table->buf_channel_a[pre_index]))
                    {
                         pre_index++;
                         return ;
                    }
            }
        
            pre_index = 0;
            decode_table->index_a = 0;
            decode_table->switch_channel = RECV_CHANNEL_A;

        }
    
    else //if(decode_table->switch_channel == RECV_CHANNEL_A)
        {

        for(pre_index=0 ; pre_index < decode_table->index_b;pre_index++ )
            {
                
            //    PRINT_DECODE("B pre:%d / %d\r\n",pre_index,decode_table->index_b);
                if(0!=CMD_5AA5_Decode(decode_table,decode_table->buf_channel_b[pre_index]))
                {
                    pre_index++;
                    return ;
                } 
            }
            pre_index = 0;
            decode_table->index_b = 0;
            decode_table->switch_channel = RECV_CHANNEL_B;

        }

}



void for_delay(int x)
{
    int m;
    int n;
    for(m=0;m<x;m++)
        {
        for(n=0;n<100;n++);
        }
}

uint32_t CMD_5AA5_Decode(pDECODE_TABLE decode_table, uint8_t chr)
{           
    switch(decode_table->part)
    {
        case 0 :    // ??
            {
                if((chr == 0x5A) || (chr == 0xA5))
                {
                    decode_table->package_buf[decode_table->recv_total] = chr;
                    decode_table->recv_total++;
                }             
                else
                {
                    decode_table->part = 0;
                    decode_table->recv_total = 0;                  
                    return 0;
                }

		  		if(decode_table->recv_total<2)
					return 0;
				
                if((decode_table->package_buf[decode_table->recv_total-2]==0x5A)  && (decode_table->package_buf[decode_table->recv_total-1]==0xA5))
                {
                    decode_table->part = 1;
                    decode_table->package_buf[0] = 0x5A;
                    decode_table->package_buf[1] = 0xA5;
                    decode_table->recv_total = 2;
                    return 0;
                }
            }
            break;
			
		case 1 :	// ???? ???;
		{
		 decode_table->package_buf[decode_table->recv_total++] = chr;
         if(decode_table->recv_total >= 4)
            {
                 decode_table->packet_len = ((decode_table->package_buf[2]<<8)|decode_table->package_buf[3]);
             	//    PRINT_DECODE("packet_len %d \r\n",decode_table->packet_len);
                 if(decode_table->packet_len > CMD_SIZE)
                    {
                   //     printf("big than max cmd\r\n");
                        decode_table->part = 0;
                        decode_table->recv_total = 0;
                    }
                 else
                    {
                        decode_table->part = 2;

                    }
            }
		break;	 
		}
				 
        case 2:// ???
            { 
                decode_table->package_buf[decode_table->recv_total++] = chr;
                
               // PRINT_DECODE("data in %x %x\r\n",decode_table->packet_len,decode_table->recv_total);
	//	PRINT_DECODE("data in (%x)\r\n",chr);
            
                if(decode_table->recv_total >= decode_table->packet_len-1)  // ????,???????
                {   
					decode_table->part = 3;
				//	PRINT_DECODE("data ok \r\n");
                    return 0;
                }

               
				break;
                
            }
        case 3 : // ???;
            { 
                decode_table->package_buf[decode_table->recv_total] = chr;

				decode_table->check_sum = Check_Xor_5AA5(decode_table->package_buf,decode_table->recv_total);

             //   PRINT_DECODE("check_sum:%x \r\n",decode_table->check_sum);
                
                if((decode_table->package_buf[decode_table->recv_total]==0x00)||\
                   (decode_table->package_buf[decode_table->recv_total]==decode_table->check_sum)) // ????,???????
                {
                ;
			//	PRINT_DECODE("check ok len :%d \r\n",decode_table->recv_total);

    				{
    				decode_table->cmd_flag = 1;
    				decode_table->cmd_len = decode_table->packet_len;
    				memcpy(decode_table->cmd_buf, &decode_table->package_buf[0],decode_table->packet_len);
    			//	PRINT_DECODE("decode ok to struct\r\n");
    				}



					// ?????
					memset(decode_table->package_buf,0,decode_table->recv_total); 
					decode_table->part = 0;
					decode_table->recv_total = 0;
					return decode_table->packet_len;

			}
			else
			{
        memset(decode_table->package_buf,0,decode_table->recv_total); 
				decode_table->part = 0;
				decode_table->recv_total = 0;
			//	PRINT_DECODE("check sum err\r\n");
				return 0;
			}		
  			break;
                }
		
        default:
			{
		//	PRINT_DECODE("default clean\r\n");
			decode_table->part = 0;
			decode_table->recv_total = 0;
			decode_table->packet_len = 0;
            break;  
        	}
    } 
    
    return 0;
}





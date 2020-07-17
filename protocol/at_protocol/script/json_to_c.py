import os
import sys

import json



#JSON_FILE_NAME   = "./at_cmd.json"
JSON_FILE_NAME   = "./json_out.json"

OUTPUT_FILE_NAME = "../at_cmd"



AT_CMD_HEAD_C = r"""

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "at_cmd.h"


#define SIZE_KEY_CNT(X)     sizeof(X)/sizeof(PARAM_KEY_ELEMENT)
#define SIZE_VALUE_CNT(X)   sizeof(X)/sizeof(PARAM_VALUE_ELEMENT)


"""


AT_CMD_BODY_C = """
/////////////////////VERSION    START //////////////////////////////////

PARAM_KEY_ELEMENT version_key_string_list[]=
{
    {DP_VERSION_DLPSOFTWARE,VALUE_CLASS_STRING,0,0,"VERSION_DLPSOFTWARE"},
    {DP_VERSION_DLPHARDWARE,VALUE_CLASS_STRING,0,0,"VERSION_DLPHARDWARE"},
    {DP_VERSION_PMUSOFTWARE,VALUE_CLASS_STRING,0,0,"VERSION_PMUSOFTWARE"},
    {DP_VERSION_PMUHARDWARE,VALUE_CLASS_STRING,0,0,"VERSION_PMUHARDWARE"},
    {DP_VERSION_IDUSOFTWARE,VALUE_CLASS_STRING,0,0,"VERSION_IDUSOFTWARE"},
    {DP_VERSION_IDUHARDWARE,VALUE_CLASS_STRING,0,0,"VERSION_IDUHARDWARE"},
};
/////////////////////VERSION    END//////////////////////////////////



/////////////////////LIGHTSOURCE    START //////////////////////////////////

PARAM_VALUE_ELEMENT lightsource_status_value_string_list[]=
{
    {DPV_LIGHTSOURCE_STATUS_ON,"ON"},
    {DPV_LIGHTSOURCE_STATUS_OFF,"OFF"},
};



PARAM_KEY_ELEMENT lightsource_key_string_list[]=
{
    {DPK_LIGHTSOURCE_STATUS,VALUE_CLASS_ENUM,lightsource_status_value_string_list,SIZE_VALUE_CNT(lightsource_status_value_string_list),"STATUS"},
    {DPK_LIGHTSOURCE_STATUS,VALUE_CLASS_ENUM,lightsource_status_value_string_list,SIZE_VALUE_CNT(lightsource_status_value_string_list),"A"},
};
/////////////////////LIGHTSOURCE    END//////////////////////////////////






///////////////////// LIGHTSOURCTIME_STATUS    START //////////////////////////////////

PARAM_KEY_ELEMENT lightsourcetime_key_string_list[]=
{
    {DPK_LIGHTSOURCTIME_STATUS,VALUE_CLASS_INT,0,0,"HOURS"},
    {DPK_LIGHTSOURCTIME_STATUS,VALUE_CLASS_INT,0,0,""},
};

///////////////////// LIGHTSOURCTIME_STATUS    END//////////////////////////////////





///////////////////// FANSTATUS    START //////////////////////////////////

PARAM_KEY_ELEMENT fanstatus_key_string_list[]=
{
    {1,VALUE_CLASS_INT,0,0,"FAN1"},
    {2,VALUE_CLASS_INT,0,0,"FAN2"},
    {3,VALUE_CLASS_INT,0,0,"FAN3"},
    {4,VALUE_CLASS_INT,0,0,"FAN4"},
    {5,VALUE_CLASS_INT,0,0,"FAN5"},
    {6,VALUE_CLASS_INT,0,0,"FAN6"},
    {7,VALUE_CLASS_INT,0,0,"FAN7"},
    {8,VALUE_CLASS_INT,0,0,"FAN8"},
    {9,VALUE_CLASS_INT,0,0,"FAN9"},

};

///////////////////// FANSTATUS    END//////////////////////////////////







ELEMENT cmd_string_list[]=
{
{DC_LIGHTSOURCE,    lightsource_key_string_list,SIZE_KEY_CNT(lightsource_key_string_list),"LIGHTSOURCE"},          
{DC_VERSION,        version_key_string_list, SIZE_KEY_CNT(version_key_string_list),"VERSION"}  ,       
{DC_LIGHTSOURCETIME,lightsourcetime_key_string_list,SIZE_KEY_CNT(lightsourcetime_key_string_list), "LIGHTSOURCETIME"},
{DC_FANSTATUS,fanstatus_key_string_list,SIZE_KEY_CNT(fanstatus_key_string_list), "FANSTATUS"},


};

"""


AT_CMD_END_C = r"""



void print_cmd_string_list(void)
{

    static int i=0;
    int max_size = 0;
    
    max_size = sizeof(cmd_string_list)/sizeof(ELEMENT);
 
        for(i=0;i<max_size;i++)
        {
            printf("cmd:%s,p:%p\r\n",(char*)(cmd_string_list[i].string_buf),(char*)(cmd_string_list[i].string_buf));
        }

}




///////////////COUNT///////////////

int16_t Get_Cmd_Count(void)
{

    int16_t len=0;
    len = sizeof(cmd_string_list)/sizeof(ELEMENT);
    return len;
}


int16_t Get_Param_Key_Count(int16_t cmd_index)
{
    return  cmd_string_list[cmd_index].param_key_cnt;
}

int16_t Get_Param_Value_Count(int16_t cmd_index,int16_t key_index)
{
    return  cmd_string_list[cmd_index].param_key_list[key_index].param_value_cnt;
}



/////////////////GET  Index   from string///////////////

int16_t Get_Cmd_Index(int8_t* cmd_string)
{

    uint16_t user_cmd_len=0;
    uint16_t table_cmd_len=0;

    user_cmd_len = strlen(cmd_string);
    int i = 0;
    for( i=0 ; i < Get_Cmd_Count() ; i++)
        {

        table_cmd_len = strlen(cmd_string_list[i].string_buf);
        
        if((table_cmd_len==user_cmd_len)&&\
            (0==memcmp(cmd_string_list[i].string_buf,cmd_string,user_cmd_len)))
            {
                return i;
            }
        }

    return  -1;

}



int16_t Get_Key_Index(int16_t cmd_index,int8_t* key_string)
{

 
    uint16_t user_key_len=0;
    uint16_t table_key_len=0;
    pPARAM_KEY_ELEMENT pk=0;

    user_key_len = strlen(key_string);

    pk = cmd_string_list[cmd_index].param_key_list;

    int i = 0;
    for( i=0 ; i < Get_Param_Key_Count(cmd_index) ; i++)
        {
        
        table_key_len = strlen(pk[i].string_buf);
        
        if((table_key_len==user_key_len)&&\
            (0==memcmp(key_string,pk[i].string_buf,user_key_len)))
            {
                return i;
            }
        }

    return  -1;

}





e_VALUE_TYPE Get_Value_Type(int16_t cmd_index,int16_t key_index)
{


    return cmd_string_list[cmd_index].param_key_list[key_index].value_type;
}





int16_t Get_Enum_Value_Index(int16_t cmd_index,int16_t key_index,int8_t* value_string)
{

    uint16_t user_value_len=0;
    uint16_t table_value_len=0;
    pPARAM_VALUE_ELEMENT pv=0;

    user_value_len = strlen(value_string);


    if(user_value_len == 0)
        {
           // printf("enum index 0\r\n");
            return 0;
        }

    pv = cmd_string_list[cmd_index].param_key_list[key_index].param_value_list;

    int i = 0;
    for( i=0 ; i < Get_Param_Value_Count(cmd_index,key_index); i++)
    {

    table_value_len = strlen(pv[i].string_buf);

    if((user_value_len==table_value_len)&&\
        (0==memcmp(value_string,pv[i].string_buf,user_value_len)))
        {
            return i;
        }
    }

    //printf("value enum error\r\n");

    return  -1;

}




////////////////////GET ID  FROM  INDEX////////////////////



int16_t Get_Cmd_Id(int16_t cmd_index)
{

    return cmd_string_list[cmd_index].cmd_id;
}


int16_t Get_Key_Id(int16_t cmd_index,int16_t key_index)
{

    return cmd_string_list[cmd_index].param_key_list[key_index].key_id;
}

int16_t Get_Enum_Value_Id(int16_t cmd_index,int16_t key_index,int16_t value_index)
{

    return cmd_string_list[cmd_index].param_key_list[key_index].param_value_list[value_index].value_id;
}




/////////////////////////GET  LIST  HEAD   FROM  id  //////////////////////////////////



pPARAM_KEY_ELEMENT Get_Param_Key_Head(uint16_t cmd_index,uint16_t key_id)
{



    return 0;
}


pPARAM_VALUE_ELEMENT Get_Param_Value_Head(uint16_t cmd_index,uint16_t key_id,uint16_t value_id)
{




  
    return 0;
}



////////////////////GET INDEX  FROM  ID////////////////////

int16_t Get_Cmd_Index_By_Id(int16_t cmd_id)
{
    int i=0;
    for(i=0; i<Get_Cmd_Count(); i++)
        {
        if(cmd_string_list[i].cmd_id == cmd_id)
            {
                return i;    
            }
        }
     return -1;
}

int16_t Get_Key_Index_By_Id(int16_t cmd_index,int16_t key_id)
{
    int i=0;
    for(i=0; i<Get_Param_Key_Count(cmd_index); i++)
        {
        if(cmd_string_list[cmd_index].param_key_list[i].key_id == key_id)
            {
                return i;    
            }
        }
    return -1;
}

int16_t Get_Enum_Value_Index_By_Id(int16_t cmd_index,int16_t key_index,int16_t enum_value_id)
{
    int i=0;
    
    for(i=0; i<Get_Param_Value_Count(cmd_index,key_index); i++)
        {
        if(cmd_string_list[cmd_index].param_key_list[key_index].param_value_list[i].value_id == enum_value_id)
            {
                return i;    
            }
        }
    return -1;
}


////////////////////GET STRING  FROM  INDEX////////////

int8_t* Get_Cmd_String_By_Index(int16_t cmd_index)
{

    if(cmd_index<0)return (int8_t*)-1;

    return cmd_string_list[cmd_index].string_buf;
}

int8_t* Get_Key_String_By_Index(int16_t cmd_index,int16_t key_index)
{
    if((cmd_index<0)||(key_index<0))return (int8_t*)-1;

    return cmd_string_list[cmd_index].param_key_list[key_index].string_buf;
}

int8_t* Get_Enum_Value_String_By_Index(int16_t cmd_index,int16_t key_index,int16_t enum_value_index)
{
    if((cmd_index<0)||(key_index<0)||(enum_value_index<0))return (int8_t*)-1;

    return cmd_string_list[cmd_index].param_key_list[key_index].param_value_list[enum_value_index].string_buf;
}

//////////////////////GET STRING FROM  ID///////////////////////////

int8_t* Get_Cmd_String_By_Id(int16_t cmd_id)
{
    int16_t cmd_index=0;
    int8_t* res_string=0;
    cmd_index = Get_Cmd_Index_By_Id(cmd_id);
    res_string = Get_Cmd_String_By_Index(cmd_index);
    if(res_string!=(int8_t* )(-1))
        {
            return res_string;
        }
    else
        {
            return (int8_t* )-1;
        }
}



int8_t* Get_Key_String_By_Id(int16_t cmd_id,int16_t key_id)
{
    int16_t cmd_index=0;
    int16_t key_index=0;
    int8_t* res_string=0;
    cmd_index = Get_Cmd_Index_By_Id(cmd_id);
    key_index = Get_Key_Index_By_Id(cmd_index,key_id);
    
    res_string = Get_Key_String_By_Index(cmd_index,key_index);
    
    if(res_string!=(int8_t* )(-1))
        {
            return res_string;
        }
    else
        {
            return (int8_t* )-1;
        }
}

int8_t* Get_Enum_Value_String_By_Id(int16_t cmd_id,int16_t key_id,int16_t enum_value_id)
{
    int16_t cmd_index=0;
    int16_t key_index=0;
    int16_t enum_value_index=0;
    int8_t* res_string=0;
    cmd_index = Get_Cmd_Index_By_Id(cmd_id);
    key_index = Get_Key_Index_By_Id(cmd_index,key_id);
    enum_value_index = Get_Enum_Value_Index_By_Id(cmd_index,key_index,enum_value_id);
    res_string = Get_Enum_Value_String_By_Index(cmd_index,key_index,enum_value_index);
    
    if(res_string!=(int8_t* )(-1))
        {
            return res_string;
        }
    else
        {
            return (int8_t* )-1;
        }
}




"""








AT_CMD_H_HEAD ="""

#ifndef  __AT_CMD_H__
#define  __AT_CMD_H__

#ifdef __cplusplus 
extern "C" { 
#endif 
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h" 


#ifndef __IO
#define __IO    volatile            
#endif


#pragma pack(1)


#define PP_FIFO_SIZE  128

#define MAX_CMD_BUF_SIZE    32
#define MAX_PARAM_BUF_SIZE  32
#define MAX_PARAM_COUNT     10


typedef enum _e_OPT
{
 AT_OPERATOR_SET   =   0 ,   //  =
 AT_OPERATOR_GET   =   1  ,  //  ?
 AT_OPERATOR_REGET =   2    //#
}e_OPT;


typedef enum _e_VALUE_TYPE
{
VALUE_TYPE_ENUM = 1,
VALUE_TYPE_INT =  2,
VALUE_TYPE_STRING = 3,

}e_VALUE_TYPE;

"""

AT_CMD_H_BODY ="""



"""





AT_CMD_H_END ="""



typedef uint32_t (*CALLBACK_FUNC)(void*,uint16_t);



typedef struct _PARAM_VALUE_ELEMENT
{
    int16_t value_id;
    int8_t*  string_buf;
}PARAM_VALUE_ELEMENT,*pPARAM_VALUE_ELEMENT;

typedef struct _PARAM_KEY_ELEMENT
{
    int16_t key_id;
    e_VALUE_TYPE value_type;  // VALUE_CLASS_ENUM   VALUE_CLASS_INT  VALUE_CLASS_STRING
    pPARAM_VALUE_ELEMENT param_value_list;
    int16_t param_value_cnt;
    int8_t*  string_buf;
}PARAM_KEY_ELEMENT,*pPARAM_KEY_ELEMENT;

typedef struct _ELEMENT
{
    int16_t cmd_id;
    pPARAM_KEY_ELEMENT  param_key_list;
    int16_t param_key_cnt;
    int8_t*  string_buf;
}ELEMENT,*pELEMENT;



typedef struct _ONE_ELEMENT
{
    e_OPT  operator;  // 0:set   1:get  2:re get
    int16_t cmd_id;
    int16_t key_id;
    int8_t is_key_empty;
    e_VALUE_TYPE value_type;
    int16_t value_id;  //enum 
    int32_t value_int;  //int 
    int8_t  value_buf[MAX_PARAM_BUF_SIZE];  //string
}ONE_ELEMENT,*pONE_ELEMENT;




typedef struct _ERROR_ELEMENT
{
    e_OPT  operator;  // 0:set   1:get  2:re get
    uint16_t cmd_id;
    uint16_t key_id;
    uint16_t value_id;  //enum 
    uint16_t value_type;
    uint32_t value_int;  //int 
    int8_t  value_buf[MAX_PARAM_BUF_SIZE];  //string
}ERROR_ELEMENT,*pERROR_ELEMENT;




extern  ELEMENT  cmd_string_list[];



///////////////COUNT///////////////

int16_t Get_Cmd_Count(void);
int16_t Get_Param_Key_Count(int16_t cmd_index);

/////////////////GET  Index   from string///////////////

int16_t Get_Cmd_Index(int8_t* cmd_string);
int16_t Get_Key_Index(int16_t cmd_index,int8_t* key_string);
int16_t Get_Enum_Value_Index(int16_t cmd_index,int16_t key_index,int8_t* value_string);


////////////////////GET ID  FROM  INDEX////////////////////

e_VALUE_TYPE Get_Value_Type(int16_t cmd_index,int16_t key_index);
int16_t Get_Cmd_Id(int16_t cmd_index);
int16_t Get_Key_Id(int16_t cmd_index,int16_t key_index);
int16_t Get_Enum_Value_Id(int16_t cmd_index,int16_t key_index,int16_t value_index);



////////////////////GET INDEX  FROM  ID////////////////////

int16_t Get_Cmd_Index_By_Id(int16_t cmd_id);
int16_t Get_Key_Index_By_Id(int16_t cmd_index,int16_t key_id);
int16_t Get_Enum_Value_Index_By_Id(int16_t cmd_index,int16_t key_index,int16_t enum_value_id);

////////////////////GET STRING  FROM  INDEX///////////////

int8_t* Get_Cmd_String_By_Index(int16_t cmd_index);
int8_t* Get_Key_String_By_Index(int16_t cmd_index,int16_t key_index);
int8_t* Get_Enum_Value_String_By_Index(int16_t cmd_index,int16_t key_index,int16_t enum_value_index);
//////////////////////GET STRING FROM  ID///////////////////////////

int8_t* Get_Cmd_String_By_Id(int16_t cmd_id);
int8_t* Get_Key_String_By_Id(int16_t cmd_id,int16_t key_id);
int8_t* Get_Enum_Value_String_By_Id(int16_t cmd_id,int16_t key_id,int16_t enum_value_id);


#pragma pack()

#ifdef __cplusplus
}
#endif

#endif



"""






cmd_count = 0
key_count = 0
value_count = 0





################  CMD STRING ###################################



DC_H_BODY = ""

def DC_H_Body_Add(id,define_name):
    global DC_H_BODY
    DC_H_BODY = DC_H_BODY + ("#define %s  0x%04X \r\n")%(define_name , id)
    
def DC_H_BODY_STRING_OUTPUT():
    global DC_H_BODY
    return DC_H_BODY



DC_HEAD_STRING = ""
DC_BODY_STRING = ""
DC_END_STRING  = ""

def DC_Body_Add(buf):
    global DC_BODY_STRING
    DC_BODY_STRING = DC_BODY_STRING + buf

def DC_Head_Add(buf):
    global DC_HEAD_STRING
    DC_HEAD_STRING = DC_HEAD_STRING + buf

def DC_End_Add(buf):
    global DC_END_STRING
    DC_END_STRING = DC_END_STRING + buf

def DC_STRING_OUTPUT():
    
    global DC_BODY_STRING
    global DC_HEAD_STRING
    global DC_END_STRING

    res = DC_HEAD_STRING + DC_BODY_STRING + DC_END_STRING
    return res


################  KEY STRING ########################################### 


DPK_H_BODY = ""

def DPK_H_Body_Add(id,define_name):
    global DPK_H_BODY
    DPK_H_BODY = DPK_H_BODY + ("#define %s  0x%04X \r\n")%(define_name , id)
    
def DPK_H_BODY_STRING_OUTPUT():
    global DPK_H_BODY
    return DPK_H_BODY




    
DPK_BODY_STRING = ""
DPK_HEAD_STRING = ""
DPK_END_STRING  = ""

def DPK_Body_Add(buf):
    global DPK_BODY_STRING
    DPK_BODY_STRING = DPK_BODY_STRING + buf

def DPK_Head_Add(buf):
    global DPK_HEAD_STRING
    DPK_HEAD_STRING = DPK_HEAD_STRING + buf

def DPK_End_Add(buf):
    global DPK_END_STRING
    DPK_END_STRING = DPK_END_STRING + buf

def DPK_STRING_OUTPUT():
    
    global DPK_BODY_STRING
    global DPK_HEAD_STRING
    global DPK_END_STRING

    return DPK_HEAD_STRING + DPK_BODY_STRING + DPK_END_STRING

################  VALUE STRING ########################################### 


DPV_H_BODY = ""

def DPV_H_Body_Add(id,define_name):
    global DPV_H_BODY
    DPV_H_BODY = DPV_H_BODY + ("#define %s  0x%04X \r\n")%(define_name , id)
    
def DPV_H_BODY_STRING_OUTPUT():
    global DPV_H_BODY
    return DPV_H_BODY


    
DPV_BODY_STRING = ""
DPV_HEAD_STRING = ""
DPV_END_STRING  = ""

def DPV_Body_Add(buf):
    global DPV_BODY_STRING
    DPV_BODY_STRING = DPV_BODY_STRING + buf

def DPV_Head_Add(buf):
    global DPV_HEAD_STRING
    DPV_HEAD_STRING = DPV_HEAD_STRING + buf

def DPV_End_Add(buf):
    global DPV_END_STRING
    DPV_END_STRING = DPV_END_STRING + buf

def DPV_STRING_OUTPUT():
    
    global DPV_BODY_STRING
    global DPV_HEAD_STRING
    global DPV_END_STRING

    return DPV_HEAD_STRING + DPV_BODY_STRING + DPV_END_STRING


########################### cmd ###########################################

def Begain_Cmd(cmd_name):
    global key_count
    key_count = 0

    temp_buf="==========="+cmd_name+"===============\r\n"
    #print(temp_buf)
    #DC_Head_Add(temp_buf)
    #PARAM_KEY_ELEMENT lightsource_key_string_list[]=
    DPK_Body_Add("PARAM_KEY_ELEMENT "+cmd_name.lower()+"_key_string_list[]=\r\n{ \r\n")


def On_One_Cmd(cmd_name,param_cnt):
    global cmd_count
    cmd_count = cmd_count +1
    define_buf = ("DC_"+cmd_name.upper()).upper()
    temp_buf = "{ %s , %s,%d,\"%s\" }, // %d \r\n"%(define_buf,(cmd_name+"_key_string_List").lower(),param_cnt,cmd_name,cmd_count)

    DC_H_Body_Add(cmd_count,define_buf)
    DC_Body_Add(temp_buf)
    
        
def End_Cmd(cmd_name):
    temp_buf = "====END======"+cmd_name+"===============\r\n"
    #DC_End_Add(temp_buf)
    DPK_Body_Add("};\r\n")

#########################  key  ###########################################


def Begain_Key(cmd_name,key_name):
    global key_count
    #key_count = 0
    print("----Begain_Key----------")
    DPV_Body_Add("PARAM_VALUE_ELEMENT "+ cmd_name.lower() + "_" + key_name.lower() + "_value_string_list[]= \r\n{ \r\n")


def On_One_Key(cmd_name,key_name,value_type,value_cnt):
    global cmd_count
    global key_count


    vt = ""
    if value_type == "string":
        vt = "VALUE_TYPE_STRING"
        
    elif value_type == "enum":
        vt = "VALUE_TYPE_ENUM"
    elif value_type == "int":
        vt = "VALUE_TYPE_INT"
    else:
        vt = "VALUE_TYPE_ENUM"
    print(vt)
    key_count = key_count + 1
    define_buf = ("DPK_"+cmd_name+"_"+key_name).upper()
    temp_buf = ("{ %s,%s, %s,%d,\"%s\"  }, // %d \r\n"%(define_buf ,vt,(cmd_name+"_"+ key_name+"_value_string_list").lower(),value_cnt,key_name,key_count))

    DPK_H_Body_Add(key_count,define_buf)
    DPK_Body_Add(temp_buf)




        
def End_Key(cmd_name , key_name):
    global key_count
    #print("----end_Key----------")
    #key_count = 0
    DPV_Body_Add("};\r\n")




def Traversal_Key(cmd_name,param):
    global cmd_count
    global key_countd
    #Begain_Key(cmd_name,p["key_name"])
    for p in param:
        Begain_Key(cmd_name,p["key_name"])
        On_One_Key(cmd_name,p["key_name"],p["value_type"],len(p["enum_name"]))
        Traversal_Value(cmd_name,p["key_name"],p["enum_name"])
        End_Key(cmd_name,p["key_name"]) 
    

#########################  value ################################################



def Begain_Value(cmd_name,key_name):
    global value_count
    #value_count = 0



def On_One_Value(cmd_name,key_name,value_name):
    global value_count
    value_count = value_count + 1
    
    

    define_buf = ("DPV_"+cmd_name+"_"+ key_name + "_"+ value_name).upper()
    temp_buf = ("{ %s ,\"%s\" }, // %d \r\n"%( define_buf ,value_name,value_count))
    
    DPV_H_Body_Add(value_count,define_buf)
    DPV_Body_Add(temp_buf)
        
def End_Value(cmd_name,key_name):
    global value_count
    #value_count = 0





def Traversal_Value(cmd_name,key_name,value_dict):
    Begain_Value(cmd_name,key_name)
    for v in value_dict:
        #print(type(v))
        On_One_Value(cmd_name,key_name,v)

    End_Value(cmd_name,key_name)




#############################################################################################

def List_Param(p):

    for x in p:
        print(x['type'])
        print(x['enum_name'])


#def Get_Param_Cnt(json_obj,cmd):
    



def Praser_Json(obj_json):
    
    DC_Body_Add("ELEMENT cmd_string_list[]=\r\n{\r\n")
    
    for cmd in obj_json['cmd_list']:
        Begain_Cmd(cmd['cmd']['name'])
        On_One_Cmd(cmd['cmd']['name'],len(cmd['param']))
        Traversal_Key(cmd['cmd']['name'],cmd['param'])
        End_Cmd(cmd['cmd']['name'])
        
    DC_Body_Add("};\r\n")

    print("=======================OUT_PUT========================")

    
    file_c = AT_CMD_HEAD_C + (DPV_STRING_OUTPUT())+ (DPK_STRING_OUTPUT())  +(DC_STRING_OUTPUT())   + AT_CMD_END_C



    file_h = AT_CMD_H_HEAD + DC_H_BODY_STRING_OUTPUT() + DPK_H_BODY_STRING_OUTPUT() + DPV_H_BODY_STRING_OUTPUT() + AT_CMD_H_END





    return file_c ,file_h

def Praser_Jsonfile_Save(json_file_name,save_filename):
    
    fd_json = open(json_file_name,"r+")
    fc_save_c = open(save_filename+".c","w+")
    fc_save_h = open(save_filename+".h","w+")
    
    buf = fd_json.read()
    fd_json.close()
    json_obj = json.loads(buf)
    
    file_c,file_h = Praser_Json(json_obj)
    
    fc_save_c.write(file_c)

    fc_save_c.close()


    fc_save_h.write(file_h)
    fc_save_h.close()





Praser_Jsonfile_Save(JSON_FILE_NAME,OUTPUT_FILE_NAME)





import os
import sys
import xlrd
import json




#nrows = table.nrows  #获取该sheet中的有效行数
#print(nrows)
#row_list1 = table.row(1)  #返回由该行中所有的单元格对象组成的列表
#print(row_list1)

#row_list0 = table.row_slice(0)  #返回由该列中所有的单元格对象组成的列表
#print(row_list0)


#table.row_types(rowx, start_colx=0, end_colx=None)    #返回由该行中所有单元格的数据类型组成的列表

#table.row_values(rowx, start_colx=0, end_colx=None)   #返回由该行中所有单元格的数据组成的列表

#table.row_len(rowx) #返回该列的有效单元格长度





#ncols = table.ncols   #获取列表的有效列数

#col = table.col(0, start_rowx=0, end_rowx=None)  #返回由该列中所有的单元格对象组成的列表
#print(col)
#table.col_slice(colx, start_rowx=0, end_rowx=None)  #返回由该列中所有的单元格对象组成的列表

#table.col_types(colx, start_rowx=0, end_rowx=None)    #返回由该列中所有单元格的数据类型组成的列表

#table.col_values(colx, start_rowx=0, end_rowx=None)   #返回由该列中所有单元格的数据组成的列表



#col = table.cell(0,0)   #返回单元格对象
#print(col)
#table.cell_type(rowx,colx)    #返回单元格中的数据类型

#table.cell_value(rowx,colx)   #返回单元格中的数据

#table.cell_xf_index(rowx, colx)   # 暂时还没有搞懂

 
#nrows = table.nrows


#for i in range(table.nrows):
    
#    row_list = table.row(i)
#    print(dir(row_list[0]))

#    print(row_list[0].ctype)
#    print(row_list[0].dump)
#    print(row_list[0].value)
#    print(row_list[0].xf_index)
#    
    
    
    
    #for x in row_list:
    #    print(x.value)
    #    print(x.ctype)
    #if table.cell_value(i,0) is not "":
    #    print(table.cell_value(i,0))

    

#active_cmd
#active_key
#active_value



#table



###########################################################################




active_cmd = ""
active_key = ""
active_value =""
active_value_type =""

gJson_obj = {"cmd_list":[]}
#= {"cmd_list":["a","b"]}

#"cmd":{"name":"Power_Ctr"},


def Cmd_In(cmd):
    global active_cmd
    active_cmd = cmd.replace(' ','')
    On_Cmd_In()

def Key_In(key):
    global active_key
    active_key = key.replace(' ','')
    On_Key_In()


def Value_In(value):
    global active_value
    active_value = value.replace(' ','')
    On_Value_In()

def Value_Type_In(value):
    global active_value_type
    active_value_type = value.replace(' ','')
    On_Value_Type_In()



#############################################################################


def On_Cmd_In():
    global gJson_obj
    global active_cmd
    tc = {"cmd":{"name":active_cmd},"param":[]}
    gJson_obj["cmd_list"].append(tc)
    #print(gJson_obj["cmd_list"])



def On_Key_In():
    global gJson_obj
    global active_key
    global active_cmd
    global active_value_type


    print("===================%s========"%active_value_type)
    tk = {"key_name":active_key,"value_type":active_value_type,"enum_name":[]}
    print(tk)
    for c in gJson_obj["cmd_list"]:

        if c["cmd"]["name"] == active_cmd:
            
            c["param"].append(tk)
            #print(c)



def On_Value_In():
    global gJson_obj
    global active_value
    global active_cmd
    global active_key
    
    for  c in gJson_obj["cmd_list"]:
        if c["cmd"]["name"] == active_cmd:
            for k in c["param"]:
                #print(k)
                #print(k["key_name"])
                if(k["key_name"] == active_key):
                    #print(active_value)
                    k["enum_name"].append(active_value)
               

def On_Value_Type_In():
    pass

###########################FILE   JSON###################################################

def Json_Obj_Save(obj,filename):
    
    res = json.dumps(obj)

    print(res)

    fd = open(filename,"w+")

    fd.write(res)

    fd.close()




##########################EXCEL #########################################################


Cmd_Index = 0
Key_Index = 0
Type_Index = 0
Value_Index = 0





def Get_Index():
    global Cmd_Index 
    global Key_Index 
    global Type_Index 
    global Value_Index

    row_head_list = table.row(0)
    i = 0
    for x in row_head_list:
        
        if x.value == "CMD" :
            Cmd_Index = i
            #print(x.value)
        if x.value == "KEY" :
            Key_Index = i 
            #print(x.value)
        if x.value == "VALUE" :
            #print(x.value)
            Value_Index = i 
        if x.value == "VALUE_TYPE" :
            #print(x.value)
            Type_Index = i
            print("type_index %d"%Type_Index)
        i = i +1



def Show_Index():
    global Cmd_Index 
    global Key_Index 
    global Type_Index 
    global Value_Index

    print("Cmd_Index:%d,Key_Index:%d,Type_Index:%d,Value_Index:%d"%(Cmd_Index,Key_Index,Type_Index,Value_Index))




def Traversal_Excel(tb):
    global Cmd_Index 
    global Key_Index 
    global Type_Index 
    global Value_Index

    
    nrows = tb.nrows
    print("rows:%d"%nrows)
    for r  in range(1,nrows):
        if tb.cell_value(r,Type_Index) is not "":
            print(tb.cell_value(r,Type_Index))
            Value_Type_In(tb.cell_value(r,Type_Index))
            
        if tb.cell_value(r,Cmd_Index) is not "":
            print(tb.cell_value(r,Cmd_Index))
            Cmd_In(tb.cell_value(r,Cmd_Index))

        if tb.cell_value(r,Key_Index) is not "":
            print(tb.cell_value(r,Key_Index))
            Key_In(tb.cell_value(r,Key_Index))

        if tb.cell_value(r,Value_Index) is not "":
            print(tb.cell_value(r,Value_Index))
            Value_In(tb.cell_value(r,Value_Index))
            


data = xlrd.open_workbook(r"./at_cmd.xlsx")

table = data.sheet_by_name("AT_CMD")




Get_Index()
Show_Index()

Traversal_Excel(table)


print(gJson_obj)
Json_Obj_Save(gJson_obj,"./json_out.json")


#######################################################################################


r"""
Cmd_In("Laser_Ctr")
Key_In("Status")
Value_In("Off")
Value_In("On")
Key_In("SB")
Value_In("ABC")


Value_In("Off")
Value_In("On")
Value_In("NG")



Cmd_In("Power")
Key_In("Status111")
Value_In("NG")


#print(gJson_obj)



res2 = json.dumps(gJson_obj)

print(res2)

fd = open("./json_out.json","w+")

fd.write(res2)

fd.close()

"""
    









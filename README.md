# DISPLAY_CTR_L8_STM32F730

显示控制板MCU

#prejeck: DISPLAY_CTR_L8_STM32F730

#object:cd MDK-ARM && UV4 -r L8_PMU.uvprojx

#output start: 

.\MDK-ARM\L8_PMU\*.hex

#output end;

------------------------------------------------------------
版本说明：
-----------------------------V1.0-------------------------------
20200601：
1.添加auto_power_task，为周期性开关光服务
2.完成了基本的电源控制，TEC控制功能

-----------------------------V1.1-------------------------------
20200614：
1.添加5AA5协议
2.编写与Imx8的电源设置的对接函数



#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "stdint.h"
#include "at_cmd.h"


#define SIZE_KEY_CNT(X)     sizeof(X)/sizeof(PARAM_KEY_ELEMENT)
#define SIZE_VALUE_CNT(X)   sizeof(X)/sizeof(PARAM_VALUE_ELEMENT)


PARAM_VALUE_ELEMENT system_status_value_string_list[]= 
{ 
{ DPV_SYSTEM_STATUS_ON ,"On" }, // 1 
{ DPV_SYSTEM_STATUS_OFF ,"Off" }, // 2 
{ DPV_SYSTEM_STATUS_IDLE ,"Idle" }, // 3 
{ DPV_SYSTEM_STATUS_SELFCHECKING ,"SelfChecking" }, // 4 
{ DPV_SYSTEM_STATUS_SELFCHECKOK ,"SelfCheckOk" }, // 5 
{ DPV_SYSTEM_STATUS_ONING ,"Oning" }, // 6 
{ DPV_SYSTEM_STATUS_OFFING ,"Offing" }, // 7 
{ DPV_SYSTEM_STATUS_STANDBY ,"Standby" }, // 8 
{ DPV_SYSTEM_STATUS_ERROR ,"Error" }, // 9 
{ DPV_SYSTEM_STATUS_OK ,"Ok" }, // 10 
};
PARAM_VALUE_ELEMENT lightsource_status_value_string_list[]= 
{ 
{ DPV_LIGHTSOURCE_STATUS_ON ,"On" }, // 11 
{ DPV_LIGHTSOURCE_STATUS_OFF ,"Off" }, // 12 
{ DPV_LIGHTSOURCE_STATUS_OK ,"Ok" }, // 13 
};
PARAM_VALUE_ELEMENT version_dlpsoftware_value_string_list[]= 
{ 
{ DPV_VERSION_DLPSOFTWARE_EMPTY ,"Empty" }, // 14 
};
PARAM_VALUE_ELEMENT version_pmusoftware_value_string_list[]= 
{ 
{ DPV_VERSION_PMUSOFTWARE_EMPTY ,"Empty" }, // 15 
};
PARAM_VALUE_ELEMENT version_idusoftware_value_string_list[]= 
{ 
{ DPV_VERSION_IDUSOFTWARE_EMPTY ,"Empty" }, // 16 
};
PARAM_VALUE_ELEMENT version_dlphardware_value_string_list[]= 
{ 
{ DPV_VERSION_DLPHARDWARE_EMPTY ,"Empty" }, // 17 
};
PARAM_VALUE_ELEMENT version_pmuhardware_value_string_list[]= 
{ 
{ DPV_VERSION_PMUHARDWARE_EMPTY ,"Empty" }, // 18 
};
PARAM_VALUE_ELEMENT version_iduhardware_value_string_list[]= 
{ 
{ DPV_VERSION_IDUHARDWARE_EMPTY ,"Empty" }, // 19 
};
PARAM_VALUE_ELEMENT version_lenssoftware_value_string_list[]= 
{ 
{ DPV_VERSION_LENSSOFTWARE_EMPTY ,"Empty" }, // 20 
};
PARAM_VALUE_ELEMENT version_lenshardware_value_string_list[]= 
{ 
{ DPV_VERSION_LENSHARDWARE_EMPTY ,"Empty" }, // 21 
};
PARAM_VALUE_ELEMENT sn_sourcelight_value_string_list[]= 
{ 
{ DPV_SN_SOURCELIGHT_EMPTY ,"Empty" }, // 22 
};
PARAM_VALUE_ELEMENT sn_projector_value_string_list[]= 
{ 
{ DPV_SN_PROJECTOR_EMPTY ,"Empty" }, // 23 
};
PARAM_VALUE_ELEMENT sn_lightengine_value_string_list[]= 
{ 
{ DPV_SN_LIGHTENGINE_EMPTY ,"Empty" }, // 24 
};
PARAM_VALUE_ELEMENT lightsourcetime_minute_value_string_list[]= 
{ 
{ DPV_LIGHTSOURCETIME_MINUTE_EMPTY ,"Empty" }, // 25 
};
PARAM_VALUE_ELEMENT gamma_status_value_string_list[]= 
{ 
{ DPV_GAMMA_STATUS_EMPTY ,"Empty" }, // 26 
};
PARAM_VALUE_ELEMENT installationmode_status_value_string_list[]= 
{ 
{ DPV_INSTALLATIONMODE_STATUS_CEILINGREAR ,"CeilingRear" }, // 27 
{ DPV_INSTALLATIONMODE_STATUS_CEILINGFRONT ,"CeilingFront" }, // 28 
{ DPV_INSTALLATIONMODE_STATUS_TABLEREAR ,"TableRear" }, // 29 
{ DPV_INSTALLATIONMODE_STATUS_TABLEFRONT ,"TableFront" }, // 30 
{ DPV_INSTALLATIONMODE_STATUS_OK ,"Ok" }, // 31 
};
PARAM_VALUE_ELEMENT verticalkeystone_status_value_string_list[]= 
{ 
{ DPV_VERTICALKEYSTONE_STATUS_EMPTY ,"Empty" }, // 32 
};
PARAM_VALUE_ELEMENT horizonkeystone_status_value_string_list[]= 
{ 
{ DPV_HORIZONKEYSTONE_STATUS_EMPTY ,"Empty" }, // 33 
};
PARAM_VALUE_ELEMENT keystone2d_topleftx_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_TOPLEFTX_EMPTY ,"Empty" }, // 34 
};
PARAM_VALUE_ELEMENT keystone2d_toplefty_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_TOPLEFTY_EMPTY ,"Empty" }, // 35 
};
PARAM_VALUE_ELEMENT keystone2d_toprightx_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_TOPRIGHTX_EMPTY ,"Empty" }, // 36 
};
PARAM_VALUE_ELEMENT keystone2d_toprighty_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_TOPRIGHTY_EMPTY ,"Empty" }, // 37 
};
PARAM_VALUE_ELEMENT keystone2d_bottomleftx_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_BOTTOMLEFTX_EMPTY ,"Empty" }, // 38 
};
PARAM_VALUE_ELEMENT keystone2d_bottomlefty_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_BOTTOMLEFTY_EMPTY ,"Empty" }, // 39 
};
PARAM_VALUE_ELEMENT keystone2d_bottomrightx_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_BOTTOMRIGHTX_EMPTY ,"Empty" }, // 40 
};
PARAM_VALUE_ELEMENT keystone2d_bottomrighty_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_BOTTOMRIGHTY_EMPTY ,"Empty" }, // 41 
};
PARAM_VALUE_ELEMENT testpattern_status_value_string_list[]= 
{ 
{ DPV_TESTPATTERN_STATUS_OFF ,"Off" }, // 42 
{ DPV_TESTPATTERN_STATUS_RGBRAMPS ,"RGBRamps" }, // 43 
{ DPV_TESTPATTERN_STATUS_COLORBARS ,"ColorBars" }, // 44 
{ DPV_TESTPATTERN_STATUS_SETBARS ,"SetBars" }, // 45 
{ DPV_TESTPATTERN_STATUS_CHECKERBOARD ,"CheckErboard" }, // 46 
{ DPV_TESTPATTERN_STATUS_GRID ,"Grid" }, // 47 
{ DPV_TESTPATTERN_STATUS_HORIZONTALLINES ,"HorizontalLines" }, // 48 
{ DPV_TESTPATTERN_STATUS_VERTICALLINES ,"VerticalLines" }, // 49 
{ DPV_TESTPATTERN_STATUS_DIAGONALLINES ,"DiagonalLines" }, // 50 
{ DPV_TESTPATTERN_STATUS_HORIZONTALRAMP ,"HorizontalRamp" }, // 51 
{ DPV_TESTPATTERN_STATUS_VERTICALRAMP ,"VerticalRamp" }, // 52 
{ DPV_TESTPATTERN_STATUS_BLACK ,"Black" }, // 53 
{ DPV_TESTPATTERN_STATUS_BLUE ,"Blue" }, // 54 
{ DPV_TESTPATTERN_STATUS_GREY ,"Grey" }, // 55 
{ DPV_TESTPATTERN_STATUS_RED ,"Red" }, // 56 
{ DPV_TESTPATTERN_STATUS_GREEN ,"Green" }, // 57 
{ DPV_TESTPATTERN_STATUS_YELLOW ,"Yellow" }, // 58 
{ DPV_TESTPATTERN_STATUS_CYAN ,"Cyan" }, // 59 
{ DPV_TESTPATTERN_STATUS_MEGENTA ,"Megenta" }, // 60 
{ DPV_TESTPATTERN_STATUS_WHITE ,"White" }, // 61 
{ DPV_TESTPATTERN_STATUS_XPR ,"Xpr" }, // 62 
{ DPV_TESTPATTERN_STATUS_OK ,"Ok" }, // 63 
};
PARAM_VALUE_ELEMENT mode3d_mode3d_value_string_list[]= 
{ 
{ DPV_MODE3D_MODE3D_OFF ,"Off" }, // 64 
{ DPV_MODE3D_MODE3D_VSYNCSEPARATEDHALF ,"VsyncSeparatedHalf" }, // 65 
{ DPV_MODE3D_MODE3D_VSYNCSEPARATEDFULL ,"VsyncSeparatedFull" }, // 66 
{ DPV_MODE3D_MODE3D_VERTPACKEDHALF ,"VertPackedHalf" }, // 67 
{ DPV_MODE3D_MODE3D_VERTPACKEDFULL ,"VertPackedFull" }, // 68 
{ DPV_MODE3D_MODE3D_HORIZPACKEDHALF ,"HorizPackedHalf" }, // 69 
{ DPV_MODE3D_MODE3D_HORIZPACKEDFULL ,"HorizPackedFull" }, // 70 
{ DPV_MODE3D_MODE3D_OK ,"Ok" }, // 71 
};
PARAM_VALUE_ELEMENT syncflip3d_status_value_string_list[]= 
{ 
{ DPV_SYNCFLIP3D_STATUS_ON ,"On" }, // 72 
{ DPV_SYNCFLIP3D_STATUS_OFF ,"Off" }, // 73 
{ DPV_SYNCFLIP3D_STATUS_OK ,"Ok" }, // 74 
};
PARAM_VALUE_ELEMENT projectionmode_status_value_string_list[]= 
{ 
{ DPV_PROJECTIONMODE_STATUS_HIGHLIGHT ,"HighLight" }, // 75 
{ DPV_PROJECTIONMODE_STATUS_MOVIE ,"Movie" }, // 76 
{ DPV_PROJECTIONMODE_STATUS_STANDARD ,"Standard" }, // 77 
{ DPV_PROJECTIONMODE_STATUS_POWERSAVE ,"PowerSave" }, // 78 
{ DPV_PROJECTIONMODE_STATUS_INVILID ,"Invilid" }, // 79 
{ DPV_PROJECTIONMODE_STATUS_OK ,"Ok" }, // 80 
};
PARAM_VALUE_ELEMENT fanstatus_fan1_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN1_EMPTY ,"Empty" }, // 81 
};
PARAM_VALUE_ELEMENT fanstatus_fan2_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN2_EMPTY ,"Empty" }, // 82 
};
PARAM_VALUE_ELEMENT fanstatus_fan3_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN3_EMPTY ,"Empty" }, // 83 
};
PARAM_VALUE_ELEMENT fanstatus_fan4_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN4_EMPTY ,"Empty" }, // 84 
};
PARAM_VALUE_ELEMENT fanstatus_fan5_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN5_EMPTY ,"Empty" }, // 85 
};
PARAM_VALUE_ELEMENT fanstatus_fan6_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN6_EMPTY ,"Empty" }, // 86 
};
PARAM_VALUE_ELEMENT fanstatus_fan7_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN7_EMPTY ,"Empty" }, // 87 
};
PARAM_VALUE_ELEMENT fanstatus_fan8_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN8_EMPTY ,"Empty" }, // 88 
};
PARAM_VALUE_ELEMENT fanstatus_fan9_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN9_EMPTY ,"Empty" }, // 89 
};
PARAM_VALUE_ELEMENT fanstatus_fan10_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN10_EMPTY ,"Empty" }, // 90 
};
PARAM_VALUE_ELEMENT fanstatus_fan11_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN11_EMPTY ,"Empty" }, // 91 
};
PARAM_VALUE_ELEMENT fanstatus_fan12_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN12_EMPTY ,"Empty" }, // 92 
};
PARAM_VALUE_ELEMENT fanstatus_fan13_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN13_EMPTY ,"Empty" }, // 93 
};
PARAM_VALUE_ELEMENT fanstatus_fan14_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN14_EMPTY ,"Empty" }, // 94 
};
PARAM_VALUE_ELEMENT fanstatus_fan15_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN15_EMPTY ,"Empty" }, // 95 
};
PARAM_VALUE_ELEMENT fanstatus_fan16_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN16_EMPTY ,"Empty" }, // 96 
};
PARAM_VALUE_ELEMENT fanstatus_fan17_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN17_EMPTY ,"Empty" }, // 97 
};
PARAM_VALUE_ELEMENT fanstatus_fan18_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN18_EMPTY ,"Empty" }, // 98 
};
PARAM_VALUE_ELEMENT fanstatus_fan19_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN19_EMPTY ,"Empty" }, // 99 
};
PARAM_VALUE_ELEMENT fanstatus_fan20_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN20_EMPTY ,"Empty" }, // 100 
};
PARAM_VALUE_ELEMENT fanstatus_fan21_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN21_EMPTY ,"Empty" }, // 101 
};
PARAM_VALUE_ELEMENT fanstatus_fan22_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN22_EMPTY ,"Empty" }, // 102 
};
PARAM_VALUE_ELEMENT fanstatus_fan23_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN23_EMPTY ,"Empty" }, // 103 
};
PARAM_VALUE_ELEMENT fanstatus_fan24_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN24_EMPTY ,"Empty" }, // 104 
};
PARAM_VALUE_ELEMENT fanstatus_fan25_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN25_EMPTY ,"Empty" }, // 105 
};
PARAM_VALUE_ELEMENT fanstatus_fan26_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN26_EMPTY ,"Empty" }, // 106 
};
PARAM_VALUE_ELEMENT fanstatus_fan27_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN27_EMPTY ,"Empty" }, // 107 
};
PARAM_VALUE_ELEMENT fanstatus_all_value_string_list[]= 
{ 
{ DPV_FANSTATUS_ALL_EMPTY ,"Empty" }, // 108 
};
PARAM_VALUE_ELEMENT setfan_fan1_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN1_EMPTY ,"Empty" }, // 109 
};
PARAM_VALUE_ELEMENT setfan_fan2_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN2_EMPTY ,"Empty" }, // 110 
};
PARAM_VALUE_ELEMENT setfan_fan3_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN3_EMPTY ,"Empty" }, // 111 
};
PARAM_VALUE_ELEMENT setfan_fan4_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN4_EMPTY ,"Empty" }, // 112 
};
PARAM_VALUE_ELEMENT setfan_fan5_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN5_EMPTY ,"Empty" }, // 113 
};
PARAM_VALUE_ELEMENT setfan_fan6_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN6_EMPTY ,"Empty" }, // 114 
};
PARAM_VALUE_ELEMENT setfan_fan7_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN7_EMPTY ,"Empty" }, // 115 
};
PARAM_VALUE_ELEMENT setfan_fan8_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN8_EMPTY ,"Empty" }, // 116 
};
PARAM_VALUE_ELEMENT setfan_fan9_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN9_EMPTY ,"Empty" }, // 117 
};
PARAM_VALUE_ELEMENT setfan_fan10_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN10_EMPTY ,"Empty" }, // 118 
};
PARAM_VALUE_ELEMENT setfan_fan11_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN11_EMPTY ,"Empty" }, // 119 
};
PARAM_VALUE_ELEMENT setfan_fan12_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN12_EMPTY ,"Empty" }, // 120 
};
PARAM_VALUE_ELEMENT setfan_fan13_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN13_EMPTY ,"Empty" }, // 121 
};
PARAM_VALUE_ELEMENT setfan_fan14_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN14_EMPTY ,"Empty" }, // 122 
};
PARAM_VALUE_ELEMENT setfan_fan15_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN15_EMPTY ,"Empty" }, // 123 
};
PARAM_VALUE_ELEMENT setfan_fan16_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN16_EMPTY ,"Empty" }, // 124 
};
PARAM_VALUE_ELEMENT setfan_fan17_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN17_EMPTY ,"Empty" }, // 125 
};
PARAM_VALUE_ELEMENT setfan_fan18_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN18_EMPTY ,"Empty" }, // 126 
};
PARAM_VALUE_ELEMENT setfan_fan19_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN19_EMPTY ,"Empty" }, // 127 
};
PARAM_VALUE_ELEMENT setfan_fan20_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN20_EMPTY ,"Empty" }, // 128 
};
PARAM_VALUE_ELEMENT setfan_fan21_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN21_EMPTY ,"Empty" }, // 129 
};
PARAM_VALUE_ELEMENT setfan_fan22_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN22_EMPTY ,"Empty" }, // 130 
};
PARAM_VALUE_ELEMENT setfan_fan23_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN23_EMPTY ,"Empty" }, // 131 
};
PARAM_VALUE_ELEMENT setfan_fan24_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN24_EMPTY ,"Empty" }, // 132 
};
PARAM_VALUE_ELEMENT setfan_fan25_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN25_EMPTY ,"Empty" }, // 133 
};
PARAM_VALUE_ELEMENT setfan_fan26_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN26_EMPTY ,"Empty" }, // 134 
};
PARAM_VALUE_ELEMENT setfan_fan27_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN27_EMPTY ,"Empty" }, // 135 
};
PARAM_VALUE_ELEMENT setfan_all_value_string_list[]= 
{ 
{ DPV_SETFAN_ALL_EMPTY ,"Empty" }, // 136 
};
PARAM_VALUE_ELEMENT temperature_ntcdmd1_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCDMD1_EMPTY ,"Empty" }, // 137 
};
PARAM_VALUE_ELEMENT temperature_ntccw1_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCCW1_EMPTY ,"Empty" }, // 138 
};
PARAM_VALUE_ELEMENT temperature_ntcbluelaser1_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCBLUELASER1_EMPTY ,"Empty" }, // 139 
};
PARAM_VALUE_ELEMENT temperature_ntcbluelaser2_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCBLUELASER2_EMPTY ,"Empty" }, // 140 
};
PARAM_VALUE_ELEMENT temperature_ntcxpr1_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCXPR1_EMPTY ,"Empty" }, // 141 
};
PARAM_VALUE_ELEMENT temperature_ntcenv1_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCENV1_EMPTY ,"Empty" }, // 142 
};
PARAM_VALUE_ELEMENT temperature_ntcpowersupply_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCPOWERSUPPLY_EMPTY ,"Empty" }, // 143 
};
PARAM_VALUE_ELEMENT cwspeed_status_value_string_list[]= 
{ 
{ DPV_CWSPEED_STATUS_STATUS ,"Status" }, // 144 
};
PARAM_VALUE_ELEMENT xprfixed_status_value_string_list[]= 
{ 
{ DPV_XPRFIXED_STATUS_ON ,"On" }, // 145 
{ DPV_XPRFIXED_STATUS_OFF ,"Off" }, // 146 
{ DPV_XPRFIXED_STATUS_ERROR ,"Error" }, // 147 
{ DPV_XPRFIXED_STATUS_INVILID ,"Invilid" }, // 148 
{ DPV_XPRFIXED_STATUS_OK ,"Ok" }, // 149 
};
PARAM_VALUE_ELEMENT reset_status_value_string_list[]= 
{ 
{ DPV_RESET_STATUS_FACTORY ,"Factory" }, // 150 
{ DPV_RESET_STATUS_USER ,"User" }, // 151 
{ DPV_RESET_STATUS_SERVICER ,"Servicer" }, // 152 
{ DPV_RESET_STATUS_OK ,"Ok" }, // 153 
};
PARAM_VALUE_ELEMENT signalchannel_none_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_NONE_EMPTY ,"Empty" }, // 154 
};
PARAM_VALUE_ELEMENT signalchannel_hdmi_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_HDMI_EMPTY ,"Empty" }, // 155 
};
PARAM_VALUE_ELEMENT signalchannel_vga_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_VGA_EMPTY ,"Empty" }, // 156 
};
PARAM_VALUE_ELEMENT signalchannel_cds_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_CDS_EMPTY ,"Empty" }, // 157 
};
PARAM_VALUE_ELEMENT signalchannel_dvi_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_DVI_EMPTY ,"Empty" }, // 158 
};
PARAM_VALUE_ELEMENT signalchannel_ycbcr_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_YCBCR_EMPTY ,"Empty" }, // 159 
};
PARAM_VALUE_ELEMENT signalchannel_sdi_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_SDI_EMPTY ,"Empty" }, // 160 
};
PARAM_VALUE_ELEMENT aspectratio_status_value_string_list[]= 
{ 
{ DPV_ASPECTRATIO_STATUS_NATIVE ,"Native" }, // 161 
{ DPV_ASPECTRATIO_STATUS_FILL ,"Fill" }, // 162 
{ DPV_ASPECTRATIO_STATUS_4_3 ,"4_3" }, // 163 
{ DPV_ASPECTRATIO_STATUS_16_6 ,"16_6" }, // 164 
{ DPV_ASPECTRATIO_STATUS_16_9 ,"16_9" }, // 165 
{ DPV_ASPECTRATIO_STATUS_16_10 ,"16_10" }, // 166 
{ DPV_ASPECTRATIO_STATUS_OK ,"Ok" }, // 167 
};
PARAM_VALUE_ELEMENT stanbytimes_status_value_string_list[]= 
{ 
{ DPV_STANBYTIMES_STATUS_EMPTY ,"Empty" }, // 168 
};
PARAM_VALUE_ELEMENT heartbeat_status_value_string_list[]= 
{ 
{ DPV_HEARTBEAT_STATUS_EMPTY ,"Empty" }, // 169 
};
PARAM_VALUE_ELEMENT eeprom_dlpvendorname_value_string_list[]= 
{ 
{ DPV_EEPROM_DLPVENDORNAME_EMPTY ,"Empty" }, // 170 
};
PARAM_VALUE_ELEMENT eeprom_colorwheeldelay_value_string_list[]= 
{ 
{ DPV_EEPROM_COLORWHEELDELAY_EMPTY ,"Empty" }, // 171 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodehighlight_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODEHIGHLIGHT_EMPTY ,"Empty" }, // 172 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodemovie_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODEMOVIE_EMPTY ,"Empty" }, // 173 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodestadard_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODESTADARD_EMPTY ,"Empty" }, // 174 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodepowersave_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODEPOWERSAVE_EMPTY ,"Empty" }, // 175 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodehighlight1_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODEHIGHLIGHT1_EMPTY ,"Empty" }, // 176 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodemovie1_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODEMOVIE1_EMPTY ,"Empty" }, // 177 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodestadard1_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODESTADARD1_EMPTY ,"Empty" }, // 178 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodepowersave1_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODEPOWERSAVE1_EMPTY ,"Empty" }, // 179 
};
PARAM_VALUE_ELEMENT eeprom_colorwheelmode_value_string_list[]= 
{ 
{ DPV_EEPROM_COLORWHEELMODE_EMPTY ,"Empty" }, // 180 
};
PARAM_VALUE_ELEMENT eeprom_colorwheelspoke_value_string_list[]= 
{ 
{ DPV_EEPROM_COLORWHEELSPOKE_EMPTY ,"Empty" }, // 181 
};
PARAM_VALUE_ELEMENT eeprom_colorwheelid_value_string_list[]= 
{ 
{ DPV_EEPROM_COLORWHEELID_EMPTY ,"Empty" }, // 182 
};
PARAM_VALUE_ELEMENT eeprom_productionweek_value_string_list[]= 
{ 
{ DPV_EEPROM_PRODUCTIONWEEK_EMPTY ,"Empty" }, // 183 
};
PARAM_VALUE_ELEMENT eeprom_brightness_value_string_list[]= 
{ 
{ DPV_EEPROM_BRIGHTNESS_EMPTY ,"Empty" }, // 184 
};
PARAM_VALUE_ELEMENT eeprom_colorcoordinatevalue_value_string_list[]= 
{ 
{ DPV_EEPROM_COLORCOORDINATEVALUE_EMPTY ,"Empty" }, // 185 
};
PARAM_VALUE_ELEMENT eeprom_contrastratio_value_string_list[]= 
{ 
{ DPV_EEPROM_CONTRASTRATIO_EMPTY ,"Empty" }, // 186 
};
PARAM_VALUE_ELEMENT eeprom_brightnessuniformity_value_string_list[]= 
{ 
{ DPV_EEPROM_BRIGHTNESSUNIFORMITY_EMPTY ,"Empty" }, // 187 
};
PARAM_VALUE_ELEMENT eeprom_serialnumber_value_string_list[]= 
{ 
{ DPV_EEPROM_SERIALNUMBER_EMPTY ,"Empty" }, // 188 
};
PARAM_VALUE_ELEMENT eeprom_brightnesmode1_value_string_list[]= 
{ 
{ DPV_EEPROM_BRIGHTNESMODE1_EMPTY ,"Empty" }, // 189 
};
PARAM_VALUE_ELEMENT eeprom_brightnesmode2_value_string_list[]= 
{ 
{ DPV_EEPROM_BRIGHTNESMODE2_EMPTY ,"Empty" }, // 190 
};
PARAM_VALUE_ELEMENT eeprom_brightnesmode3_value_string_list[]= 
{ 
{ DPV_EEPROM_BRIGHTNESMODE3_EMPTY ,"Empty" }, // 191 
};
PARAM_VALUE_ELEMENT eeprom_brightnesmode4_value_string_list[]= 
{ 
{ DPV_EEPROM_BRIGHTNESMODE4_EMPTY ,"Empty" }, // 192 
};
PARAM_VALUE_ELEMENT eeprom_brightnesmode5_value_string_list[]= 
{ 
{ DPV_EEPROM_BRIGHTNESMODE5_EMPTY ,"Empty" }, // 193 
};
PARAM_VALUE_ELEMENT eeprom_brightnesmode6_value_string_list[]= 
{ 
{ DPV_EEPROM_BRIGHTNESMODE6_EMPTY ,"Empty" }, // 194 
};
PARAM_VALUE_ELEMENT eeprom_brightnesmode7_value_string_list[]= 
{ 
{ DPV_EEPROM_BRIGHTNESMODE7_EMPTY ,"Empty" }, // 195 
};
PARAM_VALUE_ELEMENT eeprom_brightnesmode8_value_string_list[]= 
{ 
{ DPV_EEPROM_BRIGHTNESMODE8_EMPTY ,"Empty" }, // 196 
};
PARAM_VALUE_ELEMENT eeprom_dlpversion_value_string_list[]= 
{ 
{ DPV_EEPROM_DLPVERSION_EMPTY ,"Empty" }, // 197 
};
PARAM_VALUE_ELEMENT eeprom_temlookuptable1_value_string_list[]= 
{ 
{ DPV_EEPROM_TEMLOOKUPTABLE1_EMPTY ,"Empty" }, // 198 
};
PARAM_VALUE_ELEMENT eeprom_temlookuptable2_value_string_list[]= 
{ 
{ DPV_EEPROM_TEMLOOKUPTABLE2_EMPTY ,"Empty" }, // 199 
};
PARAM_VALUE_ELEMENT eeprom_temlookuptable3_value_string_list[]= 
{ 
{ DPV_EEPROM_TEMLOOKUPTABLE3_EMPTY ,"Empty" }, // 200 
};
PARAM_VALUE_ELEMENT eeprom_temlookuptable4_value_string_list[]= 
{ 
{ DPV_EEPROM_TEMLOOKUPTABLE4_EMPTY ,"Empty" }, // 201 
};
PARAM_VALUE_ELEMENT eeprom_temlookuptable5_value_string_list[]= 
{ 
{ DPV_EEPROM_TEMLOOKUPTABLE5_EMPTY ,"Empty" }, // 202 
};
PARAM_VALUE_ELEMENT eeprom_currentinfo1_value_string_list[]= 
{ 
{ DPV_EEPROM_CURRENTINFO1_EMPTY ,"Empty" }, // 203 
};
PARAM_VALUE_ELEMENT eeprom_currentinfo2_value_string_list[]= 
{ 
{ DPV_EEPROM_CURRENTINFO2_EMPTY ,"Empty" }, // 204 
};
PARAM_VALUE_ELEMENT eeprom_currentinfo3_value_string_list[]= 
{ 
{ DPV_EEPROM_CURRENTINFO3_EMPTY ,"Empty" }, // 205 
};
PARAM_VALUE_ELEMENT eeprom_currentinfo4_value_string_list[]= 
{ 
{ DPV_EEPROM_CURRENTINFO4_EMPTY ,"Empty" }, // 206 
};
PARAM_VALUE_ELEMENT eeprom_currentinfo5_value_string_list[]= 
{ 
{ DPV_EEPROM_CURRENTINFO5_EMPTY ,"Empty" }, // 207 
};
PARAM_VALUE_ELEMENT eeprom_currentinfo6_value_string_list[]= 
{ 
{ DPV_EEPROM_CURRENTINFO6_EMPTY ,"Empty" }, // 208 
};
PARAM_VALUE_ELEMENT eeprom_currentinfo7_value_string_list[]= 
{ 
{ DPV_EEPROM_CURRENTINFO7_EMPTY ,"Empty" }, // 209 
};
PARAM_VALUE_ELEMENT eeprom_currentinfo8_value_string_list[]= 
{ 
{ DPV_EEPROM_CURRENTINFO8_EMPTY ,"Empty" }, // 210 
};
PARAM_VALUE_ELEMENT eeprom_cwid_value_string_list[]= 
{ 
{ DPV_EEPROM_CWID_EMPTY ,"Empty" }, // 211 
};
PARAM_VALUE_ELEMENT eeprom_xprid_value_string_list[]= 
{ 
{ DPV_EEPROM_XPRID_EMPTY ,"Empty" }, // 212 
};
PARAM_VALUE_ELEMENT eeprom_xprgain_value_string_list[]= 
{ 
{ DPV_EEPROM_XPRGAIN_EMPTY ,"Empty" }, // 213 
};
PARAM_VALUE_ELEMENT eeprom_xprsl_value_string_list[]= 
{ 
{ DPV_EEPROM_XPRSL_EMPTY ,"Empty" }, // 214 
};
PARAM_VALUE_ELEMENT eeprom_keystoneoffset_value_string_list[]= 
{ 
{ DPV_EEPROM_KEYSTONEOFFSET_EMPTY ,"Empty" }, // 215 
};
PARAM_VALUE_ELEMENT eeprom_keystonetr_value_string_list[]= 
{ 
{ DPV_EEPROM_KEYSTONETR_EMPTY ,"Empty" }, // 216 
};
PARAM_VALUE_ELEMENT eeprom_installationmode_value_string_list[]= 
{ 
{ DPV_EEPROM_INSTALLATIONMODE_EMPTY ,"Empty" }, // 217 
};
PARAM_VALUE_ELEMENT help_status_value_string_list[]= 
{ 
{ DPV_HELP_STATUS_EMPTY ,"Empty" }, // 218 
};
PARAM_VALUE_ELEMENT setcurrent_r_value_string_list[]= 
{ 
{ DPV_SETCURRENT_R_EMPTY ,"Empty" }, // 219 
};
PARAM_VALUE_ELEMENT setcurrent_g_value_string_list[]= 
{ 
{ DPV_SETCURRENT_G_EMPTY ,"Empty" }, // 220 
};
PARAM_VALUE_ELEMENT setcurrent_b_value_string_list[]= 
{ 
{ DPV_SETCURRENT_B_EMPTY ,"Empty" }, // 221 
};
PARAM_VALUE_ELEMENT setcurrent_y_value_string_list[]= 
{ 
{ DPV_SETCURRENT_Y_EMPTY ,"Empty" }, // 222 
};
PARAM_VALUE_ELEMENT setcurrent_r1_value_string_list[]= 
{ 
{ DPV_SETCURRENT_R1_EMPTY ,"Empty" }, // 223 
};
PARAM_VALUE_ELEMENT setcurrent_g1_value_string_list[]= 
{ 
{ DPV_SETCURRENT_G1_EMPTY ,"Empty" }, // 224 
};
PARAM_VALUE_ELEMENT setcurrent_b1_value_string_list[]= 
{ 
{ DPV_SETCURRENT_B1_EMPTY ,"Empty" }, // 225 
};
PARAM_VALUE_ELEMENT setcurrent_y1_value_string_list[]= 
{ 
{ DPV_SETCURRENT_Y1_EMPTY ,"Empty" }, // 226 
};
PARAM_VALUE_ELEMENT setcurrent_all_value_string_list[]= 
{ 
{ DPV_SETCURRENT_ALL_EMPTY ,"Empty" }, // 227 
};
PARAM_VALUE_ELEMENT error_status_value_string_list[]= 
{ 
{ DPV_ERROR_STATUS_EMPTY ,"Empty" }, // 228 
};
PARAM_VALUE_ELEMENT warning_status_value_string_list[]= 
{ 
{ DPV_WARNING_STATUS_EMPTY ,"Empty" }, // 229 
};
PARAM_VALUE_ELEMENT loginfo_status_value_string_list[]= 
{ 
{ DPV_LOGINFO_STATUS_EMPTY ,"Empty" }, // 230 
};
PARAM_VALUE_ELEMENT productinformation_modelname_value_string_list[]= 
{ 
{ DPV_PRODUCTINFORMATION_MODELNAME_EMPTY ,"Empty" }, // 231 
};
PARAM_VALUE_ELEMENT productinformation_brandname_value_string_list[]= 
{ 
{ DPV_PRODUCTINFORMATION_BRANDNAME_EMPTY ,"Empty" }, // 232 
};
PARAM_VALUE_ELEMENT productinformation_machinesn_value_string_list[]= 
{ 
{ DPV_PRODUCTINFORMATION_MACHINESN_EMPTY ,"Empty" }, // 233 
};
PARAM_VALUE_ELEMENT colorwheeldelay_status_value_string_list[]= 
{ 
{ DPV_COLORWHEELDELAY_STATUS_EMPTY ,"Empty" }, // 234 
};
PARAM_VALUE_ELEMENT brilliantcolor_status_value_string_list[]= 
{ 
{ DPV_BRILLIANTCOLOR_STATUS_ON ,"On" }, // 235 
{ DPV_BRILLIANTCOLOR_STATUS_OFF ,"Off" }, // 236 
{ DPV_BRILLIANTCOLOR_STATUS_OK ,"Ok" }, // 237 
};
PARAM_VALUE_ELEMENT standbymode_status_value_string_list[]= 
{ 
{ DPV_STANDBYMODE_STATUS_LOWPOWER ,"LowPower" }, // 238 
{ DPV_STANDBYMODE_STATUS_NETWORKSTANDBY ,"NetworkStandby" }, // 239 
{ DPV_STANDBYMODE_STATUS_OK ,"Ok" }, // 240 
};
PARAM_VALUE_ELEMENT volume_status_value_string_list[]= 
{ 
{ DPV_VOLUME_STATUS_EMPTY ,"Empty" }, // 241 
};
PARAM_VALUE_ELEMENT volumecontrol_status_value_string_list[]= 
{ 
{ DPV_VOLUMECONTROL_STATUS_MUTE ,"Mute" }, // 242 
{ DPV_VOLUMECONTROL_STATUS_UNMUTE ,"UnMute" }, // 243 
{ DPV_VOLUMECONTROL_STATUS_PLUS ,"Plus" }, // 244 
{ DPV_VOLUMECONTROL_STATUS_REDUCE ,"Reduce" }, // 245 
};
PARAM_VALUE_ELEMENT runtime_status_value_string_list[]= 
{ 
{ DPV_RUNTIME_STATUS_EMPTY ,"Empty" }, // 246 
};
PARAM_VALUE_ELEMENT powerconsumption_status_value_string_list[]= 
{ 
{ DPV_POWERCONSUMPTION_STATUS_EMPTY ,"Empty" }, // 247 
};
PARAM_VALUE_ELEMENT getcurrent_r_value_string_list[]= 
{ 
{ DPV_GETCURRENT_R_EMPTY ,"Empty" }, // 248 
};
PARAM_VALUE_ELEMENT getcurrent_g_value_string_list[]= 
{ 
{ DPV_GETCURRENT_G_EMPTY ,"Empty" }, // 249 
};
PARAM_VALUE_ELEMENT getcurrent_b_value_string_list[]= 
{ 
{ DPV_GETCURRENT_B_EMPTY ,"Empty" }, // 250 
};
PARAM_VALUE_ELEMENT getcurrent_y_value_string_list[]= 
{ 
{ DPV_GETCURRENT_Y_EMPTY ,"Empty" }, // 251 
};
PARAM_VALUE_ELEMENT getcurrent_all_value_string_list[]= 
{ 
{ DPV_GETCURRENT_ALL_EMPTY ,"Empty" }, // 252 
};
PARAM_VALUE_ELEMENT upgrade_dlpcompose_value_string_list[]= 
{ 
{ DPV_UPGRADE_DLPCOMPOSE_ON ,"On" }, // 253 
{ DPV_UPGRADE_DLPCOMPOSE_OFF ,"Off" }, // 254 
{ DPV_UPGRADE_DLPCOMPOSE_OK ,"Ok" }, // 255 
};
PARAM_VALUE_ELEMENT upgrade_dlpboot_value_string_list[]= 
{ 
{ DPV_UPGRADE_DLPBOOT_ON ,"On" }, // 256 
{ DPV_UPGRADE_DLPBOOT_OFF ,"Off" }, // 257 
{ DPV_UPGRADE_DLPBOOT_OK ,"Ok" }, // 258 
};
PARAM_VALUE_ELEMENT contrastenhance_status_value_string_list[]= 
{ 
{ DPV_CONTRASTENHANCE_STATUS_ON ,"On" }, // 259 
{ DPV_CONTRASTENHANCE_STATUS_OFF ,"Off" }, // 260 
{ DPV_CONTRASTENHANCE_STATUS_OK ,"Ok" }, // 261 
};
PARAM_VALUE_ELEMENT fpga_vx1function_value_string_list[]= 
{ 
{ DPV_FPGA_VX1FUNCTION_ON ,"On" }, // 262 
{ DPV_FPGA_VX1FUNCTION_OFF ,"Off" }, // 263 
};
PARAM_VALUE_ELEMENT fpga_videooutput_value_string_list[]= 
{ 
{ DPV_FPGA_VIDEOOUTPUT_ON ,"On" }, // 264 
{ DPV_FPGA_VIDEOOUTPUT_OFF ,"Off" }, // 265 
};
PARAM_VALUE_ELEMENT fpga_xpr_value_string_list[]= 
{ 
{ DPV_FPGA_XPR_ON_60HZ ,"On_60Hz" }, // 266 
{ DPV_FPGA_XPR_OFF_30HZ ,"Off_30Hz" }, // 267 
{ DPV_FPGA_XPR_OFF_120HZ ,"Off_120Hz" }, // 268 
{ DPV_FPGA_XPR_OFF_240HZ ,"Off_240Hz" }, // 269 
};
PARAM_VALUE_ELEMENT dlppowerstatus_status_value_string_list[]= 
{ 
{ DPV_DLPPOWERSTATUS_STATUS_ON ,"On" }, // 270 
{ DPV_DLPPOWERSTATUS_STATUS_OFF ,"Off" }, // 271 
{ DPV_DLPPOWERSTATUS_STATUS_RESET ,"Reset" }, // 272 
{ DPV_DLPPOWERSTATUS_STATUS_STANDBY ,"Standby" }, // 273 
{ DPV_DLPPOWERSTATUS_STATUS_ACTIVE ,"Active" }, // 274 
{ DPV_DLPPOWERSTATUS_STATUS_COOLING ,"Cooling" }, // 275 
{ DPV_DLPPOWERSTATUS_STATUS_WARMING ,"Warming" }, // 276 
{ DPV_DLPPOWERSTATUS_STATUS_POWERUP ,"Powerup" }, // 277 
{ DPV_DLPPOWERSTATUS_STATUS_OK ,"Ok" }, // 278 
};
PARAM_VALUE_ELEMENT edid_hdcpkey_value_string_list[]= 
{ 
{ DPV_EDID_HDCPKEY_EMPTY ,"Empty" }, // 279 
};
PARAM_VALUE_ELEMENT edid_hdmi_value_string_list[]= 
{ 
{ DPV_EDID_HDMI_EMPTY ,"Empty" }, // 280 
};
PARAM_VALUE_ELEMENT edid_vga_value_string_list[]= 
{ 
{ DPV_EDID_VGA_EMPTY ,"Empty" }, // 281 
};
PARAM_VALUE_ELEMENT edid_manufacturer_value_string_list[]= 
{ 
{ DPV_EDID_MANUFACTURER_EMPTY ,"Empty" }, // 282 
};
PARAM_VALUE_ELEMENT edid_productcode_value_string_list[]= 
{ 
{ DPV_EDID_PRODUCTCODE_EMPTY ,"Empty" }, // 283 
};
PARAM_VALUE_ELEMENT edid_monitorname_value_string_list[]= 
{ 
{ DPV_EDID_MONITORNAME_EMPTY ,"Empty" }, // 284 
};
PARAM_VALUE_ELEMENT network_ip_value_string_list[]= 
{ 
{ DPV_NETWORK_IP_EMPTY ,"Empty" }, // 285 
};
PARAM_VALUE_ELEMENT network_mask_value_string_list[]= 
{ 
{ DPV_NETWORK_MASK_EMPTY ,"Empty" }, // 286 
};
PARAM_VALUE_ELEMENT network_dns_value_string_list[]= 
{ 
{ DPV_NETWORK_DNS_EMPTY ,"Empty" }, // 287 
};
PARAM_VALUE_ELEMENT network_gateway_value_string_list[]= 
{ 
{ DPV_NETWORK_GATEWAY_EMPTY ,"Empty" }, // 288 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_brand_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_BRAND_EMPTY ,"Empty" }, // 289 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_sn_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_SN_EMPTY ,"Empty" }, // 290 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_ntcmask_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_NTCMASK_EMPTY ,"Empty" }, // 291 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_fanmask_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_FANMASK_EMPTY ,"Empty" }, // 292 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_logoindex_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_LOGOINDEX_EMPTY ,"Empty" }, // 293 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_keystoneenable_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_KEYSTONEENABLE_EMPTY ,"Empty" }, // 294 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_autoflipenable_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_AUTOFLIPENABLE_EMPTY ,"Empty" }, // 295 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_screendivisionenable_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_SCREENDIVISIONENABLE_EMPTY ,"Empty" }, // 296 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_defaultlanguage_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_DEFAULTLANGUAGE_EMPTY ,"Empty" }, // 297 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_productmodelnumber_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_PRODUCTMODELNUMBER_EMPTY ,"Empty" }, // 298 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_macaddress_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_MACADDRESS_EMPTY ,"Empty" }, // 299 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_standardcurrent_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_STANDARDCURRENT_EMPTY ,"Empty" }, // 300 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_moviecurrent_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_MOVIECURRENT_EMPTY ,"Empty" }, // 301 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_democurrent_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_DEMOCURRENT_EMPTY ,"Empty" }, // 302 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_ecocurrent_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_ECOCURRENT_EMPTY ,"Empty" }, // 303 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_dicomsimcurrent_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_DICOMSIMCURRENT_EMPTY ,"Empty" }, // 304 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_srgbcurrent_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_SRGBCURRENT_EMPTY ,"Empty" }, // 305 
};
PARAM_VALUE_ELEMENT dlpdefaultconfig_usercurrent_value_string_list[]= 
{ 
{ DPV_DLPDEFAULTCONFIG_USERCURRENT_EMPTY ,"Empty" }, // 306 
};
PARAM_VALUE_ELEMENT adcadjust_status_value_string_list[]= 
{ 
{ DPV_ADCADJUST_STATUS_ON ,"On" }, // 307 
{ DPV_ADCADJUST_STATUS_OFF ,"Off" }, // 308 
};
PARAM_VALUE_ELEMENT dlplogo_null_value_string_list[]= 
{ 
{ DPV_DLPLOGO_NULL_EMPTY ,"Empty" }, // 309 
};
PARAM_VALUE_ELEMENT motorzoom_front_value_string_list[]= 
{ 
{ DPV_MOTORZOOM_FRONT_EMPTY ,"Empty" }, // 310 
};
PARAM_VALUE_ELEMENT motorzoom_back_value_string_list[]= 
{ 
{ DPV_MOTORZOOM_BACK_EMPTY ,"Empty" }, // 311 
};
PARAM_VALUE_ELEMENT motorfocus_front_value_string_list[]= 
{ 
{ DPV_MOTORFOCUS_FRONT_EMPTY ,"Empty" }, // 312 
};
PARAM_VALUE_ELEMENT motorfocus_back_value_string_list[]= 
{ 
{ DPV_MOTORFOCUS_BACK_EMPTY ,"Empty" }, // 313 
};
PARAM_VALUE_ELEMENT motorshift_left_value_string_list[]= 
{ 
{ DPV_MOTORSHIFT_LEFT_EMPTY ,"Empty" }, // 314 
};
PARAM_VALUE_ELEMENT motorshift_right_value_string_list[]= 
{ 
{ DPV_MOTORSHIFT_RIGHT_EMPTY ,"Empty" }, // 315 
};
PARAM_VALUE_ELEMENT motorshift_up_value_string_list[]= 
{ 
{ DPV_MOTORSHIFT_UP_EMPTY ,"Empty" }, // 316 
};
PARAM_VALUE_ELEMENT motorshift_down_value_string_list[]= 
{ 
{ DPV_MOTORSHIFT_DOWN_EMPTY ,"Empty" }, // 317 
};
PARAM_VALUE_ELEMENT motorcenter_zoom_value_string_list[]= 
{ 
{ DPV_MOTORCENTER_ZOOM_EMPTY ,"Empty" }, // 318 
};
PARAM_VALUE_ELEMENT motorcenter_focus_value_string_list[]= 
{ 
{ DPV_MOTORCENTER_FOCUS_EMPTY ,"Empty" }, // 319 
};
PARAM_VALUE_ELEMENT motorcenter_shiftvertical_value_string_list[]= 
{ 
{ DPV_MOTORCENTER_SHIFTVERTICAL_EMPTY ,"Empty" }, // 320 
};
PARAM_VALUE_ELEMENT motorcenter_hifthorizontal_value_string_list[]= 
{ 
{ DPV_MOTORCENTER_HIFTHORIZONTAL_EMPTY ,"Empty" }, // 321 
};
PARAM_VALUE_ELEMENT motorcenter_all_value_string_list[]= 
{ 
{ DPV_MOTORCENTER_ALL_EMPTY ,"Empty" }, // 322 
};
PARAM_VALUE_ELEMENT dlpsource_external_value_string_list[]= 
{ 
{ DPV_DLPSOURCE_EXTERNAL_EXTERNAL ,"External" }, // 323 
};
PARAM_VALUE_ELEMENT dlpsource_inside_value_string_list[]= 
{ 
{ DPV_DLPSOURCE_INSIDE_SOLIDFIELD ,"SolidField" }, // 324 
{ DPV_DLPSOURCE_INSIDE_SPLASH ,"Splash" }, // 325 
};
PARAM_VALUE_ELEMENT type3d_status_value_string_list[]= 
{ 
{ DPV_TYPE3D_STATUS_DLPLINK3D ,"DLPLink3D" }, // 326 
{ DPV_TYPE3D_STATUS_IR3D ,"IR3D" }, // 327 
};
PARAM_VALUE_ELEMENT imagefreeze_freeze_value_string_list[]= 
{ 
{ DPV_IMAGEFREEZE_FREEZE_EMPTY ,"Empty" }, // 328 
};
PARAM_VALUE_ELEMENT imagefreeze_unfreeze_value_string_list[]= 
{ 
{ DPV_IMAGEFREEZE_UNFREEZE_EMPTY ,"Empty" }, // 329 
};
PARAM_VALUE_ELEMENT fpgapatternsize_status_value_string_list[]= 
{ 
{ DPV_FPGAPATTERNSIZE_STATUS_EMPTY ,"Empty" }, // 330 
};
PARAM_VALUE_ELEMENT projectionmode1_status_value_string_list[]= 
{ 
{ DPV_PROJECTIONMODE1_STATUS_HIGHLIGHT ,"HighLight" }, // 331 
{ DPV_PROJECTIONMODE1_STATUS_MOVIE ,"Movie" }, // 332 
{ DPV_PROJECTIONMODE1_STATUS_STANDARD ,"Standard" }, // 333 
{ DPV_PROJECTIONMODE1_STATUS_POWERSAVE ,"PowerSave" }, // 334 
{ DPV_PROJECTIONMODE1_STATUS_INVILID ,"Invilid" }, // 335 
};
PARAM_KEY_ELEMENT system_key_string_list[]=
{ 
{ DPK_SYSTEM_STATUS,VALUE_TYPE_ENUM, system_status_value_string_list,10,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT lightsource_key_string_list[]=
{ 
{ DPK_LIGHTSOURCE_STATUS,VALUE_TYPE_ENUM, lightsource_status_value_string_list,3,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT version_key_string_list[]=
{ 
{ DPK_VERSION_DLPSOFTWARE,VALUE_TYPE_STRING, version_dlpsoftware_value_string_list,1,"DlpSoftware"  }, // 1 
{ DPK_VERSION_PMUSOFTWARE,VALUE_TYPE_STRING, version_pmusoftware_value_string_list,1,"PmuSoftware"  }, // 2 
{ DPK_VERSION_IDUSOFTWARE,VALUE_TYPE_STRING, version_idusoftware_value_string_list,1,"IduSoftware"  }, // 3 
{ DPK_VERSION_DLPHARDWARE,VALUE_TYPE_STRING, version_dlphardware_value_string_list,1,"DlpHardware"  }, // 4 
{ DPK_VERSION_PMUHARDWARE,VALUE_TYPE_STRING, version_pmuhardware_value_string_list,1,"PmuHardware"  }, // 5 
{ DPK_VERSION_IDUHARDWARE,VALUE_TYPE_STRING, version_iduhardware_value_string_list,1,"IduHardware"  }, // 6 
{ DPK_VERSION_LENSSOFTWARE,VALUE_TYPE_STRING, version_lenssoftware_value_string_list,1,"LensSoftware"  }, // 7 
{ DPK_VERSION_LENSHARDWARE,VALUE_TYPE_STRING, version_lenshardware_value_string_list,1,"LensHardware"  }, // 8 
};
PARAM_KEY_ELEMENT sn_key_string_list[]=
{ 
{ DPK_SN_SOURCELIGHT,VALUE_TYPE_STRING, sn_sourcelight_value_string_list,1,"SourceLight"  }, // 1 
{ DPK_SN_PROJECTOR,VALUE_TYPE_STRING, sn_projector_value_string_list,1,"Projector"  }, // 2 
{ DPK_SN_LIGHTENGINE,VALUE_TYPE_STRING, sn_lightengine_value_string_list,1,"LightEngine"  }, // 3 
};
PARAM_KEY_ELEMENT lightsourcetime_key_string_list[]=
{ 
{ DPK_LIGHTSOURCETIME_MINUTE,VALUE_TYPE_INT, lightsourcetime_minute_value_string_list,1,"Minute"  }, // 1 
};
PARAM_KEY_ELEMENT gamma_key_string_list[]=
{ 
{ DPK_GAMMA_STATUS,VALUE_TYPE_INT, gamma_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT installationmode_key_string_list[]=
{ 
{ DPK_INSTALLATIONMODE_STATUS,VALUE_TYPE_ENUM, installationmode_status_value_string_list,5,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT verticalkeystone_key_string_list[]=
{ 
{ DPK_VERTICALKEYSTONE_STATUS,VALUE_TYPE_INT, verticalkeystone_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT horizonkeystone_key_string_list[]=
{ 
{ DPK_HORIZONKEYSTONE_STATUS,VALUE_TYPE_INT, horizonkeystone_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT keystone2d_key_string_list[]=
{ 
{ DPK_KEYSTONE2D_TOPLEFTX,VALUE_TYPE_INT, keystone2d_topleftx_value_string_list,1,"TopLeftX"  }, // 1 
{ DPK_KEYSTONE2D_TOPLEFTY,VALUE_TYPE_INT, keystone2d_toplefty_value_string_list,1,"TopLeftY"  }, // 2 
{ DPK_KEYSTONE2D_TOPRIGHTX,VALUE_TYPE_INT, keystone2d_toprightx_value_string_list,1,"TopRightX"  }, // 3 
{ DPK_KEYSTONE2D_TOPRIGHTY,VALUE_TYPE_INT, keystone2d_toprighty_value_string_list,1,"TopRightY"  }, // 4 
{ DPK_KEYSTONE2D_BOTTOMLEFTX,VALUE_TYPE_INT, keystone2d_bottomleftx_value_string_list,1,"BottomLeftX"  }, // 5 
{ DPK_KEYSTONE2D_BOTTOMLEFTY,VALUE_TYPE_INT, keystone2d_bottomlefty_value_string_list,1,"BottomLeftY"  }, // 6 
{ DPK_KEYSTONE2D_BOTTOMRIGHTX,VALUE_TYPE_INT, keystone2d_bottomrightx_value_string_list,1,"BottomRightX"  }, // 7 
{ DPK_KEYSTONE2D_BOTTOMRIGHTY,VALUE_TYPE_INT, keystone2d_bottomrighty_value_string_list,1,"BottomRightY"  }, // 8 
};
PARAM_KEY_ELEMENT testpattern_key_string_list[]=
{ 
{ DPK_TESTPATTERN_STATUS,VALUE_TYPE_ENUM, testpattern_status_value_string_list,22,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT mode3d_key_string_list[]=
{ 
{ DPK_MODE3D_MODE3D,VALUE_TYPE_ENUM, mode3d_mode3d_value_string_list,8,"Mode3D"  }, // 1 
};
PARAM_KEY_ELEMENT syncflip3d_key_string_list[]=
{ 
{ DPK_SYNCFLIP3D_STATUS,VALUE_TYPE_ENUM, syncflip3d_status_value_string_list,3,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT projectionmode_key_string_list[]=
{ 
{ DPK_PROJECTIONMODE_STATUS,VALUE_TYPE_ENUM, projectionmode_status_value_string_list,6,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT fanstatus_key_string_list[]=
{ 
{ DPK_FANSTATUS_FAN1,VALUE_TYPE_INT, fanstatus_fan1_value_string_list,1,"Fan1"  }, // 1 
{ DPK_FANSTATUS_FAN2,VALUE_TYPE_INT, fanstatus_fan2_value_string_list,1,"Fan2"  }, // 2 
{ DPK_FANSTATUS_FAN3,VALUE_TYPE_INT, fanstatus_fan3_value_string_list,1,"Fan3"  }, // 3 
{ DPK_FANSTATUS_FAN4,VALUE_TYPE_INT, fanstatus_fan4_value_string_list,1,"Fan4"  }, // 4 
{ DPK_FANSTATUS_FAN5,VALUE_TYPE_INT, fanstatus_fan5_value_string_list,1,"Fan5"  }, // 5 
{ DPK_FANSTATUS_FAN6,VALUE_TYPE_INT, fanstatus_fan6_value_string_list,1,"Fan6"  }, // 6 
{ DPK_FANSTATUS_FAN7,VALUE_TYPE_INT, fanstatus_fan7_value_string_list,1,"Fan7"  }, // 7 
{ DPK_FANSTATUS_FAN8,VALUE_TYPE_INT, fanstatus_fan8_value_string_list,1,"Fan8"  }, // 8 
{ DPK_FANSTATUS_FAN9,VALUE_TYPE_INT, fanstatus_fan9_value_string_list,1,"Fan9"  }, // 9 
{ DPK_FANSTATUS_FAN10,VALUE_TYPE_INT, fanstatus_fan10_value_string_list,1,"Fan10"  }, // 10 
{ DPK_FANSTATUS_FAN11,VALUE_TYPE_INT, fanstatus_fan11_value_string_list,1,"Fan11"  }, // 11 
{ DPK_FANSTATUS_FAN12,VALUE_TYPE_INT, fanstatus_fan12_value_string_list,1,"Fan12"  }, // 12 
{ DPK_FANSTATUS_FAN13,VALUE_TYPE_INT, fanstatus_fan13_value_string_list,1,"Fan13"  }, // 13 
{ DPK_FANSTATUS_FAN14,VALUE_TYPE_INT, fanstatus_fan14_value_string_list,1,"Fan14"  }, // 14 
{ DPK_FANSTATUS_FAN15,VALUE_TYPE_INT, fanstatus_fan15_value_string_list,1,"Fan15"  }, // 15 
{ DPK_FANSTATUS_FAN16,VALUE_TYPE_INT, fanstatus_fan16_value_string_list,1,"Fan16"  }, // 16 
{ DPK_FANSTATUS_FAN17,VALUE_TYPE_INT, fanstatus_fan17_value_string_list,1,"Fan17"  }, // 17 
{ DPK_FANSTATUS_FAN18,VALUE_TYPE_INT, fanstatus_fan18_value_string_list,1,"Fan18"  }, // 18 
{ DPK_FANSTATUS_FAN19,VALUE_TYPE_INT, fanstatus_fan19_value_string_list,1,"Fan19"  }, // 19 
{ DPK_FANSTATUS_FAN20,VALUE_TYPE_INT, fanstatus_fan20_value_string_list,1,"Fan20"  }, // 20 
{ DPK_FANSTATUS_FAN21,VALUE_TYPE_INT, fanstatus_fan21_value_string_list,1,"Fan21"  }, // 21 
{ DPK_FANSTATUS_FAN22,VALUE_TYPE_INT, fanstatus_fan22_value_string_list,1,"Fan22"  }, // 22 
{ DPK_FANSTATUS_FAN23,VALUE_TYPE_INT, fanstatus_fan23_value_string_list,1,"Fan23"  }, // 23 
{ DPK_FANSTATUS_FAN24,VALUE_TYPE_INT, fanstatus_fan24_value_string_list,1,"Fan24"  }, // 24 
{ DPK_FANSTATUS_FAN25,VALUE_TYPE_INT, fanstatus_fan25_value_string_list,1,"Fan25"  }, // 25 
{ DPK_FANSTATUS_FAN26,VALUE_TYPE_INT, fanstatus_fan26_value_string_list,1,"Fan26"  }, // 26 
{ DPK_FANSTATUS_FAN27,VALUE_TYPE_INT, fanstatus_fan27_value_string_list,1,"Fan27"  }, // 27 
{ DPK_FANSTATUS_ALL,VALUE_TYPE_INT, fanstatus_all_value_string_list,1,"ALL"  }, // 28 
};
PARAM_KEY_ELEMENT setfan_key_string_list[]=
{ 
{ DPK_SETFAN_FAN1,VALUE_TYPE_INT, setfan_fan1_value_string_list,1,"Fan1"  }, // 1 
{ DPK_SETFAN_FAN2,VALUE_TYPE_INT, setfan_fan2_value_string_list,1,"Fan2"  }, // 2 
{ DPK_SETFAN_FAN3,VALUE_TYPE_INT, setfan_fan3_value_string_list,1,"Fan3"  }, // 3 
{ DPK_SETFAN_FAN4,VALUE_TYPE_INT, setfan_fan4_value_string_list,1,"Fan4"  }, // 4 
{ DPK_SETFAN_FAN5,VALUE_TYPE_INT, setfan_fan5_value_string_list,1,"Fan5"  }, // 5 
{ DPK_SETFAN_FAN6,VALUE_TYPE_INT, setfan_fan6_value_string_list,1,"Fan6"  }, // 6 
{ DPK_SETFAN_FAN7,VALUE_TYPE_INT, setfan_fan7_value_string_list,1,"Fan7"  }, // 7 
{ DPK_SETFAN_FAN8,VALUE_TYPE_INT, setfan_fan8_value_string_list,1,"Fan8"  }, // 8 
{ DPK_SETFAN_FAN9,VALUE_TYPE_INT, setfan_fan9_value_string_list,1,"Fan9"  }, // 9 
{ DPK_SETFAN_FAN10,VALUE_TYPE_INT, setfan_fan10_value_string_list,1,"Fan10"  }, // 10 
{ DPK_SETFAN_FAN11,VALUE_TYPE_INT, setfan_fan11_value_string_list,1,"Fan11"  }, // 11 
{ DPK_SETFAN_FAN12,VALUE_TYPE_INT, setfan_fan12_value_string_list,1,"Fan12"  }, // 12 
{ DPK_SETFAN_FAN13,VALUE_TYPE_INT, setfan_fan13_value_string_list,1,"Fan13"  }, // 13 
{ DPK_SETFAN_FAN14,VALUE_TYPE_INT, setfan_fan14_value_string_list,1,"Fan14"  }, // 14 
{ DPK_SETFAN_FAN15,VALUE_TYPE_INT, setfan_fan15_value_string_list,1,"Fan15"  }, // 15 
{ DPK_SETFAN_FAN16,VALUE_TYPE_INT, setfan_fan16_value_string_list,1,"Fan16"  }, // 16 
{ DPK_SETFAN_FAN17,VALUE_TYPE_INT, setfan_fan17_value_string_list,1,"Fan17"  }, // 17 
{ DPK_SETFAN_FAN18,VALUE_TYPE_INT, setfan_fan18_value_string_list,1,"Fan18"  }, // 18 
{ DPK_SETFAN_FAN19,VALUE_TYPE_INT, setfan_fan19_value_string_list,1,"Fan19"  }, // 19 
{ DPK_SETFAN_FAN20,VALUE_TYPE_INT, setfan_fan20_value_string_list,1,"Fan20"  }, // 20 
{ DPK_SETFAN_FAN21,VALUE_TYPE_INT, setfan_fan21_value_string_list,1,"Fan21"  }, // 21 
{ DPK_SETFAN_FAN22,VALUE_TYPE_INT, setfan_fan22_value_string_list,1,"Fan22"  }, // 22 
{ DPK_SETFAN_FAN23,VALUE_TYPE_INT, setfan_fan23_value_string_list,1,"Fan23"  }, // 23 
{ DPK_SETFAN_FAN24,VALUE_TYPE_INT, setfan_fan24_value_string_list,1,"Fan24"  }, // 24 
{ DPK_SETFAN_FAN25,VALUE_TYPE_INT, setfan_fan25_value_string_list,1,"Fan25"  }, // 25 
{ DPK_SETFAN_FAN26,VALUE_TYPE_INT, setfan_fan26_value_string_list,1,"Fan26"  }, // 26 
{ DPK_SETFAN_FAN27,VALUE_TYPE_INT, setfan_fan27_value_string_list,1,"Fan27"  }, // 27 
{ DPK_SETFAN_ALL,VALUE_TYPE_INT, setfan_all_value_string_list,1,"ALL"  }, // 28 
};
PARAM_KEY_ELEMENT temperature_key_string_list[]=
{ 
{ DPK_TEMPERATURE_NTCDMD1,VALUE_TYPE_INT, temperature_ntcdmd1_value_string_list,1,"NtcDmd1"  }, // 1 
{ DPK_TEMPERATURE_NTCCW1,VALUE_TYPE_INT, temperature_ntccw1_value_string_list,1,"NtcCw1"  }, // 2 
{ DPK_TEMPERATURE_NTCBLUELASER1,VALUE_TYPE_INT, temperature_ntcbluelaser1_value_string_list,1,"NtcBlueLaser1"  }, // 3 
{ DPK_TEMPERATURE_NTCBLUELASER2,VALUE_TYPE_INT, temperature_ntcbluelaser2_value_string_list,1,"NtcBlueLaser2"  }, // 4 
{ DPK_TEMPERATURE_NTCXPR1,VALUE_TYPE_INT, temperature_ntcxpr1_value_string_list,1,"NtcXpr1"  }, // 5 
{ DPK_TEMPERATURE_NTCENV1,VALUE_TYPE_INT, temperature_ntcenv1_value_string_list,1,"NtcEnv1"  }, // 6 
{ DPK_TEMPERATURE_NTCPOWERSUPPLY,VALUE_TYPE_INT, temperature_ntcpowersupply_value_string_list,1,"NtcPowerSupply"  }, // 7 
};
PARAM_KEY_ELEMENT cwspeed_key_string_list[]=
{ 
{ DPK_CWSPEED_STATUS,VALUE_TYPE_INT, cwspeed_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT xprfixed_key_string_list[]=
{ 
{ DPK_XPRFIXED_STATUS,VALUE_TYPE_ENUM, xprfixed_status_value_string_list,5,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT reset_key_string_list[]=
{ 
{ DPK_RESET_STATUS,VALUE_TYPE_ENUM, reset_status_value_string_list,4,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT signalchannel_key_string_list[]=
{ 
{ DPK_SIGNALCHANNEL_NONE,VALUE_TYPE_INT, signalchannel_none_value_string_list,1,"None"  }, // 1 
{ DPK_SIGNALCHANNEL_HDMI,VALUE_TYPE_INT, signalchannel_hdmi_value_string_list,1,"HDMI"  }, // 2 
{ DPK_SIGNALCHANNEL_VGA,VALUE_TYPE_INT, signalchannel_vga_value_string_list,1,"VGA"  }, // 3 
{ DPK_SIGNALCHANNEL_CDS,VALUE_TYPE_INT, signalchannel_cds_value_string_list,1,"CDS"  }, // 4 
{ DPK_SIGNALCHANNEL_DVI,VALUE_TYPE_INT, signalchannel_dvi_value_string_list,1,"DVI"  }, // 5 
{ DPK_SIGNALCHANNEL_YCBCR,VALUE_TYPE_INT, signalchannel_ycbcr_value_string_list,1,"YcBcR"  }, // 6 
{ DPK_SIGNALCHANNEL_SDI,VALUE_TYPE_INT, signalchannel_sdi_value_string_list,1,"SDI"  }, // 7 
};
PARAM_KEY_ELEMENT aspectratio_key_string_list[]=
{ 
{ DPK_ASPECTRATIO_STATUS,VALUE_TYPE_ENUM, aspectratio_status_value_string_list,7,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT stanbytimes_key_string_list[]=
{ 
{ DPK_STANBYTIMES_STATUS,VALUE_TYPE_INT, stanbytimes_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT heartbeat_key_string_list[]=
{ 
{ DPK_HEARTBEAT_STATUS,VALUE_TYPE_INT, heartbeat_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT eeprom_key_string_list[]=
{ 
{ DPK_EEPROM_DLPVENDORNAME,VALUE_TYPE_STRING, eeprom_dlpvendorname_value_string_list,1,"DlpVendorName"  }, // 1 
{ DPK_EEPROM_COLORWHEELDELAY,VALUE_TYPE_INT, eeprom_colorwheeldelay_value_string_list,1,"ColorWheelDelay"  }, // 2 
{ DPK_EEPROM_PROJECTIONMODEHIGHLIGHT,VALUE_TYPE_STRING, eeprom_projectionmodehighlight_value_string_list,1,"ProjectionModeHighLight"  }, // 3 
{ DPK_EEPROM_PROJECTIONMODEMOVIE,VALUE_TYPE_STRING, eeprom_projectionmodemovie_value_string_list,1,"ProjectionModeMovie"  }, // 4 
{ DPK_EEPROM_PROJECTIONMODESTADARD,VALUE_TYPE_STRING, eeprom_projectionmodestadard_value_string_list,1,"ProjectionModeStadard"  }, // 5 
{ DPK_EEPROM_PROJECTIONMODEPOWERSAVE,VALUE_TYPE_STRING, eeprom_projectionmodepowersave_value_string_list,1,"ProjectionModePowersave"  }, // 6 
{ DPK_EEPROM_PROJECTIONMODEHIGHLIGHT1,VALUE_TYPE_STRING, eeprom_projectionmodehighlight1_value_string_list,1,"ProjectionModeHighLight1"  }, // 7 
{ DPK_EEPROM_PROJECTIONMODEMOVIE1,VALUE_TYPE_STRING, eeprom_projectionmodemovie1_value_string_list,1,"ProjectionModeMovie1"  }, // 8 
{ DPK_EEPROM_PROJECTIONMODESTADARD1,VALUE_TYPE_STRING, eeprom_projectionmodestadard1_value_string_list,1,"ProjectionModeStadard1"  }, // 9 
{ DPK_EEPROM_PROJECTIONMODEPOWERSAVE1,VALUE_TYPE_STRING, eeprom_projectionmodepowersave1_value_string_list,1,"ProjectionModePowersave1"  }, // 10 
{ DPK_EEPROM_COLORWHEELMODE,VALUE_TYPE_STRING, eeprom_colorwheelmode_value_string_list,1,"ColorWheelMode"  }, // 11 
{ DPK_EEPROM_COLORWHEELSPOKE,VALUE_TYPE_STRING, eeprom_colorwheelspoke_value_string_list,1,"ColorWheelSpoke"  }, // 12 
{ DPK_EEPROM_COLORWHEELID,VALUE_TYPE_INT, eeprom_colorwheelid_value_string_list,1,"ColorWheelId"  }, // 13 
{ DPK_EEPROM_PRODUCTIONWEEK,VALUE_TYPE_STRING, eeprom_productionweek_value_string_list,1,"ProductionWeek"  }, // 14 
{ DPK_EEPROM_BRIGHTNESS,VALUE_TYPE_STRING, eeprom_brightness_value_string_list,1,"Brightness"  }, // 15 
{ DPK_EEPROM_COLORCOORDINATEVALUE,VALUE_TYPE_STRING, eeprom_colorcoordinatevalue_value_string_list,1,"ColorCoordinateValue"  }, // 16 
{ DPK_EEPROM_CONTRASTRATIO,VALUE_TYPE_STRING, eeprom_contrastratio_value_string_list,1,"ContrastRatio"  }, // 17 
{ DPK_EEPROM_BRIGHTNESSUNIFORMITY,VALUE_TYPE_STRING, eeprom_brightnessuniformity_value_string_list,1,"BrightnessUniformity"  }, // 18 
{ DPK_EEPROM_SERIALNUMBER,VALUE_TYPE_STRING, eeprom_serialnumber_value_string_list,1,"SerialNumber"  }, // 19 
{ DPK_EEPROM_BRIGHTNESMODE1,VALUE_TYPE_STRING, eeprom_brightnesmode1_value_string_list,1,"BrightnesMode1"  }, // 20 
{ DPK_EEPROM_BRIGHTNESMODE2,VALUE_TYPE_STRING, eeprom_brightnesmode2_value_string_list,1,"BrightnesMode2"  }, // 21 
{ DPK_EEPROM_BRIGHTNESMODE3,VALUE_TYPE_STRING, eeprom_brightnesmode3_value_string_list,1,"BrightnesMode3"  }, // 22 
{ DPK_EEPROM_BRIGHTNESMODE4,VALUE_TYPE_STRING, eeprom_brightnesmode4_value_string_list,1,"BrightnesMode4"  }, // 23 
{ DPK_EEPROM_BRIGHTNESMODE5,VALUE_TYPE_STRING, eeprom_brightnesmode5_value_string_list,1,"BrightnesMode5"  }, // 24 
{ DPK_EEPROM_BRIGHTNESMODE6,VALUE_TYPE_STRING, eeprom_brightnesmode6_value_string_list,1,"BrightnesMode6"  }, // 25 
{ DPK_EEPROM_BRIGHTNESMODE7,VALUE_TYPE_STRING, eeprom_brightnesmode7_value_string_list,1,"BrightnesMode7"  }, // 26 
{ DPK_EEPROM_BRIGHTNESMODE8,VALUE_TYPE_STRING, eeprom_brightnesmode8_value_string_list,1,"BrightnesMode8"  }, // 27 
{ DPK_EEPROM_DLPVERSION,VALUE_TYPE_STRING, eeprom_dlpversion_value_string_list,1,"DlpVersion"  }, // 28 
{ DPK_EEPROM_TEMLOOKUPTABLE1,VALUE_TYPE_STRING, eeprom_temlookuptable1_value_string_list,1,"TemLookupTable1"  }, // 29 
{ DPK_EEPROM_TEMLOOKUPTABLE2,VALUE_TYPE_STRING, eeprom_temlookuptable2_value_string_list,1,"TemLookupTable2"  }, // 30 
{ DPK_EEPROM_TEMLOOKUPTABLE3,VALUE_TYPE_STRING, eeprom_temlookuptable3_value_string_list,1,"TemLookupTable3"  }, // 31 
{ DPK_EEPROM_TEMLOOKUPTABLE4,VALUE_TYPE_STRING, eeprom_temlookuptable4_value_string_list,1,"TemLookupTable4"  }, // 32 
{ DPK_EEPROM_TEMLOOKUPTABLE5,VALUE_TYPE_STRING, eeprom_temlookuptable5_value_string_list,1,"TemLookupTable5"  }, // 33 
{ DPK_EEPROM_CURRENTINFO1,VALUE_TYPE_STRING, eeprom_currentinfo1_value_string_list,1,"CurrentInfo1"  }, // 34 
{ DPK_EEPROM_CURRENTINFO2,VALUE_TYPE_STRING, eeprom_currentinfo2_value_string_list,1,"CurrentInfo2"  }, // 35 
{ DPK_EEPROM_CURRENTINFO3,VALUE_TYPE_STRING, eeprom_currentinfo3_value_string_list,1,"CurrentInfo3"  }, // 36 
{ DPK_EEPROM_CURRENTINFO4,VALUE_TYPE_STRING, eeprom_currentinfo4_value_string_list,1,"CurrentInfo4"  }, // 37 
{ DPK_EEPROM_CURRENTINFO5,VALUE_TYPE_STRING, eeprom_currentinfo5_value_string_list,1,"CurrentInfo5"  }, // 38 
{ DPK_EEPROM_CURRENTINFO6,VALUE_TYPE_STRING, eeprom_currentinfo6_value_string_list,1,"CurrentInfo6"  }, // 39 
{ DPK_EEPROM_CURRENTINFO7,VALUE_TYPE_STRING, eeprom_currentinfo7_value_string_list,1,"CurrentInfo7"  }, // 40 
{ DPK_EEPROM_CURRENTINFO8,VALUE_TYPE_STRING, eeprom_currentinfo8_value_string_list,1,"CurrentInfo8"  }, // 41 
{ DPK_EEPROM_CWID,VALUE_TYPE_STRING, eeprom_cwid_value_string_list,1,"CwId"  }, // 42 
{ DPK_EEPROM_XPRID,VALUE_TYPE_STRING, eeprom_xprid_value_string_list,1,"XprId"  }, // 43 
{ DPK_EEPROM_XPRGAIN,VALUE_TYPE_STRING, eeprom_xprgain_value_string_list,1,"XprGain"  }, // 44 
{ DPK_EEPROM_XPRSL,VALUE_TYPE_STRING, eeprom_xprsl_value_string_list,1,"XprSl"  }, // 45 
{ DPK_EEPROM_KEYSTONEOFFSET,VALUE_TYPE_STRING, eeprom_keystoneoffset_value_string_list,1,"KeystoneOffset"  }, // 46 
{ DPK_EEPROM_KEYSTONETR,VALUE_TYPE_STRING, eeprom_keystonetr_value_string_list,1,"KeystoneTr"  }, // 47 
{ DPK_EEPROM_INSTALLATIONMODE,VALUE_TYPE_STRING, eeprom_installationmode_value_string_list,1,"InstallationMode"  }, // 48 
};
PARAM_KEY_ELEMENT help_key_string_list[]=
{ 
{ DPK_HELP_STATUS,VALUE_TYPE_STRING, help_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT setcurrent_key_string_list[]=
{ 
{ DPK_SETCURRENT_R,VALUE_TYPE_INT, setcurrent_r_value_string_list,1,"R"  }, // 1 
{ DPK_SETCURRENT_G,VALUE_TYPE_INT, setcurrent_g_value_string_list,1,"G"  }, // 2 
{ DPK_SETCURRENT_B,VALUE_TYPE_INT, setcurrent_b_value_string_list,1,"B"  }, // 3 
{ DPK_SETCURRENT_Y,VALUE_TYPE_INT, setcurrent_y_value_string_list,1,"Y"  }, // 4 
{ DPK_SETCURRENT_R1,VALUE_TYPE_INT, setcurrent_r1_value_string_list,1,"R1"  }, // 5 
{ DPK_SETCURRENT_G1,VALUE_TYPE_INT, setcurrent_g1_value_string_list,1,"G1"  }, // 6 
{ DPK_SETCURRENT_B1,VALUE_TYPE_INT, setcurrent_b1_value_string_list,1,"B1"  }, // 7 
{ DPK_SETCURRENT_Y1,VALUE_TYPE_INT, setcurrent_y1_value_string_list,1,"Y1"  }, // 8 
{ DPK_SETCURRENT_ALL,VALUE_TYPE_INT, setcurrent_all_value_string_list,1,"ALL"  }, // 9 
};
PARAM_KEY_ELEMENT error_key_string_list[]=
{ 
{ DPK_ERROR_STATUS,VALUE_TYPE_INT, error_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT warning_key_string_list[]=
{ 
{ DPK_WARNING_STATUS,VALUE_TYPE_INT, warning_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT loginfo_key_string_list[]=
{ 
{ DPK_LOGINFO_STATUS,VALUE_TYPE_STRING, loginfo_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT productinformation_key_string_list[]=
{ 
{ DPK_PRODUCTINFORMATION_MODELNAME,VALUE_TYPE_STRING, productinformation_modelname_value_string_list,1,"ModelName"  }, // 1 
{ DPK_PRODUCTINFORMATION_BRANDNAME,VALUE_TYPE_STRING, productinformation_brandname_value_string_list,1,"BrandName"  }, // 2 
{ DPK_PRODUCTINFORMATION_MACHINESN,VALUE_TYPE_STRING, productinformation_machinesn_value_string_list,1,"MachineSn"  }, // 3 
};
PARAM_KEY_ELEMENT colorwheeldelay_key_string_list[]=
{ 
{ DPK_COLORWHEELDELAY_STATUS,VALUE_TYPE_INT, colorwheeldelay_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT brilliantcolor_key_string_list[]=
{ 
{ DPK_BRILLIANTCOLOR_STATUS,VALUE_TYPE_ENUM, brilliantcolor_status_value_string_list,3,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT standbymode_key_string_list[]=
{ 
{ DPK_STANDBYMODE_STATUS,VALUE_TYPE_ENUM, standbymode_status_value_string_list,3,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT volume_key_string_list[]=
{ 
{ DPK_VOLUME_STATUS,VALUE_TYPE_INT, volume_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT volumecontrol_key_string_list[]=
{ 
{ DPK_VOLUMECONTROL_STATUS,VALUE_TYPE_ENUM, volumecontrol_status_value_string_list,4,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT runtime_key_string_list[]=
{ 
{ DPK_RUNTIME_STATUS,VALUE_TYPE_INT, runtime_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT powerconsumption_key_string_list[]=
{ 
{ DPK_POWERCONSUMPTION_STATUS,VALUE_TYPE_INT, powerconsumption_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT getcurrent_key_string_list[]=
{ 
{ DPK_GETCURRENT_R,VALUE_TYPE_INT, getcurrent_r_value_string_list,1,"R"  }, // 1 
{ DPK_GETCURRENT_G,VALUE_TYPE_INT, getcurrent_g_value_string_list,1,"G"  }, // 2 
{ DPK_GETCURRENT_B,VALUE_TYPE_INT, getcurrent_b_value_string_list,1,"B"  }, // 3 
{ DPK_GETCURRENT_Y,VALUE_TYPE_INT, getcurrent_y_value_string_list,1,"Y"  }, // 4 
{ DPK_GETCURRENT_ALL,VALUE_TYPE_INT, getcurrent_all_value_string_list,1,"ALL"  }, // 5 
};
PARAM_KEY_ELEMENT upgrade_key_string_list[]=
{ 
{ DPK_UPGRADE_DLPCOMPOSE,VALUE_TYPE_ENUM, upgrade_dlpcompose_value_string_list,3,"DlpCompose"  }, // 1 
{ DPK_UPGRADE_DLPBOOT,VALUE_TYPE_ENUM, upgrade_dlpboot_value_string_list,3,"DlpBoot"  }, // 2 
};
PARAM_KEY_ELEMENT contrastenhance_key_string_list[]=
{ 
{ DPK_CONTRASTENHANCE_STATUS,VALUE_TYPE_ENUM, contrastenhance_status_value_string_list,3,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT fpga_key_string_list[]=
{ 
{ DPK_FPGA_VX1FUNCTION,VALUE_TYPE_ENUM, fpga_vx1function_value_string_list,2,"Vx1Function"  }, // 1 
{ DPK_FPGA_VIDEOOUTPUT,VALUE_TYPE_ENUM, fpga_videooutput_value_string_list,2,"VideoOutput"  }, // 2 
{ DPK_FPGA_XPR,VALUE_TYPE_ENUM, fpga_xpr_value_string_list,4,"Xpr"  }, // 3 
};
PARAM_KEY_ELEMENT dlppowerstatus_key_string_list[]=
{ 
{ DPK_DLPPOWERSTATUS_STATUS,VALUE_TYPE_ENUM, dlppowerstatus_status_value_string_list,9,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT edid_key_string_list[]=
{ 
{ DPK_EDID_HDCPKEY,VALUE_TYPE_STRING, edid_hdcpkey_value_string_list,1,"HdcpKey"  }, // 1 
{ DPK_EDID_HDMI,VALUE_TYPE_STRING, edid_hdmi_value_string_list,1,"Hdmi"  }, // 2 
{ DPK_EDID_VGA,VALUE_TYPE_STRING, edid_vga_value_string_list,1,"Vga"  }, // 3 
{ DPK_EDID_MANUFACTURER,VALUE_TYPE_STRING, edid_manufacturer_value_string_list,1,"Manufacturer"  }, // 4 
{ DPK_EDID_PRODUCTCODE,VALUE_TYPE_STRING, edid_productcode_value_string_list,1,"ProductCode"  }, // 5 
{ DPK_EDID_MONITORNAME,VALUE_TYPE_STRING, edid_monitorname_value_string_list,1,"MonitorName"  }, // 6 
};
PARAM_KEY_ELEMENT network_key_string_list[]=
{ 
{ DPK_NETWORK_IP,VALUE_TYPE_STRING, network_ip_value_string_list,1,"Ip"  }, // 1 
{ DPK_NETWORK_MASK,VALUE_TYPE_STRING, network_mask_value_string_list,1,"Mask"  }, // 2 
{ DPK_NETWORK_DNS,VALUE_TYPE_STRING, network_dns_value_string_list,1,"Dns"  }, // 3 
{ DPK_NETWORK_GATEWAY,VALUE_TYPE_STRING, network_gateway_value_string_list,1,"GateWay"  }, // 4 
};
PARAM_KEY_ELEMENT dlpdefaultconfig_key_string_list[]=
{ 
{ DPK_DLPDEFAULTCONFIG_BRAND,VALUE_TYPE_STRING, dlpdefaultconfig_brand_value_string_list,1,"Brand"  }, // 1 
{ DPK_DLPDEFAULTCONFIG_SN,VALUE_TYPE_STRING, dlpdefaultconfig_sn_value_string_list,1,"Sn"  }, // 2 
{ DPK_DLPDEFAULTCONFIG_NTCMASK,VALUE_TYPE_STRING, dlpdefaultconfig_ntcmask_value_string_list,1,"NtcMask"  }, // 3 
{ DPK_DLPDEFAULTCONFIG_FANMASK,VALUE_TYPE_STRING, dlpdefaultconfig_fanmask_value_string_list,1,"FanMask"  }, // 4 
{ DPK_DLPDEFAULTCONFIG_LOGOINDEX,VALUE_TYPE_STRING, dlpdefaultconfig_logoindex_value_string_list,1,"LogoIndex"  }, // 5 
{ DPK_DLPDEFAULTCONFIG_KEYSTONEENABLE,VALUE_TYPE_STRING, dlpdefaultconfig_keystoneenable_value_string_list,1,"KeystoneEnable"  }, // 6 
{ DPK_DLPDEFAULTCONFIG_AUTOFLIPENABLE,VALUE_TYPE_STRING, dlpdefaultconfig_autoflipenable_value_string_list,1,"AutoFlipEnable"  }, // 7 
{ DPK_DLPDEFAULTCONFIG_SCREENDIVISIONENABLE,VALUE_TYPE_STRING, dlpdefaultconfig_screendivisionenable_value_string_list,1,"ScreenDivisionEnable"  }, // 8 
{ DPK_DLPDEFAULTCONFIG_DEFAULTLANGUAGE,VALUE_TYPE_STRING, dlpdefaultconfig_defaultlanguage_value_string_list,1,"DefaultLanguage"  }, // 9 
{ DPK_DLPDEFAULTCONFIG_PRODUCTMODELNUMBER,VALUE_TYPE_STRING, dlpdefaultconfig_productmodelnumber_value_string_list,1,"ProductModelNumber"  }, // 10 
{ DPK_DLPDEFAULTCONFIG_MACADDRESS,VALUE_TYPE_STRING, dlpdefaultconfig_macaddress_value_string_list,1,"MacAddress"  }, // 11 
{ DPK_DLPDEFAULTCONFIG_STANDARDCURRENT,VALUE_TYPE_STRING, dlpdefaultconfig_standardcurrent_value_string_list,1,"StandardCurrent"  }, // 12 
{ DPK_DLPDEFAULTCONFIG_MOVIECURRENT,VALUE_TYPE_STRING, dlpdefaultconfig_moviecurrent_value_string_list,1,"MovieCurrent"  }, // 13 
{ DPK_DLPDEFAULTCONFIG_DEMOCURRENT,VALUE_TYPE_STRING, dlpdefaultconfig_democurrent_value_string_list,1,"DemoCurrent"  }, // 14 
{ DPK_DLPDEFAULTCONFIG_ECOCURRENT,VALUE_TYPE_STRING, dlpdefaultconfig_ecocurrent_value_string_list,1,"EcoCurrent"  }, // 15 
{ DPK_DLPDEFAULTCONFIG_DICOMSIMCURRENT,VALUE_TYPE_STRING, dlpdefaultconfig_dicomsimcurrent_value_string_list,1,"DicomSimCurrent"  }, // 16 
{ DPK_DLPDEFAULTCONFIG_SRGBCURRENT,VALUE_TYPE_STRING, dlpdefaultconfig_srgbcurrent_value_string_list,1,"SrgbCurrent"  }, // 17 
{ DPK_DLPDEFAULTCONFIG_USERCURRENT,VALUE_TYPE_STRING, dlpdefaultconfig_usercurrent_value_string_list,1,"UserCurrent"  }, // 18 
};
PARAM_KEY_ELEMENT adcadjust_key_string_list[]=
{ 
{ DPK_ADCADJUST_STATUS,VALUE_TYPE_ENUM, adcadjust_status_value_string_list,2,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT dlplogo_key_string_list[]=
{ 
{ DPK_DLPLOGO_NULL,VALUE_TYPE_INT, dlplogo_null_value_string_list,1,"Null"  }, // 1 
};
PARAM_KEY_ELEMENT motorzoom_key_string_list[]=
{ 
{ DPK_MOTORZOOM_FRONT,VALUE_TYPE_INT, motorzoom_front_value_string_list,1,"Front"  }, // 1 
{ DPK_MOTORZOOM_BACK,VALUE_TYPE_INT, motorzoom_back_value_string_list,1,"Back"  }, // 2 
};
PARAM_KEY_ELEMENT motorfocus_key_string_list[]=
{ 
{ DPK_MOTORFOCUS_FRONT,VALUE_TYPE_INT, motorfocus_front_value_string_list,1,"Front"  }, // 1 
{ DPK_MOTORFOCUS_BACK,VALUE_TYPE_INT, motorfocus_back_value_string_list,1,"Back"  }, // 2 
};
PARAM_KEY_ELEMENT motorshift_key_string_list[]=
{ 
{ DPK_MOTORSHIFT_LEFT,VALUE_TYPE_INT, motorshift_left_value_string_list,1,"Left"  }, // 1 
{ DPK_MOTORSHIFT_RIGHT,VALUE_TYPE_INT, motorshift_right_value_string_list,1,"Right"  }, // 2 
{ DPK_MOTORSHIFT_UP,VALUE_TYPE_INT, motorshift_up_value_string_list,1,"Up"  }, // 3 
{ DPK_MOTORSHIFT_DOWN,VALUE_TYPE_INT, motorshift_down_value_string_list,1,"Down"  }, // 4 
};
PARAM_KEY_ELEMENT motorcenter_key_string_list[]=
{ 
{ DPK_MOTORCENTER_ZOOM,VALUE_TYPE_INT, motorcenter_zoom_value_string_list,1,"Zoom"  }, // 1 
{ DPK_MOTORCENTER_FOCUS,VALUE_TYPE_INT, motorcenter_focus_value_string_list,1,"Focus"  }, // 2 
{ DPK_MOTORCENTER_SHIFTVERTICAL,VALUE_TYPE_INT, motorcenter_shiftvertical_value_string_list,1,"ShiftVertical"  }, // 3 
{ DPK_MOTORCENTER_HIFTHORIZONTAL,VALUE_TYPE_INT, motorcenter_hifthorizontal_value_string_list,1,"hiftHorizontal"  }, // 4 
{ DPK_MOTORCENTER_ALL,VALUE_TYPE_INT, motorcenter_all_value_string_list,1,"All"  }, // 5 
};
PARAM_KEY_ELEMENT dlpsource_key_string_list[]=
{ 
{ DPK_DLPSOURCE_EXTERNAL,VALUE_TYPE_ENUM, dlpsource_external_value_string_list,1,"External"  }, // 1 
{ DPK_DLPSOURCE_INSIDE,VALUE_TYPE_ENUM, dlpsource_inside_value_string_list,2,"Inside"  }, // 2 
};
PARAM_KEY_ELEMENT type3d_key_string_list[]=
{ 
{ DPK_TYPE3D_STATUS,VALUE_TYPE_ENUM, type3d_status_value_string_list,2,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT imagefreeze_key_string_list[]=
{ 
{ DPK_IMAGEFREEZE_FREEZE,VALUE_TYPE_INT, imagefreeze_freeze_value_string_list,1,"Freeze"  }, // 1 
{ DPK_IMAGEFREEZE_UNFREEZE,VALUE_TYPE_INT, imagefreeze_unfreeze_value_string_list,1,"UnFreeze"  }, // 2 
};
PARAM_KEY_ELEMENT fpgapatternsize_key_string_list[]=
{ 
{ DPK_FPGAPATTERNSIZE_STATUS,VALUE_TYPE_INT, fpgapatternsize_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT projectionmode1_key_string_list[]=
{ 
{ DPK_PROJECTIONMODE1_STATUS,VALUE_TYPE_ENUM, projectionmode1_status_value_string_list,5,"Status"  }, // 1 
};
ELEMENT cmd_string_list[]=
{
{ DC_SYSTEM , system_key_string_list,1,"System" }, // 1 
{ DC_LIGHTSOURCE , lightsource_key_string_list,1,"LightSource" }, // 2 
{ DC_VERSION , version_key_string_list,8,"Version" }, // 3 
{ DC_SN , sn_key_string_list,3,"Sn" }, // 4 
{ DC_LIGHTSOURCETIME , lightsourcetime_key_string_list,1,"LightSourceTime" }, // 5 
{ DC_GAMMA , gamma_key_string_list,1,"Gamma" }, // 6 
{ DC_INSTALLATIONMODE , installationmode_key_string_list,1,"InstallationMode" }, // 7 
{ DC_VERTICALKEYSTONE , verticalkeystone_key_string_list,1,"VerticalKeystone" }, // 8 
{ DC_HORIZONKEYSTONE , horizonkeystone_key_string_list,1,"HorizonKeystone" }, // 9 
{ DC_KEYSTONE2D , keystone2d_key_string_list,8,"Keystone2D" }, // 10 
{ DC_TESTPATTERN , testpattern_key_string_list,1,"TestPattern" }, // 11 
{ DC_MODE3D , mode3d_key_string_list,1,"Mode3D" }, // 12 
{ DC_SYNCFLIP3D , syncflip3d_key_string_list,1,"SyncFlip3D" }, // 13 
{ DC_PROJECTIONMODE , projectionmode_key_string_list,1,"ProjectionMode" }, // 14 
{ DC_FANSTATUS , fanstatus_key_string_list,28,"FanStatus" }, // 15 
{ DC_SETFAN , setfan_key_string_list,28,"SetFan" }, // 16 
{ DC_TEMPERATURE , temperature_key_string_list,7,"Temperature" }, // 17 
{ DC_CWSPEED , cwspeed_key_string_list,1,"CwSpeed" }, // 18 
{ DC_XPRFIXED , xprfixed_key_string_list,1,"XprFixed" }, // 19 
{ DC_RESET , reset_key_string_list,1,"Reset" }, // 20 
{ DC_SIGNALCHANNEL , signalchannel_key_string_list,7,"SignalChannel" }, // 21 
{ DC_ASPECTRATIO , aspectratio_key_string_list,1,"AspectRatio" }, // 22 
{ DC_STANBYTIMES , stanbytimes_key_string_list,1,"StanbyTimes" }, // 23 
{ DC_HEARTBEAT , heartbeat_key_string_list,1,"HeartBeat" }, // 24 
{ DC_EEPROM , eeprom_key_string_list,48,"Eeprom" }, // 25 
{ DC_HELP , help_key_string_list,1,"Help" }, // 26 
{ DC_SETCURRENT , setcurrent_key_string_list,9,"SetCurrent" }, // 27 
{ DC_ERROR , error_key_string_list,1,"Error" }, // 28 
{ DC_WARNING , warning_key_string_list,1,"Warning" }, // 29 
{ DC_LOGINFO , loginfo_key_string_list,1,"LogInfo" }, // 30 
{ DC_PRODUCTINFORMATION , productinformation_key_string_list,3,"ProductInformation" }, // 31 
{ DC_COLORWHEELDELAY , colorwheeldelay_key_string_list,1,"ColorWheelDelay" }, // 32 
{ DC_BRILLIANTCOLOR , brilliantcolor_key_string_list,1,"BrilliantColor" }, // 33 
{ DC_STANDBYMODE , standbymode_key_string_list,1,"StandbyMode" }, // 34 
{ DC_VOLUME , volume_key_string_list,1,"Volume" }, // 35 
{ DC_VOLUMECONTROL , volumecontrol_key_string_list,1,"VolumeControl" }, // 36 
{ DC_RUNTIME , runtime_key_string_list,1,"RunTime" }, // 37 
{ DC_POWERCONSUMPTION , powerconsumption_key_string_list,1,"PowerConsumption" }, // 38 
{ DC_GETCURRENT , getcurrent_key_string_list,5,"GetCurrent" }, // 39 
{ DC_UPGRADE , upgrade_key_string_list,2,"Upgrade" }, // 40 
{ DC_CONTRASTENHANCE , contrastenhance_key_string_list,1,"ContrastEnhance" }, // 41 
{ DC_FPGA , fpga_key_string_list,3,"Fpga" }, // 42 
{ DC_DLPPOWERSTATUS , dlppowerstatus_key_string_list,1,"DlpPowerStatus" }, // 43 
{ DC_EDID , edid_key_string_list,6,"Edid" }, // 44 
{ DC_NETWORK , network_key_string_list,4,"Network" }, // 45 
{ DC_DLPDEFAULTCONFIG , dlpdefaultconfig_key_string_list,18,"DlpDefaultConfig" }, // 46 
{ DC_ADCADJUST , adcadjust_key_string_list,1,"AdcAdjust" }, // 47 
{ DC_DLPLOGO , dlplogo_key_string_list,1,"DlpLogo" }, // 48 
{ DC_MOTORZOOM , motorzoom_key_string_list,2,"MotorZoom" }, // 49 
{ DC_MOTORFOCUS , motorfocus_key_string_list,2,"MotorFocus" }, // 50 
{ DC_MOTORSHIFT , motorshift_key_string_list,4,"MotorShift" }, // 51 
{ DC_MOTORCENTER , motorcenter_key_string_list,5,"MotorCenter" }, // 52 
{ DC_DLPSOURCE , dlpsource_key_string_list,2,"DlpSource" }, // 53 
{ DC_TYPE3D , type3d_key_string_list,1,"Type3D" }, // 54 
{ DC_IMAGEFREEZE , imagefreeze_key_string_list,2,"ImageFreeze" }, // 55 
{ DC_FPGAPATTERNSIZE , fpgapatternsize_key_string_list,1,"FpgaPatternSize" }, // 56 
{ DC_PROJECTIONMODE1 , projectionmode1_key_string_list,1,"ProjectionMode1" }, // 57 
};




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





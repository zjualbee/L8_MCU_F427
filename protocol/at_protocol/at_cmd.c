

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
};
PARAM_VALUE_ELEMENT lightsource_status_value_string_list[]= 
{ 
{ DPV_LIGHTSOURCE_STATUS_ON ,"On" }, // 7 
{ DPV_LIGHTSOURCE_STATUS_OFF ,"Off" }, // 8 
};
PARAM_VALUE_ELEMENT version_dlpsoftware_value_string_list[]= 
{ 
{ DPV_VERSION_DLPSOFTWARE_EMPTY ,"Empty" }, // 9 
};
PARAM_VALUE_ELEMENT version_pmusoftware_value_string_list[]= 
{ 
{ DPV_VERSION_PMUSOFTWARE_EMPTY ,"Empty" }, // 10 
};
PARAM_VALUE_ELEMENT version_idusoftware_value_string_list[]= 
{ 
{ DPV_VERSION_IDUSOFTWARE_EMPTY ,"Empty" }, // 11 
};
PARAM_VALUE_ELEMENT version_dlphardware_value_string_list[]= 
{ 
{ DPV_VERSION_DLPHARDWARE_EMPTY ,"Empty" }, // 12 
};
PARAM_VALUE_ELEMENT version_pmuhardware_value_string_list[]= 
{ 
{ DPV_VERSION_PMUHARDWARE_EMPTY ,"Empty" }, // 13 
};
PARAM_VALUE_ELEMENT version_iduhardware_value_string_list[]= 
{ 
{ DPV_VERSION_IDUHARDWARE_EMPTY ,"Empty" }, // 14 
};
PARAM_VALUE_ELEMENT sn_sourcelight_value_string_list[]= 
{ 
{ DPV_SN_SOURCELIGHT_EMPTY ,"Empty" }, // 15 
};
PARAM_VALUE_ELEMENT lightsourcetime_minute_value_string_list[]= 
{ 
{ DPV_LIGHTSOURCETIME_MINUTE_EMPTY ,"Empty" }, // 16 
};
PARAM_VALUE_ELEMENT gamma_status_value_string_list[]= 
{ 
{ DPV_GAMMA_STATUS_EMPTY ,"Empty" }, // 17 
};
PARAM_VALUE_ELEMENT installationmode_status_value_string_list[]= 
{ 
{ DPV_INSTALLATIONMODE_STATUS_CEILINGREAR ,"CeilingRear" }, // 18 
{ DPV_INSTALLATIONMODE_STATUS_CEILINGFRONT ,"CeilingFront" }, // 19 
{ DPV_INSTALLATIONMODE_STATUS_TABLEREAR ,"TableRear" }, // 20 
{ DPV_INSTALLATIONMODE_STATUS_TABLEFRONT ,"TableFront" }, // 21 
};
PARAM_VALUE_ELEMENT verticalkeystone_status_value_string_list[]= 
{ 
{ DPV_VERTICALKEYSTONE_STATUS_EMPTY ,"Empty" }, // 22 
};
PARAM_VALUE_ELEMENT horizonkeystone_status_value_string_list[]= 
{ 
{ DPV_HORIZONKEYSTONE_STATUS_EMPTY ,"Empty" }, // 23 
};
PARAM_VALUE_ELEMENT keystone2d_topleftx_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_TOPLEFTX_EMPTY ,"Empty" }, // 24 
};
PARAM_VALUE_ELEMENT keystone2d_toplefty_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_TOPLEFTY_EMPTY ,"Empty" }, // 25 
};
PARAM_VALUE_ELEMENT keystone2d_toprightx_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_TOPRIGHTX_EMPTY ,"Empty" }, // 26 
};
PARAM_VALUE_ELEMENT keystone2d_toprighty_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_TOPRIGHTY_EMPTY ,"Empty" }, // 27 
};
PARAM_VALUE_ELEMENT keystone2d_bottomleftx_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_BOTTOMLEFTX_EMPTY ,"Empty" }, // 28 
};
PARAM_VALUE_ELEMENT keystone2d_bottomlefty_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_BOTTOMLEFTY_EMPTY ,"Empty" }, // 29 
};
PARAM_VALUE_ELEMENT keystone2d_bottomrightx_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_BOTTOMRIGHTX_EMPTY ,"Empty" }, // 30 
};
PARAM_VALUE_ELEMENT keystone2d_bottomrighty_value_string_list[]= 
{ 
{ DPV_KEYSTONE2D_BOTTOMRIGHTY_EMPTY ,"Empty" }, // 31 
};
PARAM_VALUE_ELEMENT testpattern_status_value_string_list[]= 
{ 
{ DPV_TESTPATTERN_STATUS_OFF ,"Off" }, // 32 
{ DPV_TESTPATTERN_STATUS_RGBRAMPS ,"RGBRamps" }, // 33 
{ DPV_TESTPATTERN_STATUS_COLORBARS ,"ColorBars" }, // 34 
{ DPV_TESTPATTERN_STATUS_SETBARS ,"SetBars" }, // 35 
{ DPV_TESTPATTERN_STATUS_CHECKERBOARD ,"CheckErboard" }, // 36 
{ DPV_TESTPATTERN_STATUS_GRID ,"Grid" }, // 37 
{ DPV_TESTPATTERN_STATUS_HORIZONTALLINES ,"HorizontalLines" }, // 38 
{ DPV_TESTPATTERN_STATUS_VERTICALLINES ,"VerticalLines" }, // 39 
{ DPV_TESTPATTERN_STATUS_DIAGONALLINES ,"DiagonalLines" }, // 40 
{ DPV_TESTPATTERN_STATUS_HORIZONTALRAMP ,"HorizontalRamp" }, // 41 
{ DPV_TESTPATTERN_STATUS_VERTICALRAMP ,"VerticalRamp" }, // 42 
{ DPV_TESTPATTERN_STATUS_BLACK ,"Black" }, // 43 
{ DPV_TESTPATTERN_STATUS_BLUE ,"Blue" }, // 44 
{ DPV_TESTPATTERN_STATUS_GREY ,"Grey" }, // 45 
{ DPV_TESTPATTERN_STATUS_RED ,"Red" }, // 46 
{ DPV_TESTPATTERN_STATUS_GREEN ,"Green" }, // 47 
{ DPV_TESTPATTERN_STATUS_YELLOW ,"Yellow" }, // 48 
{ DPV_TESTPATTERN_STATUS_CYAN ,"Cyan" }, // 49 
{ DPV_TESTPATTERN_STATUS_MEGENTA ,"Megenta" }, // 50 
{ DPV_TESTPATTERN_STATUS_WHITE ,"White" }, // 51 
{ DPV_TESTPATTERN_STATUS_XPR ,"xpr" }, // 52 
};
PARAM_VALUE_ELEMENT mode3d_mode3d_value_string_list[]= 
{ 
{ DPV_MODE3D_MODE3D_OFF ,"Off" }, // 53 
{ DPV_MODE3D_MODE3D_VSYNCSEPARATEDHALF ,"VsyncSeparatedHalf" }, // 54 
{ DPV_MODE3D_MODE3D_VSYNCSEPARATEDFULL ,"VsyncSeparatedFull" }, // 55 
{ DPV_MODE3D_MODE3D_VERTPACKEDHALF ,"VertPackedHalf" }, // 56 
{ DPV_MODE3D_MODE3D_VERTPACKEDFULL ,"VertPackedFull" }, // 57 
{ DPV_MODE3D_MODE3D_HORIZPACKEDHALF ,"HorizPackedHalf" }, // 58 
{ DPV_MODE3D_MODE3D_HORIZPACKEDFULL ,"HorizPackedFull" }, // 59 
};
PARAM_VALUE_ELEMENT syncflip3d_status_value_string_list[]= 
{ 
{ DPV_SYNCFLIP3D_STATUS_ON ,"On" }, // 60 
{ DPV_SYNCFLIP3D_STATUS_OFF ,"Off" }, // 61 
};
PARAM_VALUE_ELEMENT projectionmode_status_value_string_list[]= 
{ 
{ DPV_PROJECTIONMODE_STATUS_HIGHLIGHT ,"HighLight" }, // 62 
{ DPV_PROJECTIONMODE_STATUS_MOVIE ,"Movie" }, // 63 
{ DPV_PROJECTIONMODE_STATUS_STANDARD ,"Standard" }, // 64 
{ DPV_PROJECTIONMODE_STATUS_POWERSAVE ,"PowerSave" }, // 65 
{ DPV_PROJECTIONMODE_STATUS_INVILID ,"Invilid" }, // 66 
};
PARAM_VALUE_ELEMENT fanstatus_fan1_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN1_EMPTY ,"Empty" }, // 67 
};
PARAM_VALUE_ELEMENT fanstatus_fan2_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN2_EMPTY ,"Empty" }, // 68 
};
PARAM_VALUE_ELEMENT fanstatus_fan3_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN3_EMPTY ,"Empty" }, // 69 
};
PARAM_VALUE_ELEMENT fanstatus_fan4_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN4_EMPTY ,"Empty" }, // 70 
};
PARAM_VALUE_ELEMENT fanstatus_fan5_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN5_EMPTY ,"Empty" }, // 71 
};
PARAM_VALUE_ELEMENT fanstatus_fan6_value_string_list[]= 
{ 
{ DPV_FANSTATUS_FAN6_EMPTY ,"Empty" }, // 72 
};
PARAM_VALUE_ELEMENT setfan_fan1_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN1_EMPTY ,"Empty" }, // 73 
};
PARAM_VALUE_ELEMENT setfan_fan2_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN2_EMPTY ,"Empty" }, // 74 
};
PARAM_VALUE_ELEMENT setfan_fan3_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN3_EMPTY ,"Empty" }, // 75 
};
PARAM_VALUE_ELEMENT setfan_fan4_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN4_EMPTY ,"Empty" }, // 76 
};
PARAM_VALUE_ELEMENT setfan_fan5_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN5_EMPTY ,"Empty" }, // 77 
};
PARAM_VALUE_ELEMENT setfan_fan6_value_string_list[]= 
{ 
{ DPV_SETFAN_FAN6_EMPTY ,"Empty" }, // 78 
};
PARAM_VALUE_ELEMENT temperature_ntcdmd1_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCDMD1_EMPTY ,"Empty" }, // 79 
};
PARAM_VALUE_ELEMENT temperature_ntccw1_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCCW1_EMPTY ,"Empty" }, // 80 
};
PARAM_VALUE_ELEMENT temperature_ntcbluelaser1_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCBLUELASER1_EMPTY ,"Empty" }, // 81 
};
PARAM_VALUE_ELEMENT temperature_ntcxpr1_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCXPR1_EMPTY ,"Empty" }, // 82 
};
PARAM_VALUE_ELEMENT temperature_ntcenv1_value_string_list[]= 
{ 
{ DPV_TEMPERATURE_NTCENV1_EMPTY ,"Empty" }, // 83 
};
PARAM_VALUE_ELEMENT cwspeed_status_value_string_list[]= 
{ 
{ DPV_CWSPEED_STATUS_STATUS ,"Status" }, // 84 
};
PARAM_VALUE_ELEMENT xprfixed_status_value_string_list[]= 
{ 
{ DPV_XPRFIXED_STATUS_ON ,"On" }, // 85 
{ DPV_XPRFIXED_STATUS_OFF ,"Off" }, // 86 
{ DPV_XPRFIXED_STATUS_ERROR ,"Error" }, // 87 
{ DPV_XPRFIXED_STATUS_INVILID ,"Invilid" }, // 88 
};
PARAM_VALUE_ELEMENT reset_status_value_string_list[]= 
{ 
{ DPV_RESET_STATUS_FACTORY ,"Factory" }, // 89 
{ DPV_RESET_STATUS_USER ,"User" }, // 90 
{ DPV_RESET_STATUS_SERVICER ,"Servicer" }, // 91 
};
PARAM_VALUE_ELEMENT signalchannel_none_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_NONE_EMPTY ,"Empty" }, // 92 
};
PARAM_VALUE_ELEMENT signalchannel_hdmi_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_HDMI_EMPTY ,"Empty" }, // 93 
};
PARAM_VALUE_ELEMENT signalchannel_vga_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_VGA_EMPTY ,"Empty" }, // 94 
};
PARAM_VALUE_ELEMENT signalchannel_cds_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_CDS_EMPTY ,"Empty" }, // 95 
};
PARAM_VALUE_ELEMENT signalchannel_dvi_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_DVI_EMPTY ,"Empty" }, // 96 
};
PARAM_VALUE_ELEMENT signalchannel_ycbcr_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_YCBCR_EMPTY ,"Empty" }, // 97 
};
PARAM_VALUE_ELEMENT signalchannel_sdi_value_string_list[]= 
{ 
{ DPV_SIGNALCHANNEL_SDI_EMPTY ,"Empty" }, // 98 
};
PARAM_VALUE_ELEMENT aspectratio_status_value_string_list[]= 
{ 
{ DPV_ASPECTRATIO_STATUS_NATIVE ,"Native" }, // 99 
{ DPV_ASPECTRATIO_STATUS_FILL ,"Fill" }, // 100 
{ DPV_ASPECTRATIO_STATUS_4_3 ,"4_3" }, // 101 
{ DPV_ASPECTRATIO_STATUS_16_6 ,"16_6" }, // 102 
{ DPV_ASPECTRATIO_STATUS_16_9 ,"16_9" }, // 103 
{ DPV_ASPECTRATIO_STATUS_16_10 ,"16_10" }, // 104 
};
PARAM_VALUE_ELEMENT stanbytimes_status_value_string_list[]= 
{ 
{ DPV_STANBYTIMES_STATUS_EMPTY ,"Empty" }, // 105 
};
PARAM_VALUE_ELEMENT heartbeat_status_value_string_list[]= 
{ 
{ DPV_HEARTBEAT_STATUS_EMPTY ,"Empty" }, // 106 
};
PARAM_VALUE_ELEMENT eeprom_colorwheeldelay_value_string_list[]= 
{ 
{ DPV_EEPROM_COLORWHEELDELAY_EMPTY ,"Empty" }, // 107 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodehighlight_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODEHIGHLIGHT_EMPTY ,"Empty" }, // 108 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodemovie_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODEMOVIE_EMPTY ,"Empty" }, // 109 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodestadard_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODESTADARD_EMPTY ,"Empty" }, // 110 
};
PARAM_VALUE_ELEMENT eeprom_projectionmodepowersave_value_string_list[]= 
{ 
{ DPV_EEPROM_PROJECTIONMODEPOWERSAVE_EMPTY ,"Empty" }, // 111 
};
PARAM_VALUE_ELEMENT eeprom_colorwheelmode_value_string_list[]= 
{ 
{ DPV_EEPROM_COLORWHEELMODE_EMPTY ,"Empty" }, // 112 
};
PARAM_VALUE_ELEMENT eeprom_colorwheelspoke_value_string_list[]= 
{ 
{ DPV_EEPROM_COLORWHEELSPOKE_EMPTY ,"Empty" }, // 113 
};
PARAM_VALUE_ELEMENT help_status_value_string_list[]= 
{ 
{ DPV_HELP_STATUS_EMPTY ,"Empty" }, // 114 
};
PARAM_VALUE_ELEMENT setcurrent_r_value_string_list[]= 
{ 
{ DPV_SETCURRENT_R_EMPTY ,"Empty" }, // 115 
};
PARAM_VALUE_ELEMENT setcurrent_g_value_string_list[]= 
{ 
{ DPV_SETCURRENT_G_EMPTY ,"Empty" }, // 116 
};
PARAM_VALUE_ELEMENT setcurrent_b_value_string_list[]= 
{ 
{ DPV_SETCURRENT_B_EMPTY ,"Empty" }, // 117 
};
PARAM_VALUE_ELEMENT setcurrent_y_value_string_list[]= 
{ 
{ DPV_SETCURRENT_Y_EMPTY ,"Empty" }, // 118 
};
PARAM_VALUE_ELEMENT setcurrent_all_value_string_list[]= 
{ 
{ DPV_SETCURRENT_ALL_EMPTY ,"Empty" }, // 119 
};
PARAM_VALUE_ELEMENT error_status_value_string_list[]= 
{ 
{ DPV_ERROR_STATUS_EMPTY ,"Empty" }, // 120 
};
PARAM_VALUE_ELEMENT warning_status_value_string_list[]= 
{ 
{ DPV_WARNING_STATUS_EMPTY ,"Empty" }, // 121 
};
PARAM_VALUE_ELEMENT loginfo_status_value_string_list[]= 
{ 
{ DPV_LOGINFO_STATUS_EMPTY ,"Empty" }, // 122 
};
PARAM_VALUE_ELEMENT productinformation_modelname_value_string_list[]= 
{ 
{ DPV_PRODUCTINFORMATION_MODELNAME_EMPTY ,"Empty" }, // 123 
};
PARAM_VALUE_ELEMENT productinformation_brandname_value_string_list[]= 
{ 
{ DPV_PRODUCTINFORMATION_BRANDNAME_EMPTY ,"Empty" }, // 124 
};
PARAM_VALUE_ELEMENT productinformation_machinesn_value_string_list[]= 
{ 
{ DPV_PRODUCTINFORMATION_MACHINESN_EMPTY ,"Empty" }, // 125 
};
PARAM_VALUE_ELEMENT colorwheeldelay_status_value_string_list[]= 
{ 
{ DPV_COLORWHEELDELAY_STATUS_EMPTY ,"Empty" }, // 126 
};
PARAM_VALUE_ELEMENT brilliantcolor_status_value_string_list[]= 
{ 
{ DPV_BRILLIANTCOLOR_STATUS_ON ,"On" }, // 127 
{ DPV_BRILLIANTCOLOR_STATUS_OFF ,"Off" }, // 128 
};
PARAM_VALUE_ELEMENT standbymode_status_value_string_list[]= 
{ 
{ DPV_STANDBYMODE_STATUS_LOWPOWER ,"LowPower" }, // 129 
{ DPV_STANDBYMODE_STATUS_NETWORKSTANDBY ,"NetworkStandby" }, // 130 
};
PARAM_VALUE_ELEMENT volume_status_value_string_list[]= 
{ 
{ DPV_VOLUME_STATUS_EMPTY ,"Empty" }, // 131 
};
PARAM_VALUE_ELEMENT runtime_status_value_string_list[]= 
{ 
{ DPV_RUNTIME_STATUS_EMPTY ,"Empty" }, // 132 
};
PARAM_VALUE_ELEMENT powerconsumption_status_value_string_list[]= 
{ 
{ DPV_POWERCONSUMPTION_STATUS_EMPTY ,"Empty" }, // 133 
};
PARAM_VALUE_ELEMENT getcurrent_r_value_string_list[]= 
{ 
{ DPV_GETCURRENT_R_EMPTY ,"Empty" }, // 134 
};
PARAM_VALUE_ELEMENT getcurrent_g_value_string_list[]= 
{ 
{ DPV_GETCURRENT_G_EMPTY ,"Empty" }, // 135 
};
PARAM_VALUE_ELEMENT getcurrent_b_value_string_list[]= 
{ 
{ DPV_GETCURRENT_B_EMPTY ,"Empty" }, // 136 
};
PARAM_VALUE_ELEMENT getcurrent_y_value_string_list[]= 
{ 
{ DPV_GETCURRENT_Y_EMPTY ,"Empty" }, // 137 
};
PARAM_VALUE_ELEMENT getcurrent_all_value_string_list[]= 
{ 
{ DPV_GETCURRENT_ALL_EMPTY ,"Empty" }, // 138 
};
PARAM_VALUE_ELEMENT upgrade_dlpcompose_value_string_list[]= 
{ 
{ DPV_UPGRADE_DLPCOMPOSE_ON ,"On" }, // 139 
{ DPV_UPGRADE_DLPCOMPOSE_OFF ,"Off" }, // 140 
};
PARAM_VALUE_ELEMENT upgrade_dlpboot_value_string_list[]= 
{ 
{ DPV_UPGRADE_DLPBOOT_ON ,"On" }, // 141 
{ DPV_UPGRADE_DLPBOOT_OFF ,"Off" }, // 142 
};
PARAM_VALUE_ELEMENT contrastenhance_status_value_string_list[]= 
{ 
{ DPV_CONTRASTENHANCE_STATUS_ON ,"On" }, // 143 
{ DPV_CONTRASTENHANCE_STATUS_OFF ,"Off" }, // 144 
};
PARAM_VALUE_ELEMENT fpga_vx1fuction_value_string_list[]= 
{ 
{ DPV_FPGA_VX1FUCTION_ON ,"On" }, // 145 
{ DPV_FPGA_VX1FUCTION_OFF ,"Off" }, // 146 
};
PARAM_VALUE_ELEMENT fpga_videooutput_value_string_list[]= 
{ 
{ DPV_FPGA_VIDEOOUTPUT_ON ,"On" }, // 147 
{ DPV_FPGA_VIDEOOUTPUT_OFF ,"Off" }, // 148 
};
PARAM_VALUE_ELEMENT dlppowerstatus_status_value_string_list[]= 
{ 
{ DPV_DLPPOWERSTATUS_STATUS_ON ,"On" }, // 149 
{ DPV_DLPPOWERSTATUS_STATUS_OFF ,"Off" }, // 150 
{ DPV_DLPPOWERSTATUS_STATUS_RESET ,"Reset" }, // 151 
{ DPV_DLPPOWERSTATUS_STATUS_STANDBY ,"Standby" }, // 152 
{ DPV_DLPPOWERSTATUS_STATUS_ACTIVE ,"Active" }, // 153 
{ DPV_DLPPOWERSTATUS_STATUS_COOLING ,"Cooling" }, // 154 
{ DPV_DLPPOWERSTATUS_STATUS_WARMING ,"Warming" }, // 155 
{ DPV_DLPPOWERSTATUS_STATUS_POWERUP ,"Powerup" }, // 156 
};
PARAM_KEY_ELEMENT system_key_string_list[]=
{ 
{ DPK_SYSTEM_STATUS,VALUE_TYPE_ENUM, system_status_value_string_list,6,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT lightsource_key_string_list[]=
{ 
{ DPK_LIGHTSOURCE_STATUS,VALUE_TYPE_ENUM, lightsource_status_value_string_list,2,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT version_key_string_list[]=
{ 
{ DPK_VERSION_DLPSOFTWARE,VALUE_TYPE_STRING, version_dlpsoftware_value_string_list,1,"DlpSoftware"  }, // 1 
{ DPK_VERSION_PMUSOFTWARE,VALUE_TYPE_STRING, version_pmusoftware_value_string_list,1,"PmuSoftware"  }, // 2 
{ DPK_VERSION_IDUSOFTWARE,VALUE_TYPE_STRING, version_idusoftware_value_string_list,1,"IduSoftware"  }, // 3 
{ DPK_VERSION_DLPHARDWARE,VALUE_TYPE_STRING, version_dlphardware_value_string_list,1,"DlpHardware"  }, // 4 
{ DPK_VERSION_PMUHARDWARE,VALUE_TYPE_STRING, version_pmuhardware_value_string_list,1,"PmuHardware"  }, // 5 
{ DPK_VERSION_IDUHARDWARE,VALUE_TYPE_STRING, version_iduhardware_value_string_list,1,"IduHardware"  }, // 6 
};
PARAM_KEY_ELEMENT sn_key_string_list[]=
{ 
{ DPK_SN_SOURCELIGHT,VALUE_TYPE_STRING, sn_sourcelight_value_string_list,1,"SourceLight"  }, // 1 
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
{ DPK_INSTALLATIONMODE_STATUS,VALUE_TYPE_ENUM, installationmode_status_value_string_list,4,"Status"  }, // 1 
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
{ DPK_TESTPATTERN_STATUS,VALUE_TYPE_ENUM, testpattern_status_value_string_list,21,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT mode3d_key_string_list[]=
{ 
{ DPK_MODE3D_MODE3D,VALUE_TYPE_ENUM, mode3d_mode3d_value_string_list,7,"Mode3D"  }, // 1 
};
PARAM_KEY_ELEMENT syncflip3d_key_string_list[]=
{ 
{ DPK_SYNCFLIP3D_STATUS,VALUE_TYPE_ENUM, syncflip3d_status_value_string_list,2,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT projectionmode_key_string_list[]=
{ 
{ DPK_PROJECTIONMODE_STATUS,VALUE_TYPE_ENUM, projectionmode_status_value_string_list,5,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT fanstatus_key_string_list[]=
{ 
{ DPK_FANSTATUS_FAN1,VALUE_TYPE_INT, fanstatus_fan1_value_string_list,1,"Fan1"  }, // 1 
{ DPK_FANSTATUS_FAN2,VALUE_TYPE_INT, fanstatus_fan2_value_string_list,1,"Fan2"  }, // 2 
{ DPK_FANSTATUS_FAN3,VALUE_TYPE_INT, fanstatus_fan3_value_string_list,1,"Fan3"  }, // 3 
{ DPK_FANSTATUS_FAN4,VALUE_TYPE_INT, fanstatus_fan4_value_string_list,1,"Fan4"  }, // 4 
{ DPK_FANSTATUS_FAN5,VALUE_TYPE_INT, fanstatus_fan5_value_string_list,1,"Fan5"  }, // 5 
{ DPK_FANSTATUS_FAN6,VALUE_TYPE_INT, fanstatus_fan6_value_string_list,1,"Fan6"  }, // 6 
};
PARAM_KEY_ELEMENT setfan_key_string_list[]=
{ 
{ DPK_SETFAN_FAN1,VALUE_TYPE_INT, setfan_fan1_value_string_list,1,"Fan1"  }, // 1 
{ DPK_SETFAN_FAN2,VALUE_TYPE_INT, setfan_fan2_value_string_list,1,"Fan2"  }, // 2 
{ DPK_SETFAN_FAN3,VALUE_TYPE_INT, setfan_fan3_value_string_list,1,"Fan3"  }, // 3 
{ DPK_SETFAN_FAN4,VALUE_TYPE_INT, setfan_fan4_value_string_list,1,"Fan4"  }, // 4 
{ DPK_SETFAN_FAN5,VALUE_TYPE_INT, setfan_fan5_value_string_list,1,"Fan5"  }, // 5 
{ DPK_SETFAN_FAN6,VALUE_TYPE_INT, setfan_fan6_value_string_list,1,"Fan6"  }, // 6 
};
PARAM_KEY_ELEMENT temperature_key_string_list[]=
{ 
{ DPK_TEMPERATURE_NTCDMD1,VALUE_TYPE_INT, temperature_ntcdmd1_value_string_list,1,"NtcDmd1"  }, // 1 
{ DPK_TEMPERATURE_NTCCW1,VALUE_TYPE_INT, temperature_ntccw1_value_string_list,1,"NtcCw1"  }, // 2 
{ DPK_TEMPERATURE_NTCBLUELASER1,VALUE_TYPE_INT, temperature_ntcbluelaser1_value_string_list,1,"NtcBlueLaser1"  }, // 3 
{ DPK_TEMPERATURE_NTCXPR1,VALUE_TYPE_INT, temperature_ntcxpr1_value_string_list,1,"NtcXpr1"  }, // 4 
{ DPK_TEMPERATURE_NTCENV1,VALUE_TYPE_INT, temperature_ntcenv1_value_string_list,1,"NtcEnv1"  }, // 5 
};
PARAM_KEY_ELEMENT cwspeed_key_string_list[]=
{ 
{ DPK_CWSPEED_STATUS,VALUE_TYPE_INT, cwspeed_status_value_string_list,1,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT xprfixed_key_string_list[]=
{ 
{ DPK_XPRFIXED_STATUS,VALUE_TYPE_ENUM, xprfixed_status_value_string_list,4,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT reset_key_string_list[]=
{ 
{ DPK_RESET_STATUS,VALUE_TYPE_ENUM, reset_status_value_string_list,3,"Status"  }, // 1 
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
{ DPK_ASPECTRATIO_STATUS,VALUE_TYPE_ENUM, aspectratio_status_value_string_list,6,"Status"  }, // 1 
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
{ DPK_EEPROM_COLORWHEELDELAY,VALUE_TYPE_INT, eeprom_colorwheeldelay_value_string_list,1,"ColorWheelDelay"  }, // 1 
{ DPK_EEPROM_PROJECTIONMODEHIGHLIGHT,VALUE_TYPE_STRING, eeprom_projectionmodehighlight_value_string_list,1,"ProjectionModeHighLight"  }, // 2 
{ DPK_EEPROM_PROJECTIONMODEMOVIE,VALUE_TYPE_STRING, eeprom_projectionmodemovie_value_string_list,1,"ProjectionModeMovie"  }, // 3 
{ DPK_EEPROM_PROJECTIONMODESTADARD,VALUE_TYPE_STRING, eeprom_projectionmodestadard_value_string_list,1,"ProjectionModeStadard"  }, // 4 
{ DPK_EEPROM_PROJECTIONMODEPOWERSAVE,VALUE_TYPE_STRING, eeprom_projectionmodepowersave_value_string_list,1,"ProjectionModePowersave"  }, // 5 
{ DPK_EEPROM_COLORWHEELMODE,VALUE_TYPE_STRING, eeprom_colorwheelmode_value_string_list,1,"ColorWheelMode"  }, // 6 
{ DPK_EEPROM_COLORWHEELSPOKE,VALUE_TYPE_STRING, eeprom_colorwheelspoke_value_string_list,1,"ColorWheelSpoke"  }, // 7 
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
{ DPK_SETCURRENT_ALL,VALUE_TYPE_INT, setcurrent_all_value_string_list,1,"ALL"  }, // 5 
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
{ DPK_BRILLIANTCOLOR_STATUS,VALUE_TYPE_ENUM, brilliantcolor_status_value_string_list,2,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT standbymode_key_string_list[]=
{ 
{ DPK_STANDBYMODE_STATUS,VALUE_TYPE_ENUM, standbymode_status_value_string_list,2,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT volume_key_string_list[]=
{ 
{ DPK_VOLUME_STATUS,VALUE_TYPE_INT, volume_status_value_string_list,1,"Status"  }, // 1 
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
{ DPK_UPGRADE_DLPCOMPOSE,VALUE_TYPE_ENUM, upgrade_dlpcompose_value_string_list,2,"DlpCompose"  }, // 1 
{ DPK_UPGRADE_DLPBOOT,VALUE_TYPE_ENUM, upgrade_dlpboot_value_string_list,2,"DlpBoot"  }, // 2 
};
PARAM_KEY_ELEMENT contrastenhance_key_string_list[]=
{ 
{ DPK_CONTRASTENHANCE_STATUS,VALUE_TYPE_ENUM, contrastenhance_status_value_string_list,2,"Status"  }, // 1 
};
PARAM_KEY_ELEMENT fpga_key_string_list[]=
{ 
{ DPK_FPGA_VX1FUCTION,VALUE_TYPE_ENUM, fpga_vx1fuction_value_string_list,2,"Vx1Fuction"  }, // 1 
{ DPK_FPGA_VIDEOOUTPUT,VALUE_TYPE_ENUM, fpga_videooutput_value_string_list,2,"VideoOutput"  }, // 2 
};
PARAM_KEY_ELEMENT dlppowerstatus_key_string_list[]=
{ 
{ DPK_DLPPOWERSTATUS_STATUS,VALUE_TYPE_ENUM, dlppowerstatus_status_value_string_list,8,"Status"  }, // 1 
};
ELEMENT cmd_string_list[]=
{
{ DC_SYSTEM , system_key_string_list,1,"System" }, // 1 
{ DC_LIGHTSOURCE , lightsource_key_string_list,1,"LightSource" }, // 2 
{ DC_VERSION , version_key_string_list,6,"Version" }, // 3 
{ DC_SN , sn_key_string_list,1,"Sn" }, // 4 
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
{ DC_FANSTATUS , fanstatus_key_string_list,6,"FanStatus" }, // 15 
{ DC_SETFAN , setfan_key_string_list,6,"SetFan" }, // 16 
{ DC_TEMPERATURE , temperature_key_string_list,5,"Temperature" }, // 17 
{ DC_CWSPEED , cwspeed_key_string_list,1,"CwSpeed" }, // 18 
{ DC_XPRFIXED , xprfixed_key_string_list,1,"XprFixed" }, // 19 
{ DC_RESET , reset_key_string_list,1,"Reset" }, // 20 
{ DC_SIGNALCHANNEL , signalchannel_key_string_list,7,"SignalChannel" }, // 21 
{ DC_ASPECTRATIO , aspectratio_key_string_list,1,"AspectRatio" }, // 22 
{ DC_STANBYTIMES , stanbytimes_key_string_list,1,"StanbyTimes" }, // 23 
{ DC_HEARTBEAT , heartbeat_key_string_list,1,"HeartBeat" }, // 24 
{ DC_EEPROM , eeprom_key_string_list,7,"Eeprom" }, // 25 
{ DC_HELP , help_key_string_list,1,"Help" }, // 26 
{ DC_SETCURRENT , setcurrent_key_string_list,5,"SetCurrent" }, // 27 
{ DC_ERROR , error_key_string_list,1,"Error" }, // 28 
{ DC_WARNING , warning_key_string_list,1,"Warning" }, // 29 
{ DC_LOGINFO , loginfo_key_string_list,1,"LogInfo" }, // 30 
{ DC_PRODUCTINFORMATION , productinformation_key_string_list,3,"ProductInformation" }, // 31 
{ DC_COLORWHEELDELAY , colorwheeldelay_key_string_list,1,"ColorWheelDelay" }, // 32 
{ DC_BRILLIANTCOLOR , brilliantcolor_key_string_list,1,"BrilliantColor" }, // 33 
{ DC_STANDBYMODE , standbymode_key_string_list,1,"StandbyMode" }, // 34 
{ DC_VOLUME , volume_key_string_list,1,"Volume" }, // 35 
{ DC_RUNTIME , runtime_key_string_list,1,"RunTime" }, // 36 
{ DC_POWERCONSUMPTION , powerconsumption_key_string_list,1,"PowerConsumption" }, // 37 
{ DC_GETCURRENT , getcurrent_key_string_list,5,"GetCurrent" }, // 38 
{ DC_UPGRADE , upgrade_key_string_list,2,"Upgrade" }, // 39 
{ DC_CONTRASTENHANCE , contrastenhance_key_string_list,1,"ContrastEnhance" }, // 40 
{ DC_FPGA , fpga_key_string_list,2,"Fpga" }, // 41 
{ DC_DLPPOWERSTATUS , dlppowerstatus_key_string_list,1,"DlpPowerStatus" }, // 42 
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





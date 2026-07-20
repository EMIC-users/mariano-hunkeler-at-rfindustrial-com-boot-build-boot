
#include <xc.h>



#define TRIS_MCP2200_RST	                _TRISB14
#define PORT_MCP2200_RST 	                _RB14
#define LAT_MCP2200_RST 	                _LATB14
#define ODC_MCP2200_RST	                _ODB14
#define PIN_MCP2200_RST 	                _RB14
#define RPOUT_MCP2200_RST	                RPOR7bits.RP14R
#define RPIN_MCP2200_RST	                14
#define CN_MCP2200_RST		                12
#define ADC_value_MCP2200_RST              Buffer_entradas[10] 
#define HAL_SetAnalog_MCP2200_RST()        {_PCFG10=0;\
                                        adc_addAnalogChannel(10);}

#define TRIS_MCP2200_SSPND	                _TRISB13
#define PORT_MCP2200_SSPND 	                _RB13
#define LAT_MCP2200_SSPND 	                _LATB13
#define ODC_MCP2200_SSPND	                _ODB13
#define PIN_MCP2200_SSPND 	                _RB13
#define RPOUT_MCP2200_SSPND		            RPOR6bits.RP13R
#define RPIN_MCP2200_SSPND		            13
#define CN_MCP2200_SSPND		                13
#define ADC_value_MCP2200_SSPND              Buffer_entradas[11] 
#define HAL_SetAnalog_MCP2200_SSPND()        {_PCFG11=0;\
                                        adc_addAnalogChannel(11);}

#define TRIS_MCP2200_USBCFG	                _TRISB12
#define PORT_MCP2200_USBCFG 	                _RB12
#define LAT_MCP2200_USBCFG 	                _LATB12
#define ODC_MCP2200_USBCFG	                _ODB12
#define PIN_MCP2200_USBCFG 	                _RB12
#define RPOUT_MCP2200_USBCFG		            RPOR6bits.RP12R
#define RPIN_MCP2200_USBCFG		            12
#define CN_MCP2200_USBCFG		                14
#define ADC_value_MCP2200_USBCFG              Buffer_entradas[12]
#define HAL_SetAnalog_MCP2200_USBCFG()        {_PCFG12=0;\
                                        adc_addAnalogChannel(12);}

#define TRIS_Led1	                _TRISB6
#define PIN_Led1 	                _RB6
#define LAT_Led1 	                _LATB6
#define ODC_Led1	                _ODB6
#define RPOUT_Led1		            RPOR3bits.RP6R
#define RPIN_Led1		            6
#define CN_Led1		                24
// Bus I2C EMIC (I2C2): SCL=B3, SDA=B2 (de la config CCS HRD_USB V1.1)
#define TRIS_SCL	                _TRISB3
#define LAT_SCL 	                _LATB3
#define ODC_SCL	                _ODB3
#define PIN_SCL 	                _RB3
#define RPOUT_SCL	                RPOR1bits.RP3R
#define RPIN_SCL	                3
#define CN_SCL		                7
#define ADC_value_SCL              Buffer_entradas[5] 
#define HAL_SetAnalog_SCL()        {_PCFG5=0;\
                                        adc_addAnalogChannel(5);}

#define TRIS_SDA	                _TRISB2
#define PIN_SDA 	                _RB2
#define LAT_SDA 	                _LATB2
#define ODC_SDA	                _ODB2
#define RPOUT_SDA		            RPOR1bits.RP2R
#define RPIN_SDA		            2
#define CN_SDA		                6
#define ADC_value_SDA              Buffer_entradas[4] 
#define HAL_SetAnalog_SDA()        {_PCFG4=0;\
                                        adc_addAnalogChannel(4);}

#define TRIS_MCP2200_TX	                _TRISB10
#define PORT_MCP2200_TX 	                _RB10
#define LAT_MCP2200_TX 	                _LATB10
#define ODC_MCP2200_TX	                _ODB10
#define PIN_MCP2200_TX 	                _RB10
#define RPOUT_MCP2200_TX	                RPOR5bits.RP10R
#define RPIN_MCP2200_TX	                10
#define CN_MCP2200_TX		                16
#define TRIS_MCP2200_RX	                _TRISB15
#define PORT_MCP2200_RX 	                _RB15
#define LAT_MCP2200_RX 	                _LATB15
#define ODC_MCP2200_RX	                _ODB15
#define PIN_MCP2200_RX 	                _RB15
#define RPOUT_MCP2200_RX	                RPOR7bits.RP15R
#define RPIN_MCP2200_RX	                15
#define CN_MCP2200_RX		                11
#define ADC_value_MCP2200_RX              Buffer_entradas[9] 
#define HAL_SetAnalog_MCP2200_RX()        {_PCFG9=0;\
                                        adc_addAnalogChannel(9);}
#define TRIS_MCP2200_RTS	                _TRISB11
#define PORT_MCP2200_RTS 	                _RB11
#define LAT_MCP2200_RTS 	                _LATB11
#define ODC_MCP2200_RTS	                _ODB11
#define PIN_MCP2200_RTS 	                _RB11
#define RPOUT_MCP2200_RTS		            RPOR5bits.RP11R
#define RPIN_MCP2200_RTS		            11
#define CN_MCP2200_RTS		                15
#define TRIS_MCP2200_CTS	                _TRISB7
#define PORT_MCP2200_CTS 	                _RB7
#define PIN_MCP2200_CTS 	                _RB7
#define LAT_MCP2200_CTS 	                _LATB7
#define ODC_MCP2200_CTS	                _ODB7
#define RPOUT_MCP2200_CTS		            RPOR3bits.RP7R
#define RPIN_MCP2200_CTS		            7
#define CN_MCP2200_CTS		                23


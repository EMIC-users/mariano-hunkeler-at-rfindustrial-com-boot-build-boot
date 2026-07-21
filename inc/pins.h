#include <xc.h>
#include <libpic30.h>


// Puerto I2C del bus EMIC (convención EMIC system.i2c; usado por app y bootloader)

#define TRIS_Rele1	                _TRISB15
#define PORT_Rele1 	                _RB15
#define LAT_Rele1 	                _LATB15
#define ODC_Rele1	                _ODB15
#define PIN_Rele1 	                _RB15
#define RPOUT_Rele1	                RPOR7bits.RP15R
#define RPIN_Rele1	                15
#define CN_Rele1		                11
#define ADC_value_Rele1              Buffer_entradas[9] 
#define HAL_SetAnalog_Rele1()        {_PCFG9=0;\
                                        adc_addAnalogChannel(9);}
#define TRIS_Rele2	                _TRISB14
#define PORT_Rele2 	                _RB14
#define LAT_Rele2 	                _LATB14
#define ODC_Rele2	                _ODB14
#define PIN_Rele2 	                _RB14
#define RPOUT_Rele2	                RPOR7bits.RP14R
#define RPIN_Rele2	                14
#define CN_Rele2		                12
#define ADC_value_Rele2              Buffer_entradas[10] 
#define HAL_SetAnalog_Rele2()        {_PCFG10=0;\
                                        adc_addAnalogChannel(10);}

#define TRIS_Led1	                _TRISB6
#define PIN_Led1 	                _RB6
#define LAT_Led1 	                _LATB6
#define ODC_Led1	                _ODB6
#define RPOUT_Led1		            RPOR3bits.RP6R
#define RPIN_Led1		            6
#define CN_Led1		                24
#define TRIS_Led2	_TRISB4
#define LAT_Led2 	_LATB4
#define ODC_Led2	_ODB4
#define PIN_Led2 	_RB4
#define RPOUT_Led2	RPOR2bits.RP4R
#define RPIN_Led2	4
#define CN_Led2		1
#define TRIS_Led3	                _TRISB5
#define PIN_Led3 	                _RB5
#define LAT_Led3 	                _LATB5
#define ODC_Led3	                _ODB5
#define RPOUT_Led3		            RPOR2bits.RP5R
#define RPIN_Led3		            5
#define CN_Led3		                27
#define TRIS_SCL_	                _TRISB8
#define PIN_SCL_ 	                _RB8
#define LAT_SCL_ 	                _LATB8
#define ODC_SCL_	                _ODB8
#define RPOUT_SCL_		            RPOR4bits.RP8R
#define RPIN_SCL_		            8
#define CN_SCL_		                22
#define TRIS_SDA_	                _TRISB9
#define PIN_SDA_ 	                _RB9
#define LAT_SDA_ 	                _LATB9
#define ODC_SDA_	                _ODB9
#define RPOUT_SDA_		            RPOR4bits.RP9R
#define RPIN_SDA_		            9
#define CN_SDA_		                21
#define TRIS_SDA__	                _TRISB2
#define PIN_SDA__ 	                _RB2
#define LAT_SDA__ 	                _LATB2
#define ODC_SDA__	                _ODB2
#define RPOUT_SDA__		            RPOR1bits.RP2R
#define RPIN_SDA__		            2
#define CN_SDA__		                6
#define ADC_value_SDA__              Buffer_entradas[4] 
#define HAL_SetAnalog_SDA__()        {_PCFG4=0;\
                                        adc_addAnalogChannel(4);}

// Bus I2C EMIC para el bootloader (I2C1 en pines primarios por fuse I2C1SEL=PRI): SCL=B8, SDA=B9
#define TRIS_SCL	                _TRISB8
#define PIN_SCL 	                _RB8
#define LAT_SCL 	                _LATB8
#define ODC_SCL	                _ODB8
#define RPOUT_SCL		            RPOR4bits.RP8R
#define RPIN_SCL		            8
#define CN_SCL		                22
#define TRIS_SDA	                _TRISB9
#define PIN_SDA 	                _RB9
#define LAT_SDA 	                _LATB9
#define ODC_SDA	                _ODB9
#define RPOUT_SDA		            RPOR4bits.RP9R
#define RPIN_SDA		            9
#define CN_SDA		                21

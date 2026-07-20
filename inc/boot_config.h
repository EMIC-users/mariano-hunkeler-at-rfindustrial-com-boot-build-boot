#ifndef BOOT_CONFIG_H
#define BOOT_CONFIG_H

#include <xc.h>

/*==========================================================================*/
/*  Constantes del bootloader (independientes de la placa)                   */
/*==========================================================================*/
#define BUFFER_LEN_LOD  64

/* Geometría de flash (PIC24FJ64GA002): erase page = 512 instr = 0x400 PC */
#define FLASH_PAGE_ERASE_PC   0x0400UL

#define BOOT_PTR_BASE              0x9C00UL
#define BOOT_CODE_BASE             0xA000UL
#define LOADER_ADDR_PC             0x9C00UL   /* límite de escritura HEX app  */
#define ERASE_LIMIT_PC             0xA000UL   /* 'D': app + página de puntero */
#define POINTER_USER_PROGRAM_ADDR  0x9C00UL
#define ID_MODULO_ADDR             0xA000UL

#define BOOT_DEFAULT_APP_ENTRY     0x0200UL

/*==========================================================================*/
/*  Configuración dependiente de la placa                                    */
/*==========================================================================*/
#ifdef BOOT_FROM_PCB
/* ---- F2: fuente única = PCB (headers generados por EMIC-Codify) ---- */
  #include "system.h"        /* FOSC, FCY */
  #include "pins.h"          /* TRIS_Led1/LAT_Led1/CN_SCL/... (setPin.emic) */
  #include "boot_board.h"    /* nombrePCB + BOOT_I2C_PORT (emitidos por generate.emic) */

  #define PIN_LED1_LAT    LAT_Led1
  #define PIN_LED1_TRIS   TRIS_Led1
  #define PIN_SCL_TRIS    TRIS_SCL
  #define PIN_SCL_LAT     LAT_SCL
  #define PIN_SDA_TRIS    TRIS_SDA
  #define PIN_SDA_LAT     LAT_SDA

  #define _BOOT_PUE2(n)   _CN##n##PUE
  #define _BOOT_PUE(n)    _BOOT_PUE2(n)
  #define PIN_SCL_CNPU    _BOOT_PUE(CN_SCL)
  #define PIN_SDA_CNPU    _BOOT_PUE(CN_SDA)

#else
/* ---- F1: standalone HRD_ETHERNET (sin EMIC) ---- */
  #define FOSC            32000000UL
  #define FCY             (FOSC/2)
  #define nombrePCB       "HRD_ETHERNET"
  #define BOOT_I2C_PORT   1

  #define PIN_LED1_LAT    LATBbits.LATB7
  #define PIN_LED1_TRIS   TRISBbits.TRISB7
  #define PIN_SCL_TRIS    TRISBbits.TRISB6
  #define PIN_SCL_LAT     LATBbits.LATB6
  #define PIN_SCL_CNPU    CNPU2bits.CN24PUE   /* RB6 = CN24 */
  #define PIN_SDA_TRIS    TRISBbits.TRISB5
  #define PIN_SDA_LAT     LATBbits.LATB5
  #define PIN_SDA_CNPU    CNPU2bits.CN27PUE   /* RB5 = CN27 */
#endif

#endif /* BOOT_CONFIG_H */


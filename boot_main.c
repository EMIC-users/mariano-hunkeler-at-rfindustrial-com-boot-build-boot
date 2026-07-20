
#include <xc.h>
#include "inc/boot_config.h"      /* define FCY antes de libpic30 */
#include <libpic30.h>             /* __delay_ms / _FLASH_*_CODE */
#include "inc/boot_id.h"

/*==========================================================================*/
/*  Fuses.  F2: desde la PCB (systemConfig.h).  F1: inline (HRD_ETHERNET).    */
/*==========================================================================*/
#ifdef BOOT_FROM_PCB
#include "inc/systemConfig.h"
#else
#pragma config POSCMOD  = NONE
#pragma config I2C1SEL  = SEC
#pragma config IOL1WAY  = ON
#pragma config OSCIOFNC = ON
#pragma config FCKSM    = CSDCMD
#pragma config FNOSC    = FRCPLL
#pragma config SOSCSEL  = SOSC
#pragma config WUTSEL   = LEG
#pragma config IESO     = OFF
#pragma config WDTPS    = PS32768
#pragma config FWPSA    = PR128
#pragma config WINDIS   = OFF
#pragma config FWDTEN   = OFF
#pragma config ICS      = PGx1
#pragma config GWRP     = OFF
#pragma config GCP      = OFF
#pragma config JTAGEN   = OFF
#endif

/*==========================================================================*/
/*  Alias de registros I2C según puerto                                      */
/*==========================================================================*/
#if (BOOT_I2C_PORT == 2)
  #define I2CxCONbits   I2C2CONbits
  #define I2CxSTATbits  I2C2STATbits
  #define I2CxRCV       I2C2RCV
  #define I2CxTRN       I2C2TRN
  #define I2CxADD       I2C2ADD
  #define I2CxBRG       I2C2BRG
#else
  #define I2CxCONbits   I2C1CONbits
  #define I2CxSTATbits  I2C1STATbits
  #define I2CxRCV       I2C1RCV
  #define I2CxTRN       I2C1TRN
  #define I2CxADD       I2C1ADD
  #define I2CxBRG       I2C1BRG
#endif

/*==========================================================================*/
/*  ID del módulo en flash (zona bootinfo). [VALIDAR-HW] packing prog-space   */
/*==========================================================================*/
const __attribute__((space(prog), address(ID_MODULO_ADDR)))
unsigned char g_moduleId[8] = BOOT_MODULE_ID;

/*==========================================================================*/
/*  Helpers de Flash (basados en _hal/Flash, adaptados a word arbitrario)    */
/*==========================================================================*/
static void flashReadWord(uint32_t pc, uint16_t *lo, uint8_t *hi)
{
    TBLPAG = (uint16_t)(pc >> 16);
    uint16_t off = (uint16_t)(pc & 0xFFFF);
    *lo = __builtin_tblrdl(off);
    *hi = (uint8_t)__builtin_tblrdh(off);
}

static void flashWriteWord(uint32_t pc, uint16_t lo, uint8_t hi)
{
    NVMCON = _FLASH_WRITE_WORD_CODE;           /* word program (0x4003) */
    TBLPAG = (uint16_t)(pc >> 16);
    uint16_t off = (uint16_t)(pc & 0xFFFF);
    __builtin_tblwtl(off, lo);
    __builtin_tblwth(off, hi);
    __builtin_disi(6);
    __builtin_write_NVM();
    while (NVMCONbits.WR);
}

static void flashErasePage(uint32_t pc)
{
    NVMCON = _FLASH_ERASE_CODE;                /* page erase (0x4042) */
    TBLPAG = (uint16_t)(pc >> 16);
    uint16_t off = (uint16_t)(pc & 0xFF00);    /* alinear a la página */
    __builtin_tblwtl(off, 0);
    __builtin_disi(6);
    __builtin_write_NVM();
    while (NVMCONbits.WR);
}

/*==========================================================================*/
/*  I2C  (register-direct, equivalente a las macros boot_* del CCS)          */
/*==========================================================================*/
unsigned char primerByte = 0;
unsigned char I2Chit, I2Cdata;

static void bootI2cInit(void)
{
    AD1PCFG = 0xFFFF;

    /* pull-ups internos en SCL/SDA (por número CN del pin, desde la PCB) */
    PIN_SCL_CNPU = 1;
    PIN_SDA_CNPU = 1;

    /* pulso de reset de bus en SDA (igual que el CCS) */
    PIN_SDA_LAT  = 0;
    PIN_SDA_TRIS = 0;
    __delay_ms(10);
    PIN_SDA_TRIS = 1;
    PIN_SCL_TRIS = 1;

    /* baudrate ~100 kHz (misma fórmula que el HAL I2C) */
    I2CxBRG = (uint16_t)(FCY / ((uint32_t)100000) - 1);

    I2CxADD             = 0;       /* esclavo en addr 0 + general call */
    I2CxCONbits.STREN   = 0;
    I2CxCONbits.GCEN    = 1;
    I2CxCONbits.DISSLW  = 1;
    I2CxCONbits.I2CSIDL = 0;
    I2CxCONbits.I2CEN   = 1;       /* habilita el módulo y toma SCL/SDA */
}

static void bootI2cPoll(void)
{
    I2Chit = 0;
    if (I2CxSTATbits.I2COV) { I2CxSTATbits.I2COV = 0; }
    if (I2CxSTATbits.RBF)
    {
        I2Cdata = I2CxRCV;
        if (!I2CxSTATbits.D_A) { primerByte = 1; }       /* byte de dirección */
        else { if (primerByte) primerByte = 0; else I2Chit = 1; }
    }
}

#define I2C_FLUSH_RX()  do { while (I2CxSTATbits.RBF) I2Cdata = I2CxRCV; } while (0)

static void i2cWriteByte(unsigned char d)
{
    I2CxTRN = d;
    while (I2CxSTATbits.TRSTAT);   /* espera fin de transmisión */
}

static void bootWriteI2C(const char *datos)
{
    while (I2CxSTATbits.S);                 /* espera bus libre */
    I2CxCONbits.SEN = 1; while (I2CxCONbits.SEN);    /* Start */

    i2cWriteByte(0x00); I2C_FLUSH_RX();     /* direccionamiento (addr 0)  */
    i2cWriteByte(0x00); I2C_FLUSH_RX();
    i2cWriteByte(0x00); I2C_FLUSH_RX();
    i2cWriteByte(0x01); I2C_FLUSH_RX();
    i2cWriteByte('\t'); I2C_FLUSH_RX();
    while (*datos > 0) { i2cWriteByte((unsigned char)*datos); I2C_FLUSH_RX(); datos++; }
    i2cWriteByte(0x00); I2C_FLUSH_RX();

    I2CxCONbits.PEN = 1; while (I2CxCONbits.PEN);     /* Stop */
    while (!I2CxSTATbits.P);
    I2C_FLUSH_RX();
}

/*==========================================================================*/
/*  Conversión ASCII-hex de 2 caracteres a byte (= atoi_b16 del CCS)         */
/*==========================================================================*/
static unsigned char atoi_b16(const char *s)
{
    unsigned char h = (s[0] >= 'A') ? (s[0] - 'A' + 10) : (s[0] - '0');
    unsigned char l = (s[1] >= 'A') ? (s[1] - 'A' + 10) : (s[1] - '0');
    return (unsigned char)((h << 4) + l);
}

/*==========================================================================*/
/*  Variables globales (como en el CCS)                                      */
/*==========================================================================*/
unsigned char  checksum, line_type;
uint16_t       l_addr, h_addr = 0;
uint32_t       addr;
uint32_t       next_addr;
unsigned char  dataidx, i;
unsigned char  count = 0;
unsigned char  data[32];
uint32_t       address_erase;
unsigned char  sendOk = 0;

uint16_t       buffidx = 0;
char           buffer[BUFFER_LEN_LOD];

/*==========================================================================*/
int main(void)
{
    char     miMod[]  = nombrePCB;
    char     miVer[10 + sizeof(miMod)];
    char     miOk[4]  = { 'K', '0', '0', 0 };
    char     miNo[2]  = { 'N', 0 };
    uint16_t addressPrograma = 0xFFFF, addressProgramaTemp = 0xFFFF;
    uint16_t miLoop = 50000;
    unsigned char x;
    unsigned char miEstado = 0;
    uint16_t wlo; uint8_t whi;

    CLKDIVbits.RCDIV = 0;

    bootI2cInit();

    PIN_LED1_TRIS = 0;
    PIN_LED1_LAT  = 0;

    /* entry de la app guardada en flash (low word del puntero) */
    flashReadWord(POINTER_USER_PROGRAM_ADDR, &wlo, &whi);
    addressPrograma = wlo;

    miVer[0] = 'V'; miVer[1] = ':';
    flashReadWord(ID_MODULO_ADDR + 0, &wlo, &whi);
    miVer[2] = (char)(wlo & 0xFF);
    miVer[3] = (char)(wlo >> 8);
    flashReadWord(ID_MODULO_ADDR + 2, &wlo, &whi);
    miVer[4] = (char)(wlo & 0xFF);
    miVer[5] = (char)(wlo >> 8);
    flashReadWord(ID_MODULO_ADDR + 4, &wlo, &whi);
    miVer[6] = (char)(wlo & 0xFF);
    miVer[7] = (char)(wlo >> 8);
    miVer[8] = ',';
    for (x = 0; x < sizeof(miMod); x++)
        miVer[x + 9] = miMod[x];
    miVer[x + 8] = 0;

    while (miLoop || addressPrograma == 0xFFFF)
    {
        bootI2cPoll();
        if (I2Chit == 1)
        {
            buffer[buffidx] = I2Cdata;
            if (buffer[buffidx] == 0)            /* fin de trama */
            {
                if (buffer[0] == 4)              /* trama de bootloader */
                {
                    miOk[1] = '0';
                    miOk[2] = '0';

                    switch (buffer[2])
                    {
                    case 'B':                    /* conectar */
                        if (miEstado == 0)
                        {
                            PIN_LED1_LAT = 1;
                            miLoop   = 50000;
                            miEstado = 1;
                        }
                        break;
                    case 'R':                    /* reconectar */
                        miLoop   = 50000;
                        miEstado = 1;
                        break;
                    case 'V':                    /* versión */
                        if (miEstado == 1)
                        {
                            PIN_LED1_LAT ^= 1;
                            bootWriteI2C(miVer);
                        }
                        break;
                    case 'I':                    /* identificar (6 chars) */
                        if (miEstado == 1 || miEstado == 2)
                        {
                            if (buffer[4] == miVer[2] && buffer[5] == miVer[3] &&
                                buffer[6] == miVer[4] && buffer[7] == miVer[5] &&
                                buffer[8] == miVer[6] && buffer[9] == miVer[7])
                            {
                                PIN_LED1_LAT = 1;
                                miEstado = 2;
                                sendOk   = 1;
                            }
                        }
                        break;
                    case 'P':                    /* entrar a programación */
                        if (miEstado == 1 || miEstado == 2)
                        {
                            if (buffer[4] == miVer[2] && buffer[5] == miVer[3] &&
                                buffer[6] == miVer[4] && buffer[7] == miVer[5] &&
                                buffer[8] == miVer[6] && buffer[9] == miVer[7])
                            {
                                miEstado = 3;
                                sendOk   = 1;
                            }
                        }
                        break;
                    case 'W':                    /* escribir línea Intel-HEX */
                        if (miEstado == 3)
                        {
                            if (buffer[3] == ':')
                            {
                                count     = atoi_b16(buffer + 4);
                                l_addr    = (uint16_t)(((uint16_t)atoi_b16(buffer + 6) << 8) |
                                                        atoi_b16(buffer + 8));
                                line_type = atoi_b16(buffer + 10);
                                addr      = ((uint32_t)h_addr << 16) | l_addr;
                                addr     /= 2;       /* HEX byte-addr -> word-addr */

                                miOk[1] = buffer[buffidx - 2];
                                miOk[2] = buffer[buffidx - 1];

                                if (line_type == 1)          /* EOF */
                                {
                                    PIN_LED1_LAT = 1;
                                    if (addressProgramaTemp == 0xFFFF)
                                        addressProgramaTemp = BOOT_DEFAULT_APP_ENTRY;
                                    addressPrograma = addressProgramaTemp;
                                    flashErasePage(POINTER_USER_PROGRAM_ADDR);
                                    flashWriteWord(POINTER_USER_PROGRAM_ADDR,
                                                   addressPrograma, 0x00);
                                    miEstado = 2;
                                    sendOk   = 1;
                                }
                                else if (addr < LOADER_ADDR_PC)
                                {
                                    checksum = 0;
                                    for (i = 4; i < (buffidx - 2); i += 2)
                                        checksum += atoi_b16(buffer + i);
                                    checksum = (unsigned char)(0xFF - checksum + 1);

                                    if (checksum != atoi_b16(buffer + buffidx - 2))
                                    {
                                        bootWriteI2C(miNo);    /* checksum mal */
                                    }
                                    else
                                    {
                                        if (line_type == 0)
                                        {
                                            for (i = 12, dataidx = 0; i < buffidx - 2; i += 2)
                                                data[dataidx++] = atoi_b16(buffer + i);

                                            if (addr == 0)
                                            {
                                                /* captura entry de la app del GOTO del reset */
                                                addressProgramaTemp =
                                                    (uint16_t)(((uint16_t)atoi_b16(buffer + 14) << 8) |
                                                                atoi_b16(buffer + 12));
                                            }
                                            else if (addr != POINTER_USER_PROGRAM_ADDR)
                                            {
                                                uint32_t a;
                                                for (a = 0; a < count; a += 4)
                                                {
                                                    uint16_t lo = (uint16_t)(data[a] | (data[a + 1] << 8));
                                                    uint8_t  hi = data[a + 2];
                                                    flashWriteWord(addr + (a / 2), lo, hi);
                                                }
                                            }
                                        }
                                        else if (line_type == 4)
                                        {
                                            h_addr = (uint16_t)(((uint16_t)atoi_b16(buffer + 12) << 8) |
                                                                 atoi_b16(buffer + 14));
                                        }
                                        sendOk = 1;
                                        PIN_LED1_LAT ^= 1;
                                    }
                                }
                                else
                                {
                                    sendOk = 1;   /* zona del bootloader: solo ACK */
                                }
                            }
                        }
                        break;
                    case 'D':                    /* borrar la zona de app */
                        if (miEstado == 3)
                        {
                            uint16_t rv0lo, rv2lo;
                            uint8_t  rv0hi, rv2hi;
                            flashReadWord(0, &rv0lo, &rv0hi);
                            flashReadWord(2, &rv2lo, &rv2hi);

                            addressPrograma     = 0xFFFF;
                            addressProgramaTemp = 0xFFFF;
                            for (address_erase = 0; address_erase < ERASE_LIMIT_PC;
                                 address_erase += FLASH_PAGE_ERASE_PC)
                                flashErasePage(address_erase);

                            flashWriteWord(0, rv0lo, rv0hi);
                            flashWriteWord(2, rv2lo, rv2hi);
                            sendOk = 1;
                        }
                        break;
                    case 'E':                    /* salir */
                        if (miEstado)
                        {
                            PIN_LED1_LAT = 0;
                            miEstado = 0;
                            miLoop   = 0;
                        }
                        break;
                    }

                    if (sendOk)
                    {
                        sendOk = 0;
                        bootWriteI2C(miOk);
                    }
                }
                buffidx = 0;
            }
            else
            {
                buffidx++;
            }
        }
        if (miEstado == 0 && miLoop)
        {
            miLoop--;
            __delay_us(10);
        }
    }

    if (addressPrograma)
    {
        I2CxCONbits.I2CEN = 0;   /* handoff limpio: apagar I2C antes de saltar */
        ((void (*)(void))addressPrograma)();   /* salto a la app */
    }

    while (1);   /* nunca retorna */
    return 0;
}


#include "settings.h"
#include "eeprom.h"

uint16_t VirtAddVarTab[NB_OF_VAR] = {0x5555, 0x6666, 0x7777};
uint16_t VarDataTab[NB_OF_VAR] = {0,0,0};

const uint8_t default_net_addr = 1;
const uint16_t default_platform_weight = 0;
const uint16_t default_coeff = 8941;

void init_settings(void)
{
    EE_Init();
}

unsigned char get_net_address(void)
{
    uint16_t status = EE_ReadVariable(VirtAddVarTab[0], &VarDataTab[0]);
    if(status!=0) {EE_WriteVariable(VirtAddVarTab[0],default_net_addr);return default_net_addr;}
    return (unsigned char)VarDataTab[0];
}

void set_net_address(unsigned char value)
{
    VarDataTab[0] = value;
    EE_WriteVariable(VirtAddVarTab[0], VarDataTab[0]);
}

unsigned short get_platform_weight(void)
{
    uint16_t status = EE_ReadVariable(VirtAddVarTab[1], &VarDataTab[1]);
    if(status!=0) {EE_WriteVariable(VirtAddVarTab[1],default_platform_weight);return default_platform_weight;}
    return (unsigned short)VarDataTab[1];
}

void set_platform_weight(unsigned short value)
{
    VarDataTab[1] = value;
    EE_WriteVariable(VirtAddVarTab[1], VarDataTab[1]);
}

unsigned short get_coeff(void)
{
    uint16_t status = EE_ReadVariable(VirtAddVarTab[2], &VarDataTab[2]);
    if(status!=0) {EE_WriteVariable(VirtAddVarTab[2],default_coeff);return default_coeff;}
    return (unsigned short)VarDataTab[2];
}

void set_coeff(unsigned short value)
{
    VarDataTab[2] = value;
    EE_WriteVariable(VirtAddVarTab[2], VarDataTab[2]);
}

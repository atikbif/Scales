#ifndef SETTINGS_H_INCLUDED
#define SETTINGS_H_INCLUDED

void init_settings(void);
unsigned char get_net_address(void);
void set_net_address(unsigned char value);
unsigned short get_platform_weight(void);
void set_platform_weight(unsigned short value);
unsigned short get_coeff(void);
void set_coeff(unsigned short value);

#endif /* SETTINGS_H_INCLUDED */

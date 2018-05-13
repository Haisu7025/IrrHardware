#ifndef __SIM_H
#define __SIM_H
#include "sys.h"

void link_gpio_init(void);
void reset_gpio_init(void);
void reset_SIM(void);
void soft_reset_SIM(void);

void modify_heartbeat_time(u16 time);
void modify_heartbeat_content(char *content);
void modify_SIM_server(char *ip_address, u16 ip_len, u16 port);
void module_restart(void);
void modify_cmnet(void);
void save_modify(void);
void SIM_module_init(void);
void enter_com_mode(void);
void enter_configuration_mode(void);
void enter_sleep_mode(void);
void init_hbid_and_slptim(char *content);

#endif

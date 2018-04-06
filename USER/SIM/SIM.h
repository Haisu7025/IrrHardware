#ifndef __SIM_H
#define __SIM_H
#include "sys.h"

void modify_heartbeat_time(u16 time);
void modify_heartbeat_content(char* content);
void modify_SIM_server(char *ip_address, u16 ip_len, u16 port);
void module_restart();
void modify_cmnet();
void save_modify();
void SIM_module_init();
void enter_com_mode();
void enter_configuration_mode();

#endif

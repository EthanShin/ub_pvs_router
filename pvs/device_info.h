#ifndef __DEVICE_INFO_H__
#define __DEVICE_INFO_H__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>		//sockaddr_in 구조체 사용
#include <net/if.h> 
#include <netdb.h>
#include <time.h>
#include <sys/ioctl.h>
#include <sys/sysinfo.h>
#include "utility.h"

#define WAN_INF "eth1"


int get_dev_mac(char *data);
int get_period();
int get_uptime(char *uptime);
int get_free(char *free_mem);
int get_state();
int get_op_mode();
int get_booting_num();
int get_config_ver();
int get_fw_ver(char *value);
int fw_ver_check(int mode);
int get_device_status(struct s_device_status_info* s_dsi);

#endif /* __DEVICE_INFO_H__ */

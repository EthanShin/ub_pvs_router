
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>		//sockaddr_in 구조체 사용
#include <net/if.h> 
#include <netdb.h>
#include <signal.h>
#include <time.h>
#include <sys/ioctl.h>

#define STATUS_TOPIC    	"PVS/device/log"     //router -> server
#define FW_UP_TOPIC     	"PVS/server/fw_up"      //server -> router
#define FW_UP_GET_TOPIC 	"PVS/device/fw_up"      //router -> server
#define CONFIG_TOPIC    	"PVS/server/config"     //server -> router
#define CONFIG_GET_TOPIC	"PVS/device/config"     //router -> server
#define COMMAND_TOPIC   	"PVS/server/command"    //server -> router
#define ERROR_PUT_TOPIC		"PVS/device/error"     //router -> server

#define TRACE(format,arg...)    printf(  format , ##arg)


#define OK_PV				0x01
#define OK_PV_PASS_TX			0x02
#define ERROR_PV_CONF_INFO		0x03
#define ERROR_PV_FW_MD5SUM_MISMATCH	0x04
#define ERROR_PV_FW_DOWNLOAD		0x05
#define ERROR_PV_FW_WRITE		0x06

#define DIFF_FW_VER 0x11
#define SAME_FW_VER 0x12

#define SERVER_IP_COUNT 4
#define MAX_IP_LEN 25
struct s_device_config_info{
	char 	conf_ver[5];
	char 	op_mode[5];
	char	ip_addr[MAX_IP_LEN];
	char 	dhcp_client_start[MAX_IP_LEN];
	char 	dhcp_client_end[MAX_IP_LEN];
	char 	pvs_period[5];
};

struct s_firmware_info{
	char 	fw_ver[16];
	char 	fw_name[64];
	char	fw_md5[256];
	char	fw_download_path[256];
};
struct s_topic{
	char 	status_info[64];
	char 	fw_info[64];
	char 	fw_info_get[64];
	char 	config_info[64];
	char 	config_info_get[64];
	char 	command[64];
	char 	name[32];
	char 	error[64];
};

struct s_device_status_info{
	short 	op_mode;
	char 	model_name[64];
	char 	fw_ver[10];
	short 	config_ver;
	short 	state;
	char 	dev_mac[36];
	char 	wan_ip[36];
	char	uptime[36];
	char	free_mem[36];
	short	booting_num;
	char	ap_list[4][13];
};	

typedef int result_t;

enum
{
	R_SUCCESS		=	0,
	R_FAIL			=	1,
	R_ERROR_TIMEOUT		=	2,
	R_ERROR_DUPLICATE	=	3, 
	R_DEV_CHANGED		=	4, 
	R_DEV_NOT_CHANGED	=	5, 
};
enum
{
	TEXT = 0,
	HEX,
};


enum
{
	IP_ADDR,
	HW_ADDR,


};


static char SERVER_IP[SERVER_IP_COUNT][50]=
{
	"www.baruntechpvs.com",
	"192.168.5.200",
	"192.168.10.185",
	"175.197.148.220"
	"www.baruntechpvs.com",

};


int fw_upgrade(struct s_firmware_info *s_fwi);
int firmware_check(struct s_firmware_info* s_fwi);
int getAddrByDomain(char *domain,char *out_ip);
int getInAddr(void *pAddr,int type,char *wan_inf );
void TRACEN(char *comment,int len,char *data,int mode);
int flash_get(char *name,char *value);
#endif



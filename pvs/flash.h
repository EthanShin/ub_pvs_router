
#ifndef __FLASH_H__
#define __FLASH_H__

#include "utility.h"
#include "device_info.h"
#include "../boa/apmib/apmib.h"
#include "../boa/apmib/mibdef.h"

#define UBTECH  16600

//#define MIB_CONFIG_VER                  (UBTECH + 83)
//#define MIB_STATE                       (UBTECH + 84)
//#define MIB_BOOTING_NUM                 (UBTECH + 85)
//#define MIB_PVS_PERIOD                  (UBTECH + 86)
//#define MIB_FW_VER                      (UBTECH + 87)

#define d_conf_ver 		MIB_CONFIG_VER
#define d_op_mode		MIB_OP_MODE
#define d_ip_addr 		MIB_IP_ADDR
#define d_dhcp_client_start 	MIB_DHCP_CLIENT_START
#define d_dhcp_client_end 	MIB_DHCP_CLIENT_END
#define d_pvs_period 		MIB_PVS_PERIOD
	



int device_config_flash_set(struct s_device_config_info s_dci);
int firmware_check(struct s_firmware_info* s_fwi);
int set_state(int value);
int set_booting_num(int value);
int flash_set(char *field, char *data);

#endif /* __FLASH_H__ */

#include "utility.h"
#include "json.h"
void dci_parser(char *message,struct s_device_config_info* s_dci);
void fwi_parser(char *message,struct s_firmware_info* s_fwi);
void make_device_info(struct s_device_status_info *s_dsi,char* data);

#include "parser.h"

void dci_parser(char *message,struct s_device_config_info* s_dci)
{
	get_json_data(message,"config_ver",s_dci->conf_ver);
	get_json_data(message,"op_mode",s_dci->op_mode);
	get_json_data(message,"ip_addr",s_dci->ip_addr);
	get_json_data(message,"dhcp_client_start",s_dci->dhcp_client_start);
	get_json_data(message,"dhcp_client_end",s_dci->dhcp_client_end);
	get_json_data(message,"pvs_period",s_dci->pvs_period);
  	return ;
}

void fwi_parser(char *message,struct s_firmware_info* s_fwi)
{
	get_json_data(message,"fw_ver",s_fwi->fw_ver);
	get_json_data(message,"fw_name",s_fwi->fw_name);
	get_json_data(message,"fw_md5",s_fwi->fw_md5);
	get_json_data(message,"fw_download_path",s_fwi->fw_download_path);
	//printf("%s \n%s |%s| %s| %s\n",message,s_fwi->fw_ver,s_fwi->fw_name,s_fwi->fw_md5,s_fwi->fw_download_path);
	return ;
}

void make_device_info(struct s_device_status_info *s_dsi,char *data)
{	
	sprintf(data,"op_mode:%d,model_name:%s,fw_ver:%s,config_ver:%d,state:%d,dev_mac:%s,wan_ip:%s,uptime:%s,free_mem:%s,booting_num:%d",s_dsi->op_mode,s_dsi->model_name,s_dsi->fw_ver,s_dsi->config_ver,s_dsi->state,s_dsi->dev_mac,s_dsi->wan_ip,s_dsi->uptime,s_dsi->free_mem,s_dsi->booting_num);
	printf("data [%s]\n",data);
}


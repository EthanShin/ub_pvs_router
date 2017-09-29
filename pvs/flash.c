#include "flash.h"


int device_config_flash_set(struct s_device_config_info s_dci)
{
	struct in_addr ip_addr, dhcp_client_start,dhcp_client_end;
	int op_mode,pvs_period,config_ver;
	char tmpBuf[256]={0,};
	if(atoi(s_dci.conf_ver)<=get_config_ver())
	{
		printf("not update current config ver %d %d\n",get_config_ver(),atoi(s_dci.conf_ver));
		set_state(1);
		return  R_SUCCESS;
	}
	else
	{
		apmib_init();
		if(strstr(s_dci.op_mode,"#")==NULL)
		{
			op_mode=atoi(s_dci.op_mode);
			
			if(!apmib_set(MIB_OP_MODE,(void *)&op_mode))
			{
				strcpy(tmpBuf, ("Set OP_MODE error!"));
				goto error;
			}
			//	flash_set("OP_MODE",&s_dci.op_mode[0]);
		}
		if(strstr(s_dci.ip_addr,"#")==NULL)
		{
			
			if ( !inet_aton(s_dci.ip_addr, &ip_addr) ) 
			{
				strcpy(tmpBuf, ("Invalid DHCP client start address!"));
				goto error;
			}
			if ( !apmib_set(MIB_IP_ADDR, (void *)&ip_addr)) 
			{
				strcpy(tmpBuf, ("Set DHCP client start address error!"));
				goto error;
			}
			//apmib_set(MIB_IP_ADDR,(void *)&s_dci.ip_addr);
			//flash_set("IP_ADDR",&s_dci.ip_addr[0]);
			
		}
		if(strstr(s_dci.dhcp_client_start,"#")==NULL)
		{
			
			if ( !inet_aton(s_dci.dhcp_client_start, &dhcp_client_start) ) 
			{
				strcpy(tmpBuf, ("Invalid DHCP client start address!"));
				goto error;
			}
			if ( !apmib_set(MIB_DHCP_CLIENT_START, (void *)&dhcp_client_start)) 
			{
				strcpy(tmpBuf, ("Set DHCP client start address error!"));
				goto error;
			}
			//	apmib_set(MIB_DHCP_CLIENT_START,(void *)&s_dci.dhcp_client_start);
			//flash_set("DHCP_CLIENT_START",&s_dci.dhcp_client_start[0]);
		}
		if(strstr(s_dci.dhcp_client_end,"#")==NULL)
		{
			
			if ( !inet_aton(s_dci.dhcp_client_end, &dhcp_client_end) ) 
			{
				strcpy(tmpBuf, ("Invalid DHCP client start address!"));
				goto error;
			}
			if ( !apmib_set(MIB_DHCP_CLIENT_END, (void *)&dhcp_client_end)) 
			{
				strcpy(tmpBuf, ("Set DHCP client start address error!"));
				goto error;
			}
			//	apmib_set(MIB_DHCP_CLIENT_END,(void *)&s_dci.dhcp_client_end);
			//flash_set("DHCP_CLIENT_END",&s_dci.dhcp_client_end[0]);
		}
		if(strstr(s_dci.pvs_period,"#")==NULL)
		{	
			pvs_period=atoi(s_dci.pvs_period);
			printf("pvs_period = %d, dci_pvs_period = %s\n", pvs_period, s_dci.pvs_period);
			if(!apmib_set(MIB_PVS_PERIOD,(void *)&pvs_period))
			{
				strcpy(tmpBuf, ("Set PVS_PERIOD ERROR"));
				goto error;
			}
			//flash_set("PVS_PERIOD",&s_dci.pvs_period[0]);
		}
		config_ver=atoi(s_dci.conf_ver);
		if ( !apmib_set(MIB_CONFIG_VER,(void *)&config_ver))
		{
			strcpy(tmpBuf, ("Set CONF_VER  error!"));
			goto error;
		}
		//	flash_set("CONFIG_VER",&s_dci.conf_ver[0]);
		apmib_update(CURRENT_SETTING);
		system("reboot");
		return	R_SUCCESS;
		
		error:
		printf("error:[%s\n]",tmpBuf);
		return R_FAIL;
		
	}
}
int firmware_check(struct s_firmware_info* s_fwi)
{
	char temp[8]={0,};
	get_fw_ver(&temp[0]);
	if(strcmp(s_fwi->fw_ver,temp)<=0)
	{
		printf("not update firmware ver %s\n",temp);
		return  R_SUCCESS;
	}
	else
	{
		return fw_upgrade(s_fwi);
	}
}


int flash_set(char *field, char *data)
{
	char nvram_cmd[128]={0,};
	sprintf(nvram_cmd,"/bin/flash set %s %s",field,data);
	system(nvram_cmd);
	return R_SUCCESS;
}
int set_state(int value)
{
	apmib_init();
	apmib_set(MIB_STATE,(void *)&value);
	apmib_update(CURRENT_SETTING);
	return R_SUCCESS;
}
int set_booting_num(int value)
{
	apmib_init();
	apmib_set(MIB_BOOTING_NUM,(void *)&value);
	apmib_update(CURRENT_SETTING);
	return R_SUCCESS;
}


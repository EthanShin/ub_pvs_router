#include "device_info.h"

int get_dev_mac(char * data)
{
	unsigned char *pMacAddr;
	char temp[32];
 	struct sockaddr hwaddr;
	memset(&temp,0x00,32);
	if(getInAddr((void *)&hwaddr,HW_ADDR,WAN_INF))
	{
		pMacAddr = (unsigned char *)hwaddr.sa_data;
	        sprintf(temp, "%02x%02x%02x%02x%02x%02x\0", pMacAddr[0], pMacAddr[1],pMacAddr[2], pMacAddr[3], pMacAddr[4], pMacAddr[5]);

		int index;
        for(index = 0; temp[index] != '\0'; index++) {
            if(islower(temp[index])) {
                temp[index] = toupper(temp[index]);
            }
        }

		strncpy(data,temp,strlen(temp));
		printf("mac : %s \n",data);
		return R_SUCCESS;
	}
        else
	{
        	strcpy(data,"000000000000");
		printf("mac : %s \n",data);
		return R_SUCCESS;
	}
}
int get_uptime(char *uptime)
{
	struct sysinfo info ;

	unsigned long sec, mn, hr, day;


        sysinfo(&info);
        sec = (unsigned long) info.uptime ;
        day = sec / 86400;
        sec %= 86400;
        hr = sec / 3600;
        sec %= 3600;
        mn = sec / 60;
        sec %= 60;

        sprintf(uptime,"%ld일%ld시%ld분%ld초",day, hr, mn, sec);

	return R_SUCCESS;
}


int get_free(char *free_mem)
{
        FILE *fp;
        char buf[512]={0,};
        int i,j,len,change;
        char old_char;
        //if((fp = popen("free | grep Mem | cut -f27 -d ' '","r"))!=NULL)
        if((fp = popen("free | grep Mem","r"))!=NULL)
        {
                if(fgets(buf,512,fp)!=NULL)
                {
                        len = strlen(buf);
                        i=0;
                        change=0;
                        old_char=' ';

                        while(i++ < len)
                        {
                                if(old_char !=  buf[i] && buf[i]!=' ' && old_char==' ')
                                        change++;

                                if(change == 4)
                                {
                                        j=0;
                                        while(i < len)
                                        {
                                                free_mem[j++] = buf[i++];
                                                if(buf[i] == ' ')
                                                {
                                                        pclose(fp);
                                                        return R_SUCCESS;
                                                }
                                        }
                                }
                                old_char = buf[i];
                        }
                }
                pclose(fp);
        }

        return R_SUCCESS;
}

int get_period()
{
        char str_value[6]={0,};
        if(flash_get("PVS_PERIOD",str_value)!=R_SUCCESS)
        {
                printf("get_state() fail\n)");
                return R_FAIL;
        }
        return  atoi(str_value);
}

int get_state()
{
	char str_value[4]={0,};
	if(flash_get("STATE",str_value)!=R_SUCCESS)
	{
		printf("get_state() fail\n)");		
		return R_FAIL;
	}
	return  atoi(str_value);
}
int get_op_mode()
{
        char str_value[4]={0,};
        if(flash_get("OP_MODE",str_value)!=R_SUCCESS)
        {
                printf("get_op_mode() fail\n)");
                return R_FAIL;
        }
        return  atoi(str_value);
}
int get_booting_num()
{
        char str_value[4]={0,};
        if(flash_get("BOOTING_NUM",str_value)!=R_SUCCESS)
        {
                printf("get_booting_num() fail\n)");
                return R_FAIL;
        }
        return  atoi(str_value);
}
int get_config_ver()
{
        char str_value[4]={0,};
        if(flash_get("CONFIG_VER",str_value)!=R_SUCCESS)
        {
                printf("CONFIG_VER fail\n)");
                return R_FAIL;
        }
        return  atoi(str_value);
}


int get_fw_ver(char *value)
{
	FILE *fp;
	char buffer[128]={0,};
	char model_type[128]={0,};
	char version[8]={0,};

	fp = fopen("/etc/version", "r");
	if(fp != NULL)
	{
		fgets(buffer, sizeof(buffer), fp);
		fclose(fp);
		sscanf(buffer,"%s %s", model_type, version);
		strcpy(value,version);
		printf("get_fw_ver : %s \n",value);
	}
	else
		strcpy(value,"Unknown");
	return R_SUCCESS;
}

int fw_ver_check(int mode)
{
	char fw_ver[8]={0,};
	char flash_fw_ver[8]={0,};
	char temp[16]={0,};
	flash_get("FW_VER",&flash_fw_ver[0]);
	get_fw_ver(&temp[0]);
	if(strcmp(flash_fw_ver,fw_ver)!=0)
	{
		return DIFF_FW_VER;
	}
	else
	{
		return SAME_FW_VER;
	}
}
int get_wan_ip(char *value)
{
	struct in_addr  ipaddr;
	if(getInAddr((void *)&ipaddr,IP_ADDR,WAN_INF)) 
        	sprintf(value,"%s",inet_ntoa(ipaddr));
        else
        	strcpy(value,"0.0.0.0");
}
int get_device_status(struct s_device_status_info* s_dsi)
{	

	s_dsi->op_mode=get_op_mode();
	printf("op_mode %d\n",s_dsi->op_mode);
	//s_dsi->model_name
	//strcpy(s_dsi->fw_ver,fw_ver);
	get_fw_ver(s_dsi->fw_ver);
	printf("fw_ver %s\n",s_dsi->fw_ver);

	s_dsi->config_ver=get_config_ver();
	printf("config ver %d\n",s_dsi->config_ver);
	s_dsi->state=get_state();
	printf("state %d\n", s_dsi->state);
	//strcpy(s_dsi->dev_mac,dev_mac);
	get_dev_mac(s_dsi->dev_mac);
	printf("dev_mac %s\n", s_dsi->dev_mac);
	//strcpy(s_dsi->wan_ip,wan_ip);
	get_wan_ip(s_dsi->wan_ip);

	printf("wan_ip %s\n", s_dsi->wan_ip);
	//strcpy(s_dsi->uptime,uptime);
	get_uptime(s_dsi->uptime);

	printf("uptime %s\n", s_dsi->uptime);
	//strcpy(s_dsi->free_mem,free_mem);
	get_free(s_dsi->free_mem);
	printf("free %s\n", s_dsi->free_mem);
	s_dsi->booting_num=get_booting_num();
	printf("booting %d \n",s_dsi->booting_num);
}

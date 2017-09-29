#include "utility.h"

void TRACEN(char *comment,int len,char *data,int mode)
{
	
	#ifdef DEBUG
	int i;
	printf("%s -> ",comment);
	for(i=0; i<len; i++)
	{
		if(mode==TEXT)
			printf("%c",data[i]);
		else
			printf("%02X ",data[i] & 0xFF);
	
	}
	printf("\n");
	#endif
}

int getAddrByDomain(char *domain, char *out_ip)
{
	 struct hostent *host_entry;
    int             ndx;

	host_entry = gethostbyname(domain);
	
	 if(host_entry == NULL)
	{
		strcpy(out_ip , SERVER_IP[0]);
		printf("pvs_addr : default value set(%s)",SERVER_IP[0]);
		return R_SUCCESS;
	} 
	for ( ndx = 0; NULL != host_entry->h_addr_list[ndx]; ndx++)
	{
		strcpy(out_ip , inet_ntoa( *(struct in_addr*)host_entry->h_addr_list[ndx]));
		return R_SUCCESS; 	
	}
}


int getInAddr(void *pAddr,int type,char *wan_inf )
{
	struct ifreq ifr;
	int skfd=0, found=0;
	struct sockaddr_in *addr;

	skfd = socket(AF_INET, SOCK_DGRAM, 0);
	if(skfd==-1)
		return 0;

	strcpy(ifr.ifr_name, wan_inf);

	if (ioctl(skfd, SIOCGIFFLAGS, &ifr) < 0)
	{
    		close( skfd );
		return (0);
	}

	if(type == IP_ADDR)
	{
		if (ioctl(skfd, SIOCGIFADDR, &ifr) == 0)
		{
			addr = ((struct sockaddr_in *)&ifr.ifr_addr);
			*((struct in_addr *)pAddr) = *((struct in_addr *)&addr->sin_addr);
			found = 1;
		}
	}
	else if(type == HW_ADDR)
	{
    		if (ioctl(skfd, SIOCGIFHWADDR, &ifr) >= 0)
		{
			memcpy(pAddr, &ifr.ifr_hwaddr, sizeof(struct sockaddr));
			found = 1;
		}	
	}

    close( skfd );
    return found;
}

int fw_upgrade(struct s_firmware_info *s_fwi)
{
	FILE *fp;
	char download_cmd[512]={0,};
	char buf[128]={0,};
	char md5sum_value[64]={0,};
	char md5sum_cmd[128]={0,};

	char *fw_path;
	char *fw_name;
	char *fw_md5;


	fw_path = s_fwi->fw_download_path;
	fw_name = s_fwi->fw_name;
	fw_md5 = s_fwi->fw_md5;
	//fw ver flash write code add plz
	sprintf(download_cmd,"/bin/wget -O /tmp/BASS_WEB %s/%s",fw_path,fw_name);
	printf("download_cmd : [%s]\n",download_cmd);
	system(download_cmd);
	
	sprintf(md5sum_cmd,"/bin/md5sum /tmp/BASS_WEB");
	printf("md5sum_cmd : [%s]\n",md5sum_cmd);
	
	if((fp = popen(md5sum_cmd,"r"))!=NULL)
	{
		if(fgets(buf,128,fp)!=NULL)
			strncpy(md5sum_value,buf,32);
		
		pclose(fp);
	}

	printf("now_md5sum  : [%s]\n",md5sum_value);
	printf("conf_md5sum : [%s]\n",fw_md5);

	if (!strcmp(md5sum_value, fw_md5) && strlen(md5sum_value) != 0)
	{
		printf("md5sum same goto mtd write\n");
		system("/bin/killall boa");
		system("/bin/killall fwd");
		system("/bin/boa -u 1");
	}
	else
	{
		if(strlen(md5sum_value) == 0)
		{
			printf("fw download fail\n");
			return ERROR_PV_FW_DOWNLOAD;
		}
		else
		{
			printf("md5sum mismatch\n");
			return ERROR_PV_FW_MD5SUM_MISMATCH;
		}
	}


	return ERROR_PV_FW_WRITE;//maybe don't return
}

int flash_get(char *name,char *value)
{
	FILE *fp;
	char buf[128]={0,};
	char nvram_cmd[128]={0,};
	int i,len;

	sprintf(nvram_cmd,"/bin/flash get %s",name);
	if((fp = popen(nvram_cmd,"r"))!=NULL)
	{
		if(fgets(buf,128,fp)!=NULL)
		{
			if(strncmp("Usage",buf,5))
			{
				i=0;
				len = strlen(buf);
				
				while(i<len)
				{
					if(buf[i]!='=')
						i++;
					else
						break;

				}
				i++;

				if(buf[i]=='"')
					strncpy(value,&buf[i+1],strlen(buf)-i-3);
				else
					strncpy(value,&buf[i],strlen(buf)-i-1);
			}
		}
		else
		{
			printf("flash get fail\n");
			pclose(fp);
			return R_FAIL;
		}

		pclose(fp);
	}
	

	return R_SUCCESS;
}


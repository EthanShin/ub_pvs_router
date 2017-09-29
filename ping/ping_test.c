#include "ping_test.h"

int get_period() {
	char str_value[6]={0,};
	if(flash_get("PVS_PERIOD", str_value) != 1) {
		printf("get_period() fail\n");
		return 0;
	}
	return atoi(str_value);
}

int flash_get(char *name, char *value) {
    FILE *fp;
    char buf[128] = {0,};
    char nvram_cmd[128] = {0,};
    int i, len;

    sprintf(nvram_cmd, "/bin/flash get %s", name);
    if((fp = popen(nvram_cmd, "r")) != NULL) {
        if(fgets(buf, 128, fp) != NULL) {
            if(strncmp("Usage", buf, 5)) {
                i = 0;
                len = strlen(buf);

                while(i<len) {
                    if(buf[i]!='=')
                        i++;
                    else
                        break;
                }             
				i++;

                if(buf[i]=='"')
                    strncpy(value, &buf[i+1], strlen(buf)-i-3);
                else
                    strncpy(value, &buf[i], strlen(buf)-i-1);
            }
        }
        else {
            printf("flash get fail\n");
            pclose(fp);
            return 0;
        }
        pclose(fp);
    }
    return 1;
}

void escapeStr(const char* str) {
	for(; *str != '\0'; str++) {
		if(*str == ':') {
			strcpy(str, str + 1);
			str--;
		}
	}
}

void get_dev_mac(char* mac_address) {
	char buf[128] = {0,};

    FILE *fp = NULL;
    fp = popen("ifconfig | grep eth1 | cut -d\"H\" -f2 | cut -d\" \" -f2", "r");
    if(fp == NULL) {
        perror("get data fail");
        return;
    }
    else {
        fgets(buf, sizeof(buf), fp);
        pclose(fp);
        strcpy(mac_address, buf);
    }
}

void ping_test() {
	char flash_cmd[20] = {0};
    char ip_array[5][16] = {0};
    char mac_array[5][13] = {0};
    int check[5] = {0};
    char cmd[30] = {0};
	char str_payload[512] = {0};
	char mosq_cmd[512] = {0};
	char mac_address[13] = {0};

	get_dev_mac(mac_address);
	mac_address[strlen(mac_address)-1] = '\0'; 
	escapeStr(mac_address);
	printf("mac_address = %s\n", mac_address);

    int i;
    for(i = 0; i < 5; i++) {
        sprintf(flash_cmd, "POE_AP%d_IP", i+1);
        printf("flash_cmd = %s\n", flash_cmd);
        if(flash_get(flash_cmd, ip_array[i])==1) {
			if(strcmp(ip_array[i], "")) {

				char temp[20] = {0,};
		        sprintf(flash_cmd, "POE_AP%d_MAC", i+1);
		        if(flash_get(flash_cmd, temp)==1) {
		            escapeStr(temp);
		            strcpy(mac_array[i], temp);
		            printf("temp = %s, mac = %s\n", temp, mac_array[i]);
		        }

            	sprintf(cmd, "ping -c1 -w1 %s", ip_array[i]);
        	    if(system(cmd)) {
    	            printf("fail\n");
					sprintf(str_payload, "{\"dead_device\":\"%s\"}", mac_array[i]);
					sprintf(mosq_cmd, "mosquitto_pub -h www.baruntechpvs.com -t PVS/device/ping/%s -m %s", mac_address, str_payload);
				    system(mosq_cmd);

					char power[5] = {0,};
					switch(i) {
						case 0: strcpy(power, "0x9f"); break;
						case 1: strcpy(power, "0x99"); break;
						case 2: strcpy(power, "0x9b"); break;
						case 3: strcpy(power, "0x9d"); break;
						case 4: strcpy(power, "0x9e"); break;
					}
					setRegist(power, "0x00");
					sleep(1);
					setRegist(power, "0x01");
	            }
			}
        }
	}
	
	sprintf(str_payload, "{");
	int first_item = 0;
	for(i = 0; i < 5; i++) {
		if(strcmp(mac_array[i], "")) {
			if(first_item == 0 ) {
				sprintf(str_payload, "%s\"%d\":\"%s\"", str_payload, i, mac_array[i]);
				first_item = 1;
			} else {
				sprintf(str_payload, "%s,\"%d\":\"%s\"", str_payload, i, mac_array[i]);
			}
		}
	}
	sprintf(str_payload, "%s}", str_payload);
	sprintf(mosq_cmd, "mosquitto_pub -h www.baruntechpvs.com -t PVS/device/set/%s -m %s", mac_address, str_payload);
	system(mosq_cmd);
}

int main(int argc, char *argv[]) {
	
	while(1) {
		sleep(get_period());
		ping_test();
	}
	return 0;
}

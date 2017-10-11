#include "main.h"
struct mosquitto *mosq;
void send_dev_status(struct mosquitto *mosq)
{
	char data[512]={0,};
	get_device_status(&s_dsi);
	make_device_info(&s_dsi,&data[0]);
	mosquitto_publish(mosq, NULL, topic.status_info, strlen(data), data, 2, false);
}
void on_connect(struct mosquitto *mosq, void *obj, int rc)
{
	if(!rc)
	{
		printf("On Connect Broker\n");
		mosquitto_subscribe(mosq, NULL, topic.config_info,0);
		mosquitto_subscribe(mosq, NULL, topic.fw_info,0);
	}
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
	int r;
	char temp[3]={0,};
	printf("============================================================================================\n");
	printf("mid | payload | payloadlen |qos | retain | topic \n");
	printf("%d  | %s      | %d  | %d  | %d  | %s \n", msg->mid, (char*)msg->payload, msg->payloadlen, msg->qos, msg->retain, msg->topic);
	printf("============================================================================================\n");
	if(strcmp(msg->topic, topic.config_info)==0)
	{
		set_state(2); //설정값 업데이트
		dci_parser((char*)msg->payload,&s_dci);
		device_config_flash_set(s_dci); //state 1
		send_dev_status(mosq);
		set_state(0); //설정값 업데이트
	}
	else if(strcmp(msg->topic, topic.fw_info)==0)
	{
		set_state(1); //펌업 이후
		fwi_parser((char*)msg->payload,&s_fwi);
		r=firmware_check(&s_fwi);
		if(r==R_SUCCESS)
		{
			mosquitto_publish(mosq, NULL, topic.config_info_get, 0, NULL, 2, false);
		}
		else //firmware update fail send to server error message
		{
			sprintf(temp,"%d",r);
			mosquitto_publish(mosq, NULL, topic.error,strlen(temp),temp, 2, false);
		}
	}
	else if(strcmp(msg->topic, topic.command)==0)
	{
	//	message_parser((char*)msg->payload);
		send_dev_status(mosq);
	}
}
void on_publish(struct mosquitto *mosq, void *obj, int mid)
{
//        mosquitto_disconnect(mosq);
	printf("publish ok mid %d\n",mid);
}

void on_subscribe(struct mosquitto *mosq, void *obj, int mid, int qos_count, const int *granted_qos)
{
	sub_count++;
	printf("sub_count %d\n",sub_count);
	if(sub_count==2)
	{
		switch(state)
        {
			case 0://nomarl
				printf("%s\n",topic.fw_info_get);
				mosquitto_publish(mosq, NULL, topic.fw_info_get,0,NULL, 2, false);
				break;
			case 1: // firmware up then...
					printf("%s\n",topic.fw_info_get);
				mosquitto_publish(mosq, NULL, topic.fw_info_get,0,NULL, 2, false);
				break;
			case 2:
				mosquitto_publish(mosq, NULL, topic.config_info_get, 0, NULL, 2, false);
				printf("%s\n",topic.config_info_get);
				//send_dev_status(mosq);
				set_state(0);
				break;
			case 3:
				set_state(0);
				mosquitto_publish(mosq, NULL, topic.fw_info_get,0,NULL, 2, false);
				break;
			case 4:
				break;
			default:
				break;
		}
	}
}


void make_topic(struct s_topic *topic)
{
	char mac_addr[24]={0,};
	char temp[64]={0,};

	get_dev_mac(mac_addr);

	memset(&temp,0x00,64);
	sprintf(temp,"ROUTER/%s",mac_addr);
	strncpy(topic->name,temp,strlen(temp));

	memset(&temp,0x00,64);
	sprintf(temp,"%s/%s",STATUS_TOPIC,mac_addr);
	strncpy(topic->status_info,temp,strlen(temp));

	memset(&temp,0x00,64);
	sprintf(temp,"%s/%s",FW_UP_TOPIC,mac_addr);
	strncpy(topic->fw_info,temp,strlen(temp));
	
	memset(&temp,0x00,64);
	sprintf(temp,"%s/%s",FW_UP_GET_TOPIC,mac_addr);
	strncpy(topic->fw_info_get,temp,strlen(temp));

	memset(&temp,0x00,64);
	sprintf(temp,"%s/%s",CONFIG_TOPIC,mac_addr);
	strncpy(topic->config_info,temp,strlen(temp));

 	memset(&temp,0x00,64);
	sprintf(temp,"%s/%s",CONFIG_GET_TOPIC,mac_addr);
	strncpy(topic->config_info_get,temp,strlen(temp));
	printf("%s\n",topic->config_info_get);

	memset(&temp,0x00,64);
	sprintf(temp,"%s/%s",COMMAND_TOPIC,mac_addr);
	strncpy(topic->command,temp,strlen(temp));

	memset(&temp,0x00,64);
	sprintf(temp,"%s/%s",ERROR_PUT_TOPIC,mac_addr);
	strncpy(topic->error,temp,strlen(temp));
}

void period_pvs(int mode)
{
	printf("--------------------------------------------------------\n");
	set_state(3);
				send_dev_status(mosq);
	set_state(0);
				mosquitto_publish(mosq, NULL, topic.fw_info_get,0,NULL, 2, false);

	alarm(get_period());
	
}

int main(int argc, char *argv[])
{
	int rc,boot;
	memset(&topic,0x00,sizeof(struct s_topic));
	mosquitto_lib_init();
	make_topic(&topic);
	state=get_state();
	boot=get_booting_num();
	set_booting_num(boot+1);

	printf("%s\n",topic.name);
	mosq = mosquitto_new(topic.name, true, &run);
	mosquitto_connect_callback_set(mosq, on_connect);
	mosquitto_message_callback_set(mosq, on_message);
	mosquitto_publish_callback_set(mosq, on_publish);
	mosquitto_subscribe_callback_set(mosq, on_subscribe);
	mosquitto_message_retry_set(mosq, 5);
	
	while(1)
	{
		rc = mosquitto_connect(mosq, "www.baruntechpvs.com", 1883, 60);
		if(rc==MOSQ_ERR_SUCCESS)
		{
			break;
		}
		else
		{
			sleep(5); //5초에 한번씩 접속시도
		}
	}
	signal(SIGALRM,(void*)period_pvs);
	alarm(get_period());


	while(run == -1){
		mosquitto_loop(mosq, 300, 1);
	}

	mosquitto_lib_cleanup();
	return run;
}

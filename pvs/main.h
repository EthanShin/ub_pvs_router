#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include "mosquitto.h"
#include "device_info.h"
#include "parser.h"
#include "flash.h"


static int run = -1;
void make_topic(struct s_topic *topic);
void send_dev_status(struct mosquitto *mosq);
void on_connect(struct mosquitto *mosq, void *obj, int rc);
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg);
struct s_device_config_info s_dci={0,};
struct s_firmware_info s_fwi={0,};
struct s_device_status_info s_dsi={0,};
struct s_topic topic={0,};

int state;
int sub_count=0;


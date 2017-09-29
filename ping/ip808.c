#include <stdio.h>
#include <stdlib.h>
#include "ip808lib.h" 

void setPage(int num)
{
	if(!num)
	{
		set_page(0x54,IP808_REG_PAGE0);
	}
	else 
	{
		set_page(0x54,IP808_REG_PAGE1);
	}
}
void setRegist(char *regs, char *value)
{
	unsigned char reg;
	unsigned char val;
	set_page(0x54,IP808_REG_PAGE1);
	reg = strtol(regs, NULL, 16);
	val = strtol(value, NULL, 16);
	if(i2c_write_reg(0x54, reg, val)!=OK)
	{
		printf("Write ERROR\n");
		return;
	}
	printf("Write Reg[%02X]=%02X\n", reg, val);
}
/*
void setInit()
{
	int power;
	char tmpBuf[100];
	while(1)
	{
		if(IP808_init(0x54)==0)
		{
			break;
		}
		printf("IP808 init error!\n");
		sleep(1000);
	}
	set_page(0x54,IP808_REG_PAGE1);
	apmib_init();
	if (!apmib_get(MIB_POE_AP1_ENABLED, (void *)&power)) 
	{
		strcpy(tmpBuf, ("GET AP_POWER_ENABLED mib error!"));
		goto setErr_ip808;
	}
	if(!power)
		setRegist("0x9F","0x00");
		
	if (!apmib_get(MIB_POE_AP2_ENABLED, (void *)&power)) 
	{
		strcpy(tmpBuf, ("GET AP_POWER_ENABLED mib error!"));
		goto setErr_ip808;
	}
	if(!power)
		setRegist("0x9D","0x00");
	if (!apmib_get(MIB_POE_AP3_ENABLED, (void *)&power)) 
	{
		strcpy(tmpBuf, ("GET AP_POWER_ENABLED mib error!"));
		goto setErr_ip808;
	}
	if(!power)
		setRegist("0x9B","0x00");
	if (!apmib_get(MIB_POE_AP4_ENABLED, (void *)&power)) 
	{
		strcpy(tmpBuf, ("GET AP_POWER_ENABLED mib error!"));
		goto setErr_ip808;
	}
	if(!power)
		setRegist("0x99","0x00");
	if (!apmib_get(MIB_POE_AP5_ENABLED, (void *)&power)) 
	{
		strcpy(tmpBuf, ("GET AP_POWER_ENABLED mib error!"));
		goto setErr_ip808;
	}
	if(!power)
		setRegist("0x9E","0x00");
	return;
setErr_ip808:
	printf(" ip808 error %s\n",tmpBuf);
}
*/

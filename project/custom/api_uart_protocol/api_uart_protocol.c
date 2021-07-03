//===========================================================================================================================
//===========================================================================================================================
//本文件实现设备检测功能
//创建者：steve
//创建日期：2018-06- 04
//创建版本：version-1.0
//修改日期：
//修改内容：
//修改版本：
//===========================================================================================================================
#include "config.h"

comm_msg_t handler_msg;
u8 pedestalNo[MCU_ID_LEN_MAX];
static void api_send_cmd_to_handler_ack(u8 adr, u8 ack);
//printf redefined
void api_comm_parm_init(void)
{
    memset((char *)&handler_msg, 0, sizeof(comm_msg_t));
    handler_msg.write.max_len = COMM_BUF_LEN;
    handler_msg.read.max_len = COMM_BUF_LEN;
    handler_msg.dev_en = 1;
    Get_ChipID();
}

void uart_protocol_process(u8 *buf, u8 len)
{//aa 55 10 01 00 00 00 00 00 00 00 00 00 00 00 00 00 00 11
    sh_printf("uart_protocol_process:%d\n",buf[0]);
    if(buf[0] == CMD_PEDESTAL_GET_BAT_INFO)
    {//aa 55 10 01 ID1 ... ID12 电量 上锁情况 checksum
        sh_printf("GET_BAT_INFO:%d\n",buf[1]);
        if(buf[1] == E_GET_BAT_INFO)
        {
            handler_msg.bat_report_notify = E_HAND_GET_INFO;
            batt_discharge_ctl(TRUE);
        }else if(buf[1] == E_BAT_RETURN_OK)
        {
            batt_discharge_ctl(FALSE);
        }else if(buf[1] == E_BAT_RENT_OK)
        {
            batt_discharge_ctl(TRUE);
        }
        handler_msg.read.end_flag = 1;
    }else if(buf[0] == CMD_RETURN_GET_BAT_INFO)
    {//aa 55 10 01 ID1 ... ID12 电量 上锁情况 checksum
        handler_msg.read.end_flag = 1;
        handler_msg.bat_report_notify = E_HAND_RETRUN_INFO;
    }else if(buf[0] == CMD_GET_PEDESTAL_ID)
    {
        u8 i = 0;
        for(; i< 12; i++)
        {
            pedestalNo[i] = buf[1+i];
        }
        handler_msg.read.end_flag = 1;
    }else if(buf[0] == CMD_PC_PEDESTAL_ID)
    {
        api_send_pedestal_id();
        handler_msg.read.end_flag = 1;
        return;
    }else if(buf[0] == CMD_HD_ACK){
        if(buf[1] == HD_ACK)
        {
            handler_msg.read.end_flag = 1;
        }else{
            handler_msg.read.end_flag = 0xff;
        }
        return;
    }
    
    api_send_cmd_to_handler_ack(CMD_HD_ACK, HD_ACK);
}
static void api_comm_handle_pack_prc(u8 dat)
{
	static u16 head = 0;
	static u8 data_len;
	static u8 get_len = 0;
	static u8 step = 0;
	static u8 get_buf[128];
	static u8 checksum = 0;
	//head = buf[0];
	//os_printf("RX:%02x ",buf[0]);
	//for(cnt = 1; cnt < len; cnt++)
	{
		//os_printf("%02x ",buf[cnt]);
		if(step == 0)
		{
		    head <<= 8;
			head |= dat;
			if(head == SYNC_HEAD)
			{
				step = 1;
				data_len = 0;
				get_len = 0;
				checksum = 0;
			    head = 0;
			}
		}else if(step == 1)
		{
			data_len = dat;
			step++;
			checksum += dat;
		}else if(step == 2)
		{
			get_buf[get_len] = dat;
			checksum += dat;
			get_len++;
			if(get_len == (data_len-1))
			{
				step = 3;
			}
		}else if(step == 3)
		{
			if(checksum == dat)
			{
				uart_protocol_process(get_buf, data_len-1);
				step = 0;
				sh_printf("HD ok\r\n");
			}else{
				sh_printf("HD err\r\n");
				api_send_cmd_to_handler_ack(CMD_HD_ACK, HD_NACK);
				//os_printf("check failed:%d,%d,%d\n",checksum,buf[cnt],cnt);
			}
			step = 0;
			head = 0;
		}
	}
}

static void handle_parse_analysis(comm_msg_t *msg)
{
    //if(msg->dev_en == 0)return;
    //if(msg->read.end_flag)
    //if(msg->read.get_len != msg->read.que_len)
    {
        u8 get_byte;
        u16 get_cnt = 0;

        msg->read.end_flag = 0;
        if(msg->read.get_len == msg->read.que_len)return;
        while(1)
        {
            get_byte = msg->read.que_buf[msg->read.get_len];
            msg->read.get_buf[get_cnt] = get_byte;
            sh_printf("%02x ",get_byte);
            api_comm_handle_pack_prc(get_byte);

            msg->read.get_len++;
            get_cnt++;
            if(get_cnt >= sizeof(msg->read.get_buf))get_cnt = 0;
            if(msg->read.get_len >= msg->read.max_len)msg->read.get_len = 0;
            if(msg->read.get_len == msg->read.que_len)break;
        }
        sh_printf("\r\n");
    }
}
void api_uart_protocol_task(void)//protocol
{
    handle_parse_analysis(&handler_msg);
}
u8 api_uart_handler_send_cmd (u8 * cmd, u8 len, u8 ack, u32 waittime)
{
    comm_pack_t *w_pack = (comm_pack_t *)&handler_msg.write;
    comm_pack_t *r_pack = (comm_pack_t *)&handler_msg.read;
	//sprintf(w_pack->que_buf, "%s\r\n", cmd);
	handle_parse_analysis(&handler_msg);
	r_pack->end_flag = 0;
    r_pack->get_len = r_pack->que_len = 0;
	bsp_uart_tx_string(w_pack->que_buf, len);
	if(ack != HD_ACK)//???????
	{
	    return 1;
    }

    w_pack->time_out = waittime;
    while(w_pack->time_out)
    {
        r_pack->end_flag = 0;
        while ((!r_pack->end_flag) && w_pack->time_out)
        {
            handle_parse_analysis(&handler_msg);
        }
        //memcpy(r_pack->get_buf, r_pack->que_buf, r_pack->que_len);
        //r_pack->get_buf[r_pack->que_len] = '\0';
        sh_printf("end_flag:%d\n",r_pack->end_flag);
    	if(r_pack->end_flag == 1)
    	{
    	    return 1;
     	}
    	else if(r_pack->end_flag == 0xff)
    	{
    	    return 0;
    	}

    }
    return 0;
}
u8 api_handler_send_cmd(char *cmd, u8 len)
{
    u8 fail = 4;

    while(fail--)
    {
        if(api_uart_handler_send_cmd((u8 *)cmd, len, HD_ACK, 100))
        {
            sh_printf("HD send success\r\n");
            return 1;
        }
    }
    sh_printf("HD send fail\r\n");
    return 0;
}
static void api_send_cmd_to_handler_ack(u8 adr, u8 ack)
{
    u8 Chksum;
    u8 cmd_buf[6];
	//if(wifi_msg.wift_work_sta == E_WIFI_DISCONNECT)return;
	cmd_buf[0] = SYNC_HEAD>>8;
	cmd_buf[1] = SYNC_HEAD&0xff;
	cmd_buf[2] = 1 + 2;
	cmd_buf[3] = adr;
	Chksum = (u8)(1 + 2);
	Chksum += adr;
	cmd_buf[4] = ack;
	Chksum += ack;
	cmd_buf[5] = Chksum;
	bsp_uart_tx_string(cmd_buf, 6);
}
u8 api_send_cmd_to_handler(u8 adr,u8 *str,u8 len)
{
    u8 i;
    u8 Chksum;
    u8 *cmd_buf;
	if(!handler_msg.dev_en)return 0;
    cmd_buf = handler_msg.write.que_buf;
	cmd_buf[0] = SYNC_HEAD>>8;
	cmd_buf[1] = SYNC_HEAD&0xff;
	cmd_buf[2] = len + 2;
	cmd_buf[3] = adr;
	Chksum = (u8)(len + 2);
	Chksum += adr;
	for(i=0;i<len;i++)
	{
		cmd_buf[4+i] = *str;
		Chksum += *str++;
	}
	cmd_buf[4+i] = Chksum;
	return (api_handler_send_cmd((char *)cmd_buf, len+5));
}

void api_send_handler_sta(u8 sta)
{
    u8 buf[128];
    shared.handler.status = sta;
    shared.handler.bat_level = Bat.PCT;
    memcpy(buf, &shared.handler, sizeof(shared.handler)-1);
    api_send_cmd_to_handler(CMD_HD_REPORT_BAT_INFO, buf, sizeof(shared.handler)-1);
}
void api_send_pedestal_id(void)
{
    u8 buf[128];
    memcpy(buf, &shared.handler.No, sizeof(shared.handler.No));
    memcpy(&buf[sizeof(shared.handler.No)], &pedestalNo, sizeof(pedestalNo));
    api_send_cmd_to_handler(CMD_GET_PEDESTAL_ID, buf, MCU_ID_LEN_MAX*2);
}
/*********************************************************
                File End
*********************************************************/

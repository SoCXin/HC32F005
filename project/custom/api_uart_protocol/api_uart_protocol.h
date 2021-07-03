//===========================================================================================================================
//本文件实现设备检测功能
//创建者：steve
//创建日期：2018-06- 04
//创建版本：version-1.0
//修改日期：
//修改内容：
//修改版本：
//===========================================================================================================================
#ifndef __API_UART_PROTOCOL_H__
#define __API_UART_PROTOCOL_H__


#ifdef __cplusplus
extern "C" {
#endif
#define SYNC_HEAD          0xaa55
typedef enum
{
	CMD_HD_REPORT_BAT_INFO = 0,////底座上报ID及电池信息
	CMD_PEDESTAL_GET_BAT_INFO,//手柄查询ID及电池信息
	CMD_RETURN_GET_BAT_INFO,//归完手柄查询ID及电池信息
	CMD_GET_PEDESTAL_ID,    //获取底座ID
	CMD_PC_PEDESTAL_ID, //PC获取底座ID
	CMD_HD_ACK=0xff,  //0:ACK 1:NACK
}eUART_HANDLER_CMD;
enum{////底座上报ID及电池信息
	E_GET_BAT_INFO = 0,//取电池信息
	E_BAT_RETURN_OK,//归还成功
	E_BAT_RENT_OK,//租借成功
};
enum{
    HD_ACK = 0,
    HD_NACK,
};
enum{
    UART_RX_STATE_READY = 0,
    UART_RX_STATE_START,
};
#define HANDLE_POWER_UNIT           4
#define COMM_BUF_LEN              100
typedef struct
{
    u8 get_buf[COMM_BUF_LEN+1];
    u8 que_buf[COMM_BUF_LEN+1];
    u16 que_len;
    u16 get_len;
    u16 max_len;
    u8 end_flag;
    u32 time_out;
}comm_pack_t;
typedef struct
{
    comm_pack_t read;
    comm_pack_t write;
    u8 dev_en;
	u8 bat_report_notify;
}comm_msg_t;

extern comm_msg_t handler_msg;

void api_uart_protocol_task(void);
u8 api_send_cmd_to_handler(u8 adr,u8 *str,u8 len);
void api_comm_cmd(char *cmd);
void api_comm_parm_init(void);
void api_send_handler_sta(u8 lock);
void api_send_pedestal_id(void);
#ifdef __cplusplus
}
#endif
#endif//__API_UART_PROTOCOL_H__

/*********************************************************
				File End
*********************************************************/


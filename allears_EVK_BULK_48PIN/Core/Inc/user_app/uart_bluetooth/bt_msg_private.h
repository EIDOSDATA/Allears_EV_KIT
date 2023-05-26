#ifndef	BT_MSG_PRIVATE_H

#define BT_MSG_PRIVATE_H


#include <user_app/uart_bluetooth/bt_msg.h>



/* bt_msg.c */
void btMsg_init(void);

void btMsg_rcvData_handle(void);


/* bt_msg_rcv.c */
void bt_state_req(uint8* msg);

void bt_mode_set_req(uint8* msg);

void bt_level_set_req(uint8* msg);

void bt_stimul_req(uint8* msg);

void bt_elect_detect_req(uint8* msg);

void bt_man_mode_req(uint8* msg);

void bt_fw_ver_req(void);

void bt_stim_lib_ver_req(void);

void bt_stim_stimul_type_req(void);

void bt_stim_sys_reset_req(void);

void bt_stim_trg_cfg_req(uint8* msg);


/* bt_msg_send.c */
void bt_msg_res(bt_msg_type_t type, bt_msg_res_t res, uint8* data, uint8 data_len);

void bt_state_ind(void);

#endif	/* BT_MSG_PRIVATE_H */


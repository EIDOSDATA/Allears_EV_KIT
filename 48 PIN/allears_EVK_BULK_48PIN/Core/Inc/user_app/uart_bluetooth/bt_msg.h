#ifndef	BT_MSG_H

#define BT_MSG_H

#include "td_types.h"

#define BT_MSG_SIZE_MAX									20
#define BT_MSG_HEADER_SIZE								3
#define BT_MSG_DATA_SIZE_MAX							17				/* BT_MSG_SIZE_MAX - BT_MSG_HEADER_SIZE */

#define	BT_MSG_START_CODE_IX							0x00
#define	BT_MSG_TYPE_IX									0x01
#define	BT_MSG_LEN_IX									0x02
#define	BT_MSG_DATA_IX									0x03

#define BT_MSG_START_CODE								0xEF

#define BT_MSG_TYPE_BASE								0x00

#define AUL_BT_MSG_STIMUL_STOP							0
#define AUL_BT_MSG_STIMUL_START							1


/* MSG structure */
typedef struct
{
	uint8 start_code;
	uint8 msg_type;
	uint8 data_len;
	uint8 data[BT_MSG_DATA_SIZE_MAX];
} bt_msg_t;

/*
	MSG types

	Do not change order
*/
typedef enum
{
	BT_MSG_RES = BT_MSG_TYPE_BASE,
	BT_STATE_REQ,
	BT_STATE_IND,
	BT_MODE_SET_REQ,
	BT_LEVEL_SET_REQ,
	BT_STIMUL_REQ,
	BT_ELECT_DETECT_REQ,
	BT_MAN_MODE_REQ,
	BT_FW_VER_REQ,

	/* MSG specific to STIM_LIB */
	BT_STIM_LIB_VER_REQ,
	BT_STIM_STIMUL_TYPE_REQ,
	BT_STIM_SYS_RESET_REQ,
	BT_STIM_TRG_CFG_REQ,

	BT_MSG_TYPE_MAX
} bt_msg_type_t;


/*
	Code definition
*/
/* Message BOOL value */
typedef enum
{
	BT_MSG_BOOL_FALSE = 0x00,
	BT_MSG_BOOL_TRUE,
	
	BT_MSG_BOOL_MAX
} bt_msg_bool_t;

/* Message response code */
typedef enum
{
	BT_MSG_RES_OK = 0x00,
	BT_MSG_RES_INVALID_STATUS,
	BT_MSG_RES_INVALID_PARAM,
	BT_MSG_RES_UNKNOWN_MSG,
	BT_MSG_RES_CAN_NOT_HANDLE_MSG,

	BT_MSG_RES_MAX
} bt_msg_res_t;

/* Message stimulation type */
typedef enum
{
	BT_MSG_STIMUL_TYPE_UNKNOWN = 0x00,
	BT_MSG_STIMUL_TYPE_CONST_CURR,
	BT_MSG_STIMUL_TYPE_CONST_VOLT,
	
	BT_MSG_STIMUL_TYPE_MAX
} bt_msg_stimul_type_t;

#endif	/* BT_MSG_H */


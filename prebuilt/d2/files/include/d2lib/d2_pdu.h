/*************
 *
 * $Id:$
 *
 *  Name:      d2_pdu.h
 *
 *  Purpose:   Define API at layer of D2D protocol
 *
 *  Note:
 *
 * Copyright: (c) 2017 Netgear, Inc.
 *            All rights reserved
 *
 **************/
#ifndef D2_PDU_H_
#define D2_PDU_H_

#ifdef	__cplusplus
extern "C" {
#endif


/************
 *
 * Name:     dxap_pdu - represents PDU values
 *
 * Purpose:  hold/communicate PDU values
 *
 * Members:  req - request type
 *           tblname - table name
 *           fldname - field name
 *           fldindex - sometimes field may be expressed as index
 *                      instead of name- it is positional index within
 *                      table definition
 *           resp_code - repsonse code for responses
 *           type  - field type
 *           value_len - length of value
 *           value - field value
 *
 * Notes:  none
 *
 ************/
struct dxap_pdu
{
  enum dxap_pdu_type {
    DXAP_INFO,
    DXAP_INFO_RESP,

    DXAP_GET,
    DXAP_GET_RESP,

    DXAP_SET,
    DXAP_SET_RESP,

	DXAP_REG,
	DXAP_REG_RESP,

	DXAP_LOCK,
	DXAP_LOCK_RESP,

    DXAP_NTF,
  } req;

  char tblname[64];
  int  index;
  char fldname[64];
  uint32_t correlator; /* transaction id */
  uint8_t fldindex;

  uint8_t resp_code;

  uint16_t type;
  uint8_t  onoff;

  uint16_t value_len;
  uint8_t value[256];
};


/*
 * Response code value for responses
 */
enum muxdx_resp_code
{
	 MUXDX_RESP_CODE_OK =       0,  /* OK */
	 MUXDX_RESP_CODE_NOT_FOUND= 1,  /* table, or field not found */
	 MUXDX_RESP_CODE_TOO_LONG = 2,  /* incompatible value or buffer */
	 MUXDX_RESP_CODE_ERROR    = 3,  /* generic system error */
	 MUXDX_RESP_CODE_WRONG_TYPE=4,
	 MUXDX_RESP_CODE_ENCODING = 5,
};

int d2_rx_pdu( int sockfd, struct dxap_pdu * pdu, uint32_t tmoMs );
int d2_send_pdu( int sockfd, struct dxap_pdu * pdu );

int d2_client_socket_init( const char * name );
int d2_server_socket_init( const char * name );

#ifdef	__cplusplus
}
#endif



#endif /* D2_PDU_H_ */

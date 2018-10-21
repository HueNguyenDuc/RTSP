#ifndef _RTP_UDP_H
#define _RTP_UDP_H

#include "rtsp.h"
#include "rtcp.h"
#include "type.h"

VOID *vd_rtp_func(VOID *arg);
VOID *vd_rtcp_func(VOID *arg);
S32 create_vrtp_socket(const CHAR *host, S32 port, S32 type, S32 cur_conn_num);
S32 create_vrtcp_socket(const CHAR *host, S32 port, S32 type, S32 cur_conn_num);
S32 proc_rtp(S32 cur_conn_num);
S32 proc_rtcp(S32 cur_conn_num);

#endif


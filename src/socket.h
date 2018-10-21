#ifndef _SOCKET_H
#define _SOCKET_H

#include "type.h"
#include "rtsp.h"

S32 tcp_read(S32 fd, void *buf, S32 length);
S32 tcp_write(S32 fd, void *buf, S32 length);
S32 init_memory();
S32 free_memory();
S32 close_rtsp_fd();
void *vd_rtsp_procin(void *arg);
void *vd_rtsp_proc(void *arg);
S32 create_sercmd_socket(const CHAR *host, const CHAR *port, S32 type);
S32 rtsp_cmd_match(S32 method, S32 cur_conn_num);
S32 set_free_conn_status(S32 cur_conn, S32 cur_status);
S32 get_free_conn_status();
S32 tcp_accept();

#endif

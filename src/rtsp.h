#ifndef _RTSP_H
#define _RTSP_H

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <time.h>
#include <netdb.h>
#include <pthread.h>
#include <sys/utsname.h>
#include <unistd.h>
#include <errno.h>
#include <semaphore.h>

#include "type.h"
#include "global.h"
#include "md5.h"

#define RTSP_EL "\r\n"
#define RTSP_VER "RTSP/1.0"
#define HDR_REQUIRE "Require"
#define HDR_ACCEPT "Accept"
#define PACKAGE "rtspd"
#define VERSION "1.0"
#define SDP_EL "\r\n"
#define HDR_TRANSPORT "Transport"
#define HDR_SESSION "Session"


#define RTSP_BUFFERSIZE 4096
#define MAX_DESCR_LENGTH 4096
#define DEFAULT_TTL 32
#define MAX_CONN 100
#define HDR_CSEQ "CSeq"
extern sem_t rtspd_semop;
extern sem_t rtspd_lock[MAX_CONN];
extern sem_t rtspd_accept_lock;
extern pthread_cond_t rtspd_cond;
extern pthread_mutex_t rtspd_mutex;

extern struct rtsp_buffer *rtsp[MAX_CONN];
struct rtsp_port{
	S32 rtp_cli_port;
	S32 rtcp_cli_port;
	S32 rtp_ser_port;
	S32 rtcp_ser_port;
	U32 ssrc;
	U32 timestamp;
	U32 frame_rate_step;
	U16 seq;
};

struct rtsp_fd{
	S32 rtspfd;
	S32 video_rtp_fd;
	S32 video_rtcp_fd;
	S32 audio_rtp_fd;
	S32 audio_rtcp_fd;
};

struct rtsp_th{
	pthread_t rtsp_vthread;	
	pthread_t rtsp_vthread1;
	pthread_t rtp_vthread;
	pthread_t rtcp_vthread;
};

struct rtsp_cli{
	S32 cli_fd;
	S32 conn_num;
	CHAR cli_host[128];
};

struct rtsp_buffer {
	S32 payload_type; /* 96 h263/h264*/
	S32 session_id;
	U32 rtsp_deport;
	U32 rtsp_um_stat;  /**** 0 is Unicast   1 is multicast  ****/
	U32 rtsp_cseq;
	U32 is_runing;
	U32 cur_conn;
	U32 conn_status;	
	U32 rtspd_status;
	U32 vist_type;  /****0: H264 file vist  1: PS file vist  2: h264 stream vist ****/

	struct rtsp_port cmd_port;
	struct rtsp_fd fd;
	struct rtsp_th pth;
	struct rtsp_cli cli_rtsp;
	// Buffers		
	CHAR file_name[128];
	CHAR host_name[128];
	U8 nalu_buffer[1448];
	CHAR in_buffer[RTSP_BUFFERSIZE];
	CHAR out_buffer[RTSP_BUFFERSIZE];	
	CHAR sdp_buffer[MAX_DESCR_LENGTH];
};

S32 build_rtp_nalu(U8 *inbuffer, S32 frame_size, S32 cur_conn_num);
struct rtsp_buffer *rtsp[MAX_CONN];
S32 get_rtsp_cseg(S32 cur_conn_num);
S32 check_rtsp_url(S32 cur_conn_num);
S32 check_rtsp_filename(S32 cur_conn_num);
CHAR *get_stat(S32 err);
S32 send_reply(S32 errornumber, S32 cur_conn_num);
S32 get_rtsp_method(S32 cur_conn_num);
S32 is_supported_mediatype(CHAR *p, S32 cur_conn_num);
S32 parse_url(const CHAR *url, CHAR *server, U16 *port, CHAR *file_name);
CHAR *get_hostname();
VOID add_time_stamp(CHAR *b, S32 crlf);
S32 send_describe_reply(S32 status, S32 cur_conn_num);
CHAR *get_SDP_user_name(CHAR *buffer);
float NTP_time(time_t t);
CHAR *get_SDP_session_id(CHAR *buffer);
CHAR *get_SDP_version(CHAR *buffer);
CHAR *get_address();
S32 get_describe_sdp(CHAR *sdp_buff, S32 cur_conn_num);
S32 rtsp_describe(S32 cur_conn_num);
S32 set_options_reply(S32 status, S32 cur_conn_num);
S32 rtsp_options(S32 cur_conn_num);
S32 send_play_reply(S32 status, S32 cur_conn_num);
S32 rtsp_play(S32 cur_conn_num);
static U32 md_32(CHAR *string, S32 length);
U32 random32(S32 type);
S32 get_server_port(S32 cur_conn_num);
S32 send_setup_reply(S32 status, S32 cur_conn_num);
S32 rtsp_setup(S32 cur_conn_num);
S32 send_terardown_reply(S32 status, S32 cur_conn_num);
S32 rtsp_terardown(S32 cur_conn_num);

#endif
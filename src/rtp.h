#ifndef _RTP_H
#define _RTP_H

#include <unistd.h>
#include "type.h"
#include "rtcp.h"
#include "rtsp.h"
#include "../crypt/cipher/aes.h"

/****
 0					 1					 2					 3
 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|V=2|P|X|  CC	|M| 	PT		|		sequence number 		|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|							timestamp							|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|			synchronization source (SSRC) identifier			|
+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
|			 contributing source (CSRC) identifiers 			|
|							  ....								|
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+

****/
#define RTP_SIZE_MAX    1460
#define RTP_HEADER_SIZE 12
#define NALU_INDIC_SIZE 4
#define NALU_HEAD_SIZE  1
#define FU_A_INDI_SIZE  1
#define FU_A_HEAD_SIZE  1

/* SINGLE_NALU_DATA_MAX = RTP_SIZE_MAX - RTP_HEADER_SIZE*/
#define SINGLE_NALU_DATA_MAX  1448

/* SLICE_NALU_DATA_MAX = RTP_SIZE_MAX - RTP_HEADER_SIZE - FU_A_INDI_SIZE
       - FU_A_HEAD_SIZE */
#define SLICE_NALU_DATA_MAX   1446

#define MIN(a,b) ( ((a)<(b)) ? (a) : (b) )

#define READ_LEN 150000
#define SLICE_SIZE 1448
#define SLICE_FSIZE 1435
#define DE_TIME 3600

typedef struct _RTP_header {
	/* byte 0 */
	#ifdef WORDS_BIGENDIAN
		U8 version:2;
		U8 padding:1;
		U8 extension:1;
		U8 csrc_len:4;
	#else
		U8 csrc_len:4;		
		U8 extension:1;		
		U8 padding:1;		
		U8 version:2;	
	#endif
	/* byte 1 */
	#if WORDS_BIGENDIAN
		U8 marker:1;
		U8 payload:7;		
	#else
		U8 payload:7;
		U8 marker:1; 
	#endif
	/* bytes 2, 3 */
	U16 seq_no;
	/* bytes 4-7 */
	U32 timestamp;
	/* bytes 8-11 */
	U32 ssrc;					/* stream number is used here. */
} RTP_header;

#endif

int find_nal_unit(uint8_t* buf, int sz, int* nal_start, int* nal_end);
L64 get_file_size(FILE *infile);
UL64 get_randdom_seq(VOID);
UL64 get_randdom_timestamp(VOID);
L64  get_timestamp();
S32 build_rtp_header(RTP_header *r, S32 cur_conn_num);
ssize_t write_n(S32 fd, const VOID *vptr, size_t n);
S32 udp_write(S32 len, S32 cur_conn_num);
S32 udp_write_fua(S32 len, S32 time, S32 cur_conn_num);
S32 abstr_nalu_indic(U8 *buf, S32 buf_size, S32 *be_found);
S32 build_rtp_nalu(U8 *inbuffer, S32 frame_size, S32 cur_conn_num);
S32 rtp_send_form_file(S32 cur_conn_num);
S32 rtp_send_packet(S32 cur_conn_num);

#!/usr/bin/env bash

gcc -c ../crypt/cipher/*.c
gcc -c ../H264/*.c
gcc -o aes h264_analyze.c h264_nal.o h264_sei.o h264_stream.o aes.o -no-pie

gcc -c ../include/*.c
gcc -c ../src/*.c
gcc -o server sample_rtspd.c md5c.o rtcp.o rtp_h264.o rtspd_api.o rtsp.o socket.o udp.o aes.o cpu_endian.o -pthread

#include "cipher/aes.c"
#include "cipher/rc4.c"
#include <stdio.h>
#include <string.h>

int main(int argc,char **argv)
{
	AesContext data;
	unsigned char in[16];
	in[0]=0x01;in[1]=0x4B;in[2]=0xAF;in[3]=0x22;in[4]=0x78;in[5]=0xA6;in[6]=0x9D;in[7]=0x33;in[8]=0x1D;in[9]=0x51;in[10]=0x80;in[11]=0x10;in[12]=0x36;in[13]=0x43;in[14]=0xE9;in[15]=0x9A;
	char* key="1234567890abcdef";
	/*unsigned char in[16];
	in[0]=0x76;in[1]=0x77;in[2]=0x74;in[3]=0x75;in[4]=0xf1;in[5]=0xf2;in[6]=0xf3;in[7]=0xf4;in[8]=0xf8;in[9]=0xf9;in[10]=0xe6;in[11]=0xe7;in[12]=0x77;in[13]=0x70;in[14]=0x71;in[15]=0x72;
	in[0]=0x06;in[1]=0x9A;in[2]=0x00;in[3]=0x7F;in[4]=0xC7;in[5]=0x6A;in[6]=0x45;in[7]=0x9F;in[8]=0x98;in[9]=0xBA;in[10]=0xF9;in[11]=0x17;in[12]=0xFE;in[13]=0xDF;in[14]=0x95;in[15]=0x21;*/
	aesInit(&data,(uint8_t* )key,16);
	unsigned char out[256];
	memset(out,0x00,256);
	
	int i=0;
	printf("Key: \n\n");
	for(i=0;i<16;++i)
	printf("%02x ",key[i]);
	printf("\n\n");
	printf("Plaintext : \n\n");
	for(i=0;i<16;++i)
	printf("%02x ",in[i]);
	printf("\n\n");
	aesEncryptBlock(&data,(uint8_t* )in,(uint8_t* )out);
	printf("Ciphertext :\n\n");
	for(i=0;i<strlen(out);++i)
	printf("%02x ",out[i]);
	printf("\n\n");
	unsigned char de[256];
	memset(de,0x00,256);
	printf("Plaintext : \n\n");
	aesDecryptBlock(&data,(uint8_t* )out,(uint8_t* )in);
	for(i=0;i<16;++i)
	printf("%02x ",in[i]);
	printf("\n");
}

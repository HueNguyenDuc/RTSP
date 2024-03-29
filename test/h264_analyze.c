/* 
 * h264bitstream - a library for reading and writing H.264 video
 * Copyright (C) 2005-2007 Auroras Entertainment, LLC
 * 
 * Written by Alex Izvorski <aizvorski@gmail.com>
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

#include "../H264/h264_stream.h"
#include "../H264/file.h"
#include "../crypt/cipher/aes.h"


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#define BUFSIZE 32*1024*1024

#if (defined(__GNUC__))
#define HAVE_GETOPT_LONG

#include <getopt.h>


static struct option long_options[] =
{
    { "probe",   no_argument, NULL, 'p'},
    { "output",  required_argument, NULL, 'o'},
    { "help",    no_argument,       NULL, 'h'},
    { "verbose", required_argument, NULL, 'v'},
};
#endif

static char options[] =
"\t-o output_file, defaults to test.264\n"
"\t-v verbose_level, print more info\n"
"\t-p print codec for HTML5 video tag's codecs parameter, per RFC6381\n"
"\t-h print this message and exit\n";

void usage( )
{

    fprintf( stderr, "h264_analyze, version 0.2.0\n");
    fprintf( stderr, "Analyze H.264 bitstreams in Annex B format\n");
    fprintf( stderr, "Usage: \n");

    fprintf( stderr, "h264_analyze [options] <input bitstream>\noptions:\n%s\n", options);
}
//-----------------------------------------------------//

typedef struct nal_header
{
	int forbidden_zero_bit;
	int nal_ref_idc;
	unsigned int nal_unit_type;
} NAL_HEADER, *PNAL_HEADER;

int read_nal(PNAL_HEADER d,char in)
{
	if (d != NULL)
	{
		d->forbidden_zero_bit = 0;
		d->nal_ref_idc = 0;
		d->nal_unit_type = 0;
		d->forbidden_zero_bit = (0x80 & in) >> 7;
		d->nal_ref_idc = (0x60 & in) >> 5;
		d->nal_unit_type = 0x1f & in;
		return 1;
	}
	return -1;
}

//-------------------------------------------------------//

int main(int argc, char *argv[])
{
    FILE* infile;
	FILE* fout = fopen(argv[2], "w+b");

    uint8_t* buf = (uint8_t*)malloc( BUFSIZE );

    h264_stream_t* h = h264_new();

    if (argc < 2) { usage(); return EXIT_FAILURE; }

    int opt_verbose = 1;
    int opt_probe = 0;

#ifdef HAVE_GETOPT_LONG
    int c;
    int long_options_index;
    extern char* optarg;
    extern int   optind;

    while ( ( c = getopt_long( argc, argv, "o:phv:", long_options, &long_options_index) ) != -1 )
    {
        switch ( c )
        {
            case 'o':
                if (h264_dbgfile == NULL) { h264_dbgfile = fopen( optarg, "wt"); }
                break;
            case 'p':
                opt_probe = 1;
                opt_verbose = 0;
                break;
            case 'v':
                opt_verbose = atoi( optarg );
                break;
            case 'h':
            default:
                usage( );
                return 1;
        }
    }

    infile = fopen(argv[optind], "rb");

#else

    infile = fopen(argv[1], "rb");

#endif

    if (infile == NULL) { fprintf( stderr, "!! Error: could not open file: %s \n", strerror(errno)); exit(EXIT_FAILURE); }

    if (h264_dbgfile == NULL) { h264_dbgfile = stdout; }
    

    size_t rsz = 0;
    size_t sz = 0;
    int64_t off = 0;
    uint8_t* p = buf;
	char *key = "0123456789abcdef";
	int keysize = 16; // 128 bits 
	AesContext aesContext;
	aesInit(&aesContext, (uint8_t*)key, keysize);

    int nal_start, nal_end;
    long long int start,size;
    while (1)
    {
        rsz = fread(buf + sz, 1, BUFSIZE - sz, infile);
        if (rsz == 0)
        {
            if (ferror(infile)) { fprintf( stderr, "!! Error: read failed: %s \n", strerror(errno)); break; }
            break;  // if (feof(infile)) 
        }

        sz += rsz;

		while (find_nal_unit(p, sz, &nal_start, &nal_end) > 0)
		{
			if (opt_verbose > 0)
			{
				fprintf(h264_dbgfile, "!! found nal at offset %lld (0x%04llx), size %lld (0x%04llx) \n",
					(long long int)(off + (p - buf) + nal_start),
					(long long int)(off + (p - buf) + nal_start),
					(long long int)(nal_end - nal_start),
					(long long int)(nal_end - nal_start)); */
					start = (long long int)(off + (p - buf) + nal_start);
				size = (long long int)(nal_end - nal_start);
			}
			p += nal_start;
			read_nal_unit(h, p, nal_end - nal_start);
			if (h->nal->nal_unit_type == NAL_UNIT_TYPE_CODED_SLICE_IDR && h->sh->slice_type == SH_SLICE_TYPE_I_ONLY)
				printf("\nFound NAL Slice_type = %d at: %lld (0x%08x) --- size: %lld(0x%08x)\n", h->sh->slice_type, start, start, size, size);
			p += nal_end;
			sz -= nal_end;
		}
		

		if (fout != NULL)
		{
			fwrite(buf, 1, rsz, fout);
		}

        // if no NALs found in buffer, discard it
        if (p == buf) 
        {
            fprintf( stderr, "!! Did not find any NALs between offset %lld (0x%04llX), size %lld (0x%04llX), discarding \n",
                   (long long int)off, 
                   (long long int)off, 
                   (long long int)off + sz, 
                   (long long int)off + sz);

            p = buf + sz;
            sz = 0;
        }

        memmove(buf, p, sz);
        off += p - buf;
        p = buf;
    }

    h264_free(h);
    free(buf);

    fclose(h264_dbgfile);
    fclose(infile);
	fclose(fout);

    return 0;
}

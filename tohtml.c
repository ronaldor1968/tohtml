/*
 * tohtml.c
 *
 *  Created on: 09/12/2012
 *      Author: Ronaldo Rebouças de Oliveira
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void print_usage() {
	printf("convert binary file to antivirus-prof html\n");
	printf("by Ronaldo Oliveira\n\n");
	printf("usage: tohtml file\n");
	printf("example: tohtml myfile.exe > myfile.html\n");
}

/* grep main: search for regexp in files */
int main(int argc, char *argv[])
{
	FILE *f;

	if (argc < 2) {
		print_usage();
	} else if (argc == 2) {
		f = fopen(argv[1], "rb");
		printf("<html><body><a href=\"data:application/octet-stream;charset=utf-8;base64,");
		encode_file(f);
		printf("\">file</a></body></html>");
	} else {
		print_usage();
	}
	return 0;
}



void encode_file(FILE *f)
{
	static char encoding_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
	                                'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
	                                'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
	                                'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
	                                'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
	                                'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
	                                'w', 'x', 'y', 'z', '0', '1', '2', '3',
	                                '4', '5', '6', '7', '8', '9', '+', '/'};

	int n, i0, i1, i2,i3;
	char c[] = {0,0,0,0,0};
	long int s0,s1,s2;
	int BLOCK = ((BUFSIZ / 3) - 1)*3; //BUFSIZ; deve ser multiplo de 3
	char buf[BLOCK];

	while ((n = fread(buf, 1, BLOCK, f)) != 0) {
		int i = 0;
		while(i < n) {
			// 0        1        2
			// AAAAAABB BBBBCCCC CCDDDDDD
			// XXXXXXXX YYYYYYYY ZZZZZZZZ
			// read and count
			int read = 0;
			if (i < n) {
				read++;
				s0 = buf[i++];
			} else s0 = 0;
			if (i < n) {
				read++;
				s1 = buf[i++];
			} else s1 = 0;
			if (i < n) {
				read++;
				s2 = buf[i++];
			} else s2 = 0;

			i0 = (s0 & 0xfc) >> 2;
			i1 = ((s0 & 0x03) << 4) | ((s1 & 0xf0) >> 4);
			i2 = ((s1 & 0x0f) << 2) | ((s2 & 0xc0) >> 6);
			i3 = s2 & 0x3F;
			c[0] = encoding_table[i0];
			c[1] = encoding_table[i1];
			switch (read) {
				case 3:
					c[2] = encoding_table[i2];
					c[3] = encoding_table[i3];
					break;
				case 2:
					c[2] = encoding_table[i2];
					c[3] = '=';
					break;
				case 1:
					c[2] = '=';
					c[3] = '=';
					break;
				default:
					break;
			}
			printf("%s",c);
		}
	}
}

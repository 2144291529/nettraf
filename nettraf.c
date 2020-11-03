/* show NIC traffic stats by james@ustc.edu.cn 2020.11.03
*/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

typedef unsigned long int u32;

#define MAXDEV 10
#define DEVNAMELEN 20
#define MAXLEN 1024

int totaldev = 0;

time_t curt;
struct tm *curtm;

u32 rp[MAXDEV], rb[MAXDEV], tp[MAXDEV], tb[MAXDEV];
u32 lrp[MAXDEV], lrb[MAXDEV], ltp[MAXDEV], ltb[MAXDEV];

char devname[MAXDEV][DEVNAMELEN + 1];

void AddCount(int devnum, u32 rp1, u32 rb1, u32 tp1, u32 tb1)
{
	if (devnum >= MAXDEV)
		return;
	if (lrp[devnum] < rp1)
		rp[devnum] = rp1 - lrp[devnum], lrp[devnum] = rp1;
	else
		lrp[devnum] = rp1, rp[devnum] = 0;
	if (lrb[devnum] < rb1)
		rb[devnum] = rb1 - lrb[devnum], lrb[devnum] = rb1;
	else
		lrb[devnum] = rb1, rb[devnum] = 0;
	if (ltp[devnum] < tp1)
		tp[devnum] = tp1 - ltp[devnum], ltp[devnum] = tp1;
	else
		ltp[devnum] = tp1, tp[devnum] = 0;
	if (ltb[devnum] < tb1)
		tb[devnum] = tb1 - ltb[devnum], ltb[devnum] = tb1;
	else
		ltb[devnum] = tb1, tb[devnum] = 0;
}

float Nom1(float f)
{
	while (f > 1000)
		f = f / 1000;
	return f;
}

char Nom2(float f)
{
	if (f < 1e3)
		return ' ';
	if (f < 1e6)
		return 'K';
	if (f < 1e9)
		return 'M';
	if (f < 1e12)
		return 'G';
	return 'T';
}

void dostatis()
{
	FILE *fp;
	char buf[MAXLEN];
	static int outlen = 0;
	int i;
	fp = fopen("/proc/net/dev", "r");
	if (fp == NULL) {
		fprintf(stderr, "Could not open /proc/net/dev\n");
		sleep(30);
		return;
	}
	while (fgets(buf, MAXLEN, fp) != NULL) {
		unsigned long int rp1, rb1, tp1, tb1, ign;
		int devnum, i;
		char *devn;
		char *p = buf;
		while (*p == ' ')
			p++;
		if (*p == 0)
			continue;
		devn = p;
		while (*p && (*p != ':'))
			p++;
		if (*p == 0)
			continue;
		*p = 0;
		p++;

		devnum = -1;
		for (i = 0; i < totaldev; i++)
			if (strcmp(devname[i], devn) == 0)
				devnum = i;
		if (devnum == -1)
			continue;
		if (sscanf(p, "%lu %lu %lu %lu %lu %lu %lu %lu %lu %lu",
			   &rb1, &rp1, &ign, &ign, &ign, &ign, &ign, &ign, &tb1, &tp1) != 10) {
			fprintf(stderr, "Could not open /proc/net/dev\n");
			sleep(30);
			continue;
		}
		//      printf("%d %lu %lu %lu %lu\n",devnum,rp1,rb1,tp1,tb1);
		AddCount(devnum, rp1, rb1, tp1, tb1);
	}
	fclose(fp);
	if ((outlen == 0) || (outlen == 20)) {
		for (i = 0; i < totaldev; i++)
			printf("+-----------------------");
		printf("+\n");
		for (i = 0; i < totaldev; i++) {
			int x;
			printf("|");
			for (x = 0; x < 23 / 2 - strlen(devname[i]) / 2; x++)
				printf(" ");
			printf("%s", devname[i]);
			for (x = 0; x < 23 - strlen(devname[i]) - 23 / 2 + strlen(devname[i]) / 2;
			     x++)
				printf(" ");
		}
		printf("|\n");
		for (i = 0; i < totaldev; i++)
			printf("|      TX         RX    ");
		printf("|\n");
		for (i = 0; i < totaldev; i++)
			printf("|  PPS   BPS   PPS   BPS");
		printf("|\n");
		for (i = 0; i < totaldev; i++)
			printf("+-----------------------");
		printf("+\n");
		if (outlen == 0) {
			outlen++;
			return;
		} else if (outlen == 20)
			outlen = 0;
	}
	outlen++;
	printf("|");
	for (i = 0; i < totaldev; i++) {
		printf("%4.0f%c %4.0f%c %4.0f%c %4.0f%c|",
		       Nom1((float)tp[i]), Nom2((float)tp[i]),
		       Nom1((float)tb[i] * 8.0), Nom2((float)tb[i] * 8.0),
		       Nom1((float)rp[i]), Nom2((float)rp[i]),
		       Nom1((float)rb[i] * 8.0), Nom2((float)rb[i] * 8.0));
	}
	printf("\n");

}

int main(int argc, char *argv[])
{
	int i;
	for (i = 1; i < argc; i++) {
		if (strcmp(argv[i], "-h") == 0) {
			printf("Usage: nettraf [ -h ] [ NIC1 ] [ NIC2 ] [ NIC3 ]\n");
			exit(0);
		}
		if (totaldev >= MAXDEV) {
			printf("too much devs\n");
		}
		strncpy(devname[totaldev], argv[i], DEVNAMELEN);
		totaldev++;
	}
	if (totaldev == 0) {
		FILE *fp;
		char buf[MAXLEN];
		fp = fopen("/proc/net/dev", "r");
		if (fp == NULL) {
			fprintf(stderr, "Could not open /proc/net/dev\n");
			exit(0);
		}
		while (fgets(buf, MAXLEN, fp) != NULL) {
			char *devn;
			char *p = buf;
			if (strstr(buf, ":") == 0)
				continue;
			if (strstr(buf, "lo"))
				continue;
			while (*p == ' ')
				p++;
			if (*p == 0)
				continue;
			devn = p;
			while (*p && (*p != ':'))
				p++;
			if (*p == 0)
				continue;
			*p = 0;
			strncpy(devname[totaldev], devn, DEVNAMELEN);
			totaldev++;
			if (totaldev == MAXDEV)
				break;
		}
		fclose(fp);
	}
	while (1) {
		dostatis();
		sleep(1);
	}
	return 0;
}

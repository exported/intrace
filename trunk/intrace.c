/*
 * intrace
 *
 * Main
 *
 * author: Robert Swiecki <robert@swiecki.net>
 */

#include <config.h>

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <time.h>
#include <string.h>

#include <intrace.h>

extern char *optarg;

int main(int argc, char **argv)
{
	char c;
	int dl = dlInfo, err;
	intrace_t intrace;

	bzero(&intrace, sizeof(intrace_t));
	intrace.paylSz = 1;

	printf(INTRACE_NAME ", version " INTRACE_VERSION " " INTRACE_AUTHORS"\n");

	for (;;) {
		c = getopt(argc, argv, "h:p:d:s:");
		if (c < 0)
			break;

		switch (c) {
		case 'h':
			intrace.hostname = optarg;
			break;
		case 'p':
			intrace.port = atoi(optarg);
			break;
		case 'd':
			dl = atoi(optarg);
			break;
		case 's':
			intrace.paylSz = atoi(optarg);
			break;
		default:
			break;
		}
	}

	/* Initialize subsystems */
	if ((err = _debug_init(dl, NULL)) < 0) {
		fprintf(stderr, "Can't initialize debug, err=%d!\n", err);
		return err;
	}

	if (!intrace.hostname) {
		debug_printf(dlInfo, "Usage: %s <-h hostname> [-p <port>] [-d <debuglevel>] [-s <payloadsize>]\n", argv[0]);
		return errArg;
	}

	if (intrace.paylSz < 0) {
		debug_printf(dlWarn, "Payload size set to 0\n");
		intrace.paylSz = 0;
	}

	if (intrace.paylSz > MAX_PAYL_SZ) {
		debug_printf(dlWarn, "Payload size set to %d\n", MAX_PAYL_SZ);
		intrace.paylSz = MAX_PAYL_SZ;
	}

	return threads_process(&intrace);
}
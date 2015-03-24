#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
	int opt, files, i;
	int append_flag = 0;
	int *fds;
	int open_mode = S_IRUSR|S_IWUSR|S_IRGRP|S_IROTH;
	char buf[1024];
	ssize_t len;

	while ((opt = getopt(argc, argv, "a")) != -1) {
		switch (opt) {
		case 'a':
			append_flag = 1;
			break;
		default:
			fprintf(stderr, "Usage: tee [-a] [files]...\n");
			exit(1);
		}
	}

	files = argc - optind;
	fds = (int*)malloc(sizeof(int) * files);
	if (fds == NULL) {
		perror("malloc");
		exit(1);
	}

	for (i = 0; i < files; i++) {
		int open_flag;
		if (append_flag == 1) {
			open_flag = O_WRONLY | O_CREAT | O_APPEND;
		} else {
			open_flag = O_WRONLY | O_CREAT | O_TRUNC;
		}

		fds[i] = open(argv[optind+i], open_flag, open_mode);
		if (fds[i] == -1) {
			perror("open");
			exit(1);
		}
	}

	while ((len = read(STDIN_FILENO, buf, sizeof(buf))) > 0) {
		write(STDOUT_FILENO, buf, len);
		for (i = 0; i < files; i++) {
			ssize_t written = write(fds[i], buf, len);
			if (written == -1) {
				perror("write");
				exit(1);
			}
		}
	}

	for (i = 0; i < files; i++) {
		if (close(fds[i]) == -1) {
			perror("open");
			exit(1);
		}
	}

	return 0;
}

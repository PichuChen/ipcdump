// Pichu Chen 2020, All right reserved.

#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/errno.h>
#include <sys/mman.h>

#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int shmkey = 0;
char* file_path = "out.shm";
unsigned char debug_mode = 0;


void usage();

void dump(const int shmkey, const char* file_path);

int main(int argc, char **argv) {

	int opt = 0;


	while(1) {
		opt = getopt(argc, argv, "dM:o:");
		if (opt == -1){
			break;
		}

		switch(opt) {
			case 'd':
				debug_mode = 1;
				break;
			case 'M':
				// For shmem-id
				shmkey = atoi(optarg);
				if(errno != 0) {
					// TODO: error
				}

			break;

			case 'o':
				// For output file
				fprintf(stdout, "file: %s\n", optarg);
				int len = strlen(optarg);
				file_path = (char*)malloc(len + 1);
				strncpy(file_path, optarg, len);
				break;
			case 'h':
			default:
				usage();
			break;
		}

	}


	if(debug_mode == 1) {
		printf("shmkey: %d, file; %s\n", shmkey, file_path);
	}

	dump(shmkey, file_path);

}


void usage() {
	// TODO
}



void dump(const int shmkey, const char* file_path) {
	int id = shmget(shmkey, 0, 0);
	if(errno != 0) {
		printf("shmget error: %s(%d)\n", strerror(errno), errno);
		return;
	}

	void *src = (void *)shmat(id, NULL, 0);
	if(errno != 0) {
		printf("shmat error: %s(%d)\n", strerror(errno), errno);
		return;
	}
	// Get size
	struct shmid_ds buf;
	shmctl(id, IPC_STAT, &buf);
	if(errno != 0) {
		printf("shmctl error: %s(%d)\n", strerror(errno), errno);
		return;
	}
	size_t size = buf.shm_segsz;
	if(debug_mode == 1) {
		printf("got shm size with %zu Byte(s)\n", size);
	}

	int fd = open(file_path, O_RDWR | O_TRUNC | O_CREAT, 0600);
	if(errno != 0) {
		printf("open error: %s(%d)\n", strerror(errno), errno);
		return;
	}

	ftruncate(fd, size);
	if(errno != 0) {
		printf("ftruncate error: %s(%d)\n", strerror(errno), errno);
		return;
	}

	void * dst = mmap(NULL, size, PROT_WRITE, MAP_FILE | MAP_SHARED, fd, 0);
	if(errno != 0) {
		printf("mmap error: %s(%d)\n", strerror(errno), errno);
		return;
	}

	memcpy(dst, src, size);
	if(errno != 0) {
		printf("memcpy error: %s(%d)\n", strerror(errno), errno);
		return;
	}

	// munmap(dst, size);
	// if(errno != 0) {
	// 	printf("munmap error: %s(%d)\n", strerror(errno), errno);
	// 	return;
	// }

	// close(fd);
	// if(errno != 0) {
	// 	printf("close error: %s(%d)\n", strerror(errno), errno);
	// 	return;
	// }




}
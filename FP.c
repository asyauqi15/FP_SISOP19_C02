#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <stdlib.h>
#include <time.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pthread.h>
#include <grp.h>
#include <pwd.h>

#define MAX 10240
//Queue

char queue_array[MAX][MAX];
int rear = - 1;
int front = - 1;

void push(char *dir)
{
    if (rear < MAX - 1)
    {
        if (front == - 1) front = 0;
        rear = rear + 1;
        strcpy(queue_array[rear], dir);
    }
}

void pop()
{
    if (front > -1 && front <= rear)
	{
        front = front + 1;
    }
}
//End Queue

static const char *source = "/home/pejuangsisop";

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char filename[1024];
	sprintf(filename, "%s%s", source, path);

	int res;

	res = lstat(filename, stbuf);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
	char filename[1024];
	sprintf(filename, "%s%s", source, path);

	int fd;
	int res;

	(void) fi;
	fd = open(filename, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}


static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
	char huft[10240];
	sprintf(huft, "%s", source);
	push(huft);

	DIR *dp;
	struct dirent *de;

	(void) offset;
	(void) fi;

	while(front<=rear){
		char filename[10240];
		sprintf(filename, "%s%s", queue_array[front], path);
		dp = opendir(filename);

		// if (dp == NULL)
		// 	return -errno;

		while ((de = readdir(dp)) != NULL) {
			if (de->d_type == 4 && strcmp(de->d_name, ".") && strcmp(de->d_name, "..") && strcmp(de->d_name, "FP_Mount"))
			{
				char filename2[10240];
				sprintf(filename2, "%s/%s", queue_array[front], de->d_name);
				push(filename2);
			}
			if((strstr(de->d_name, ".c")!=NULL || de->d_type == 4) && strcmp(de->d_name, "FP_Mount"))
			{
				struct stat st;
				memset(&st, 0, sizeof(st));
				st.st_ino = de->d_ino;
				st.st_mode = de->d_type << 12;
				if (filler(buf, de->d_name, &st, 0)) break;
			}
		}
		closedir(dp);
		pop();
	}
	return 0;
}

static int xmp_open(const char *path, struct fuse_file_info *fi)
{
	char filename[1024];
	sprintf(filename, "%s%s", source, path);

	int fd;

	fd = open(filename, fi->flags);
	if (fd == -1)
		return -errno;

	fi->fh = fd;
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	char filename[1024];
	sprintf(filename, "%s%s", source, path);

	int res;

	(void) filename;
	res = pwrite(fi->fh, buf, size, offset);
	if (res == -1)
		res = -errno;

	return res;
}

static int xmp_access(const char *path, int mask)
{
	char filename[1024];
	sprintf(filename, "%s%s", source, path);

	int res;

	res = access(filename, mask);
	if (res == -1)
		return -errno;

	return 0;
}

static struct fuse_operations xmp_oper = 
{
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.access		= xmp_access,
	.open		= xmp_open,
	.read		= xmp_read,
	.write		= xmp_write,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
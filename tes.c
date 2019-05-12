#define _POSIX_SOURCE
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#undef _POSIX_SOURCE
#include <stdio.h>
#include <string.h>

#define MAX 10240

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

int main()
{
    char huft[10240];
	sprintf(huft, "%s", "/home/pejuangsisop");
	push(huft);

    DIR *dp;
	struct dirent *de;

	while(front<=rear){
		char filename[10240];
		sprintf(filename, "%s/", queue_array[front]);
		dp = opendir(filename);

		while ((de = readdir(dp)) != NULL) {
			if (de->d_type == 4 && strcmp(de->d_name, ".") && strcmp(de->d_name, "..") && strcmp(de->d_name, "FP_Mount"))
			{
				char filename2[10240];
				sprintf(filename2, "%s/%s", queue_array[front], de->d_name);
				push(filename2);
			}
		}
		closedir(dp);
		pop();
	}

    front=0;
    while (front<=rear)
    {
        char filename[10240];
		sprintf(filename, "%s/", queue_array[front]);
		dp = opendir(filename);

		while ((de = readdir(dp)) != NULL) {
			if (strstr(de->d_name, ".c\0")!=NULL)
				printf("%s\n", de->d_name);
		}
		closedir(dp);
		pop();
    }

    return 0;
}
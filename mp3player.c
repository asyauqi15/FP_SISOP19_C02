#include <ao/ao.h>
#include <mpg123.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <dirent.h>
#define BITS 8

char filelist[index];
int play_status = 0;

void* play(int argc, char *argv[]){
    play_status = 1;
    mpg123_handle *mh;
    unsigned char *buffer;
    size_t buffer_size;
    size_t done;
    int err;

    int driver;
    ao_device *dev;

    ao_sample_format format;
    int channels, encoding;
    long rate;

    /* initializations */
    ao_initialize();
    driver = ao_default_driver_id();
    mpg123_init();
    mh = mpg123_new(NULL, &err);
    buffer_size = mpg123_outblock(mh);
    buffer = (unsigned char*) malloc(buffer_size * sizeof(unsigned char));

    /* open the file and get the decoding format */
    mpg123_open(mh, argv[1]);
    mpg123_getformat(mh, &rate, &channels, &encoding);

    /* set the output format and open the output device */
    format.bits = mpg123_encsize(encoding) * BITS;
    format.rate = rate;
    format.channels = channels;
    format.byte_format = AO_FMT_NATIVE;
    format.matrix = 0;
    dev = ao_open_live(driver, &format, NULL);

    /* decode and play */
    while(1){
        while (play_status == 1){
            mpg123_read(mh, buffer, buffer_size, &done)
            ao_play(dev, buffer, done);
        }
        if (play_status == 0){
            break;
        }

        sleep(1);
    }

    /* clean up */
    free(buffer);
    ao_close(dev);
    mpg123_close(mh);
    mpg123_delete(mh);
    mpg123_exit();
    ao_shutdown();

    return 0;
}

void* pause(void *arg){
    if(play_status = 1){
        play_status = 0;
    }
}

void* resume(void *arg){
    if(play_status = 0){
        play_status = 1;
    }
}

void* next(void* arg){
}

void* prev(void* arg)

void* list(void* arg)

int main()
{
    DIR *dp;
    struct dirent *dir;
    dp = opendir("/home/becak/Music");

    if (dp){
        while ((dir = readdir(dp)) != NULL){
            char temp[1024];
            if(strcmp(dir->d_name,".") == 0 || strcmp(dir->d_name,".." == 0)){}
            else{
                strcpy(filelist[index], dir->d_name);
                index++;
            }
        }
        closedir(dp);
    }

    pthread_t thread1, thread2, thread3, thread4, thread5, thread6;
    char input[1024];
    pthread_create(&(thread1), NULL, play, NULL);
    pthread_create(&(thread2), NULL, pause, NULL);
    pthread_create(&(thread3), NULL, resume, NULL);
    pthread_create(&(thread4), NULL, next, NULL);
    pthread_create(&(thread5), NULL, prev, NULL);
    pthread_create(&(thread6), NULL, list, NULL);

    while(1){
        fgets(input, 1024, stdin);

        if(strcmp(input, "play\n") == 0)
        {
            status = 1;
        }

        else if(strcmp(input, "resume\n") == 0)
        {
            status = 2;
        }

        else if(strcmp(input, "stop\n") == 0)
        {
            status = 3;
        }
        else if(strcmp(input, "exit\n") == 0)
        {
            /* clean up */
            free(buffer);
            ao_close(dev);
            mpg123_close(mh);
            mpg123_delete(mh);
            mpg123_exit();
            ao_shutdown();
            return 0;
        }
    }

    pthread_join(thread1,NULL);
    pthread_join(thread2,NULL);
    pthread_join(thread3,NULL);
    pthread_join(thread4,NULL);
    pthread_join(thread5,NULL);
    pthread_join(thread6,NULL);
}
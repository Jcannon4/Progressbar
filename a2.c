//Jaxon
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#define PROGRESS_STATUS {0, 0, 0}

struct prog_stat{
    int initial_value;
    int * current_status;
    int termination_value;
};
void* progress_monitor (void * arg) {
   struct prog_stat *threadProg = (struct prog_stat*) malloc(sizeof(arg));
   int i =0;
    for(; i<1; i++)
    {
        sleep(1);
        printf("My Turn %d\t%p\t%d\t%d\n",i, (void *)threadProg->current_status , threadProg->initial_value, threadProg->termination_value);
    }
    return threadProg;
}
void* wordcount (void * arg) {
    FILE *fptr;
    int i = 0;
    fptr = fopen("sample.txt", "r");
    if(fptr == NULL){
        printf("OPENING FILE ERROR");
        exit(1);
    }
    for(; i< 1; i++)
    {
        sleep(1);
        printf("next thread %d\n", i);
    }
    return NULL;
    
}
int main (){
    struct prog_stat curProg = PROGRESS_STATUS;
    struct stat buff;
    int initial_value = 0;
    char *fd = "sample.txt";
    if (stat(fd, &buff) == 0)
    {
        curProg.termination_value =  buff.st_size;
        printf("Byte Size: %d\n", curProg.termination_value);
    }
    else
    {
        printf("File not found\n");
        printf("Check that '%s' file exists.\n", "sample.txt");
    }
    pthread_t newthread;
    int *result;
    pthread_create(&newthread, NULL, progress_monitor, &curProg);
    wordcount(&curProg);
    pthread_join(newthread, (void *)&result);
    printf("calling before seg fault\n");
    printf("THREAD DONE result=%d\n", *result);
    return 0;
    
}
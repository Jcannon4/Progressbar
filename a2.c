//Jaxon
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
//#define PROGRESS_STATUS {0, 0, 0}

typedef struct{
    long initial_value;
    long * current_status;
    long termination_value;
} prog_stat;

void* progress_monitor (void * arg) {
   //struct prog_stat *threadProg = (struct prog_stat*) arg;
   prog_stat *threadProg = arg;
   int i =0;
    for(; i<threadProg->termination_value; i++)
    {
        usleep(250000);
        printf("-");
        fflush(stdout);
        //printf("My Turn %ld\t%p\t%ld\t%ld\n",i, (void *)threadProg->current_status , threadProg->initial_value, threadProg->termination_value);
    }
    return threadProg;
}
int wordcount (void * arg, char *filedesc) {
    int iochar;
    
    //struct prog_stat *threadProg = (struct prog_stat*) arg;
    prog_stat *threadProg = arg;
    FILE *fptr;
    int i = 0;
    printf("CALLING\n");
    char str[1];
    fptr = fopen(filedesc, "r");
    if(fptr != NULL){
        printf("file opened succesfully\n");
        for(; i< threadProg->termination_value; i++)
        {
            if(fgets(str, 1, fptr) != NULL)
            {
                (threadProg->current_status)++;
                printf("wordcount working : %p\n" , threadProg->current_status );
            }
        
         } 
    }
    else{printf("ERROR OPENING FILE");}
    return 2;
    
}

int main (int argc, char* argv[]){
    prog_stat *curProg = (prog_stat*)malloc(sizeof(prog_stat));
    //struct prog_stat curProg = PROGRESS_STATUS;
    struct stat buff;
    int initial_value = 0;
    
    char *fd = argv[1];
    printf("arg array: %s\t%s\n", argv[0], argv[1]);
    if (stat(fd, &buff) == 0)
    {
        curProg->termination_value =  buff.st_size;
        curProg->initial_value = 0;
        curProg->current_status = 0;
        printf("Byte Size: %ld\n", curProg->termination_value);
    }
    else
    {
        printf("File not found\n");
        printf("Check that file : '%s' file exists.\n", argv[1]);
        exit(2);
    }
    pthread_t newthread;
    int *result;
    pthread_create(&newthread, NULL, progress_monitor, &curProg);
    int iochar = wordcount(&curProg, argv[1]);
    pthread_join(newthread, (void *)&result);
    printf("\ncalling before seg fault\n");
    printf("THREAD DONE result=%d\nwordcount = %d\n", *result, iochar);
    return 0;
    
}
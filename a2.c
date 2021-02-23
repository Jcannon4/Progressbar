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
   long i = 0;
    for(; i<threadProg->termination_value; i++)
    {
        usleep(250000);
        printf("-");
        fflush(stdout);
        //printf("My Turn %ld\t%p\t%ld\t%ld\n",i, (void *)threadProg->current_status , threadProg->initial_value, threadProg->termination_value);
    }
    return threadProg;
}
int wordcount (prog_stat *arg, char *filedesc) {
    int iochar;
    
    //struct prog_stat *threadProg = (struct prog_stat*) arg;
    prog_stat *threadProg = arg;
    
    FILE *fptr;
    long i = 0;
    printf("CALLING\n");
    char str[1];
    fptr = fopen(filedesc, "r");
    long *init;
    init = (long *)malloc(sizeof(threadProg->current_status));
    //init = threadProg->current_status;
    long current = threadProg->initial_value;
    long term = threadProg->termination_value;
    printf("current : %p\ninit : %ld\nBYTES: %ld\n", init, current, term);
    if(fptr != NULL){
        printf("file opened succesfully\n");
        for(; i< threadProg->termination_value; i++)
        {
            if(fgets(str, 1, fptr) != NULL)
            {
                (*init)++;
                printf("wordcount working : %ld\n" , *init );
            }
        
         } 
    }
    else{printf("ERROR OPENING FILE");}
    return 4;
    
}

int main (int argc, char* argv[]){
    prog_stat *curProg = (prog_stat*)malloc(sizeof(prog_stat));
    //struct prog_stat curProg = PROGRESS_STATUS;
    struct stat buff;
    //int initial_value = 0;
    
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
    pthread_attr_t	pthread_attributes;
    int *result;
    printf("BYTE SIZE CHECK : %ld\n", curProg->termination_value);
    pthread_attr_init(&pthread_attributes);
    pthread_create(&newthread, &pthread_attributes, progress_monitor, &curProg);
    wordcount(curProg, argv[1]);
    pthread_join(newthread, (void *)&result);
    printf("\ncalling before seg fault\n");
    printf("THREAD DONE result=%d\nwordcount = \n", *result);
    return 0;
    
}
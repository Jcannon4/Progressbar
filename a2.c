//Jaxon
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#define PROGRESS_STATUS {0, 0, 0}

struct prog_stat{
    long initial_value;
    long * current_status;
    long termination_value;
};
void* progress_monitor (void * arg) {
   struct prog_stat *threadProg = (struct prog_stat*) arg;
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
int wordcount (void * arg) {
    int iochar;
    struct prog_stat *threadProg = (struct prog_stat*) arg;
    FILE *fptr;
    int i = 0;
    printf("CALLING\n");
    char str[1];
    //fptr = fopen("sample.txt", "r");
    
    
//    for(; i< threadProg->termination_value; i++)
//     {
//         if(fgets(str, 1, fptr) != NULL){
//            (*threadProg->current_status)++;
//            //printf("wordcount working : %ld\n", *(threadProg->current_status +1));
//             }
        
//    }
    
    return 2;
    
}
int main (int argc, char* argv[]){
    
    struct prog_stat curProg = PROGRESS_STATUS;
    struct stat buff;
    int initial_value = 0;
    
    char *fd = argv[1];
    printf("arg array: %s\t%s\n", argv[0], argv[1]);
    if (stat(fd, &buff) == 0)
    {
        curProg.termination_value =  buff.st_size;
        printf("Byte Size: %ld\n", curProg.termination_value);
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
    int iochar = wordcount(&curProg);
    pthread_join(newthread, (void *)&result);
    printf("\ncalling before seg fault\n");
    printf("THREAD DONE result=%d\nwordcount = %d\n", *result, iochar);
    return 0;
    
}
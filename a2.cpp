//Jaxon

#define whiteSpace ' '
#define newline '\n'
#define tab '\t'
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
//#define PROGRESS_STATUS {0, 0, 0}

typedef struct {
    long initial_value;
    long * current_status;
    long termination_value;
} prog_stat;

void* progress_monitor (void * recArg){
    prog_stat *progbar = (prog_stat*)recArg;
   // printf("initial value:  %ld, termination_value %ld\n", progbar->initial_value, progbar->termination_value);
    long current = *progbar->current_status;
    long init = (progbar->initial_value);
    long term = progbar->termination_value;
    long fraction = progbar->termination_value/50;
    printf("fraction : %ld\n",fraction);
    long i = 0;
    while(*progbar->current_status < term)
    {
        
        //printf("-%ld", *progbar->current_status);
       // printf("\n%ld\n",*progbar->current_status);
      
        fflush(stdout);
        //printf("My Turn %ld\t%p\t%ld\t%ld\n",i, (void *)threadProg->current_status , threadProg->initial_value, threadProg->termination_value);
    }
    return progbar;
}
int wordcount (char *filedesc) {
    struct stat buff;
    char *fd = filedesc;
    printf("file to be read: %s\n", filedesc);
    prog_stat *threadProg = new prog_stat;
    threadProg->current_status = new long;

    if (stat(fd, &buff) == 0)
    {
        threadProg->termination_value =  buff.st_size;
        threadProg->initial_value = 0;
        *threadProg->current_status = 0;
        printf("Byte Size: %ld\n", threadProg->termination_value);
    }
    else
    {
        printf("File not found\n");
        printf("Check that file : '%s' file exists.\n", filedesc);
        exit(2);
    }
    printf("BYTE SIZE CHECK : %ld\n", threadProg->termination_value);

    pthread_t newthread;
    pthread_attr_t	pthread_attributes;
    FILE *fptr;
    fptr = fopen(filedesc, "r");
    long i = 0;
    char str[255];

    // long *current;
    // current = (long *)malloc(sizeof(threadProg->current_status));
    //long *current = threadProg->current_status;
    long init = threadProg->initial_value;
    long term = threadProg->termination_value;
    printf("current : %ld\ninit : %ld\nBYTES: %ld\n", *threadProg->current_status, init, term);
    int *result;
    int iochar;
    pthread_attr_init(&pthread_attributes);
    pthread_create(&newthread, NULL, &progress_monitor, (void *)threadProg);

    int wCount = 0;
    

    int charcount = 0;
    while((iochar = fgetc((FILE*)fptr)) != EOF){
        //printf("ANOTHER ITR\n");
        if (iochar == whiteSpace || iochar == newline || iochar == tab){
            if(charcount != 0)
            { 
                wCount++;
            }
            charcount = 0;
            //printf("count1 = %d\n", wCount);
        }
        else { 
            charcount++;
        }
        
        (*threadProg->current_status)++;
        //printf("charcount: %ld\n", *threadProg->current_status);
        
    }
    if(iochar == EOF && charcount !=0 ){
        charcount = 0;
        wCount++;

    }
    
    pthread_join(newthread, NULL);
    return wCount;
    
}

int main (int argc, char* argv[]){
     int count = wordcount(argv[1]);
     printf("ending statement\nword count = %d\n", count);
    return 0;
    
}
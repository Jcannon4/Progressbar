/*  Jackson Contreras : 820436208 : cssc3511
*   Ahmet Gueye : : 
*   Professor Shen 
*   CS570-Operating Systems
*   02/23/2021
*   Assignment #2 "Poor Man's Progress Bar"
*
*/
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/stat.h>
#define whiteSpace ' '
#define newline '\n'
#define tab '\t'


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
    long i = 0;
    float threshold = 1.0 / 50.0;
    int perTick = 1;
    int prevTick = 0;
    //printf("LAST TERM CHECK : %f\n", threshold);
    while(*progbar->current_status <= term && perTick < 51)
    {
       float fraction;
        if(progbar->termination_value == 0){
            int i = 1;
            fraction = 0;
            for(; i < 51; i++){
                if(i%10 == 0){
                    usleep(25000);
                    printf("+");
                } else{usleep(25000);printf("-");}
                
            }
            break;
        } else {
           fraction = *progbar->current_status * 1.0 / (progbar->termination_value);
            }
        if(fraction >= (threshold * perTick)){
            if(perTick%10 == 0)
            {
            usleep(25000);
            printf("+");
            }else{
            usleep(25000);
            printf("-");
            }
        fflush(stdout);
        perTick++;

        }
        
    }
    printf("\n");
    return progbar;
}
int wordcount (char *filedesc) {
    struct stat buff;
    char *fd = filedesc;
    prog_stat *threadProg = new prog_stat;
    threadProg->current_status = new long;
    if(filedesc == NULL){
        printf("no file specified\n");
        exit(10);
    }
    if (stat(fd, &buff) == 0)
    {
        threadProg->termination_value =  buff.st_size;
        threadProg->initial_value = 0;
        *threadProg->current_status = 0;
        printf("Byte Size: %ld\n", threadProg->termination_value);
    }
    else
    {
        printf("could not open file\n");
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
     printf("There are %d words in %s.\n", count, argv[1]);
    return 0;
    
}
/*  
   Jackson Contreras : 820436208 : cssc3511
   Ahmet Gueye : 821319753:  cssc3519
   Professor Shen 
   CS570-Operating Systems
   02/23/2021
   Assignment #2 "Poor Man's Progress Bar"
   
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
    //Bring in struct
    prog_stat *progbar = (prog_stat*)recArg;
    //put our total number of bytes in a new value for simplicity
    long term = progbar->termination_value;
    //Each symbol + or -, represents 2% of the program being finished. So lets
    //define a ratio to compare our current status to.
    float threshold = 1.0 / 50.0;
    
    int perTick = 1; //current iteration, we will multiply our ration by this later 
    //run our progress monitor until current_Status reaches termination value
    //Exit once weve printed 50 symbols
    while(*progbar->current_status <= term && perTick < 51)
    {
        //This block catches an important edge case. If the file is empty then there are 0 bytes
       float fraction;
        if(progbar->termination_value == 0){ //There aree in fact 0 bytes
            int i = 1;
            fraction = 0;
            for(; i < 51; i++){ //so lets run through and print the 50 symbols
                if(i%10 == 0){
                    usleep(25000);
                    printf("+"); //every 10th symbol is a +
                } else{usleep(25000);printf("-");} //all other symbols are -
                fflush(stdout);
            }
            break; //break out of while loop so thread can return back to getword()
        } else { //File size is greater than 0 bytes
        //fraction will be a decimal value of programs progress 
           fraction = *progbar->current_status * 1.0 / (progbar->termination_value); //if denom was 0 we'd be introuble!
            }
            //when our fraction surpasses a threshold then we have the signal to print
            //compares current progress as a decimal to pending symbol's threshold
        if(fraction >= (threshold * perTick)){
            if(perTick%10 == 0) //every 10th symbol is a +
            {
            usleep(25000); //slows down print to give visible animation
            printf("+");
            }else{
            usleep(25000);
            printf("-"); //aall other symbols are a -
            }
        fflush(stdout); //keeps progress bar on the same line of output
        perTick++; //Very important!!
        //notifies us we have printed the threshold, so next iteration the threshold will have to be higher
        //namely our threshold (1/50) * perTick (1) 
        //first iteration 1/50 * 1 is threshold
        //second iteration is 1/50 * 2 etc up to 50

        }
        
    }
    printf("\n");
    return progbar;
}
int wordcount (char *filedesc) {
   
    struct stat buff; //necessary to get total number of bytes
    char *fd = filedesc; //bring in file to open

    prog_stat *threadProg = new prog_stat; //instance of our custom struct
    threadProg->current_status = new long; //instantiate struct's pointer 

    //check if an argument is passed
    if(filedesc == NULL){
        printf("no file specified");
        exit(10); //end program if nothing wsa supplies
    }
    if (stat(fd, &buff) == 0) //utilizes stat while also null checking
    {
        threadProg->termination_value =  buff.st_size; //struct has unique byte count
        threadProg->initial_value = 0; //file starts at 0 bytes
        *threadProg->current_status = 0; //instance starts at 0 bytes
    }
    else
    {
        printf("could not open file");
        exit(2); //if stat fails we come here and terminate program
    }

    //instantiate pthread object
    pthread_t newthread;
    //Grab file pointer so we can fgetc it later
    FILE *fptr;

    //file open attempt, null check was done earlier so would be redundant here
    fptr = fopen(filedesc, "r");

    //pass in instantiated thread, we dont need thread_attributes so second arg is NULL
    //third argument is our thread function name
    //fourth argument is our struct typecasted as a void pointer
    pthread_create(&newthread, NULL, &progress_monitor, (void *)threadProg);

    //variables that will help us track when words end
    int wCount = 0;
    int iochar;
    int charcount = 0;

    //Keep grabbing chars until End OF File is reached
    while((iochar = fgetc((FILE*)fptr)) != EOF){
        //check if current char is a whitespace
        if (iochar == whiteSpace || iochar == newline || iochar == tab){
            //if there is a character length followed by a whitespace then we have a word
            if(charcount != 0)
            { 
                //continuous whitespaces do not enter here
                wCount++;
            }
            //since we have a whitespace lets reset our charcount so we can catch the next word or whitespace
            charcount = 0;
        }
        //no whitespace detected so we have a character, lets increase charcount, this way
        //we can tell if a white space followed a word or another whitespace
        else { 
            charcount++;
        }
        //regardless of white space or character we increase our current_status
        (*threadProg->current_status)++;
    }
    //edge case to catch word followed by EOF signal
    if(iochar == EOF && charcount !=0 ){
        charcount = 0;
        wCount++;
    }
    //wait for thread functino to finish
    pthread_join(newthread, NULL);
    //return wordcount
    return wCount;
}

// main : Takes one file as an argument to read the amount of words in it.
// invokes wordcount() which will do most of the work
int main (int argc, char* argv[]){
     int count = wordcount(argv[1]);
     printf("There are %d words in %s.", count, argv[1]); //prints when wordcount finishes and thread completes
    return 0;
}
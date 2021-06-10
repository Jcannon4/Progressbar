# Progressbar


=======
Progress Bar utilizing threading with C++
Jackson's branch. Feel free to cop/paste to your branch or use git commands. which ever your preference is
=======
This is assignment 2 for SDSU cs570. The purpose is to create a progress bar
using POSIX threads.

a2.cpp
main : Takes one file as an argument to read the amount of words in it.
invokes wordcount() which will do most of the work

    wordcount : Attmepts to open a given file, exits program if open fails. Upon
    success the program initializes the struct prog_stat and assigns current_status
    and initial_value to 0. Also sets termination_value to the size of the file in Bytes.
    This method then instantiates a pthread so we can run out progress monitor function in the background.
    Calls method pthread_create to intiate threading. Lastly, program iterates through
    each character from the given file and separates them by white space. Checks for
    empty file condition so we don't divide by 0! When a character is found the
    current_status of the program is updated so our thread can output progress.
    pThread_join waits for thread to finish before getword() returns the
    amount of word found in this file.

    progress_monitro : Take as void pointer as an argument. This is mandatory in the C
    programming language. The prog_stat struct is type casted so it can be passed into the thread.
    Initializes a float threshold as .02 or (1/50) so calculation of when to place respective
    '-' or '+' symbols can easily be indentified. The while loop will run while our current byte count is less than our termination value and terminate after 50 iterations. The first block is to catch a 0 byte edge case, it prints everything and manually breaks while loop after all symbols are printed. if our termination_value is greater than 0, we create a fraction to track current status progress. If our frraction passed the threshold for its respective iteration (denoted by perTick) then it will print a symbol. Every 10th symbol is a + all others are a -. Lastly, fflush(stdout) to add symbols to the same line and increase our iteration tracker (perTick). returns the void pointer struct prog_stat.
>>>>>>> Jax

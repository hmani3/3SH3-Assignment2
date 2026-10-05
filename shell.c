#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>

#define MAX_LINE 80
#define HISTORY_STORED 5

typedef struct {
    char commands[HISTORY_STORED][MAX_LINE];
    int  command_count; // trakc the current number of commands globally to mark plus x to 5 on each ommand
} CommandHistory;

void initialise_command_history (CommandHistory *tracked) {
    // initialize each history line as empty char, and total command counts at 0
    for (int i = 0; i < HISTORY_STORED; i ++){ tracked->commands[i][0] = '\0'; }
    tracked->command_count = 0;
}

void add_command_to_history (CommandHistory *tracked, const char *exact_input) {
    // we will add each command as a raw strinng, and increment total commands
    // Wwe ALWAYS have exactly 5 elements, impleement as circular buffer, i.e.,
    // place of next element is command_count % 5
    strcpy(tracked->commands[tracked->command_count % 5], exact_input);
    tracked->command_count ++;
}

void print_history (CommandHistory *tracked) {
    // if the history command is detected, print just this command skip other logic
    // We want to sstart at the beginning, print 0-4 + commandcount (converts to 1-index n tracks total)
    // If we encounter a "\0" instantly terminate
    for (int i = 0; i < HISTORY_STORED; i ++) {
        if (tracked->commands[i][0] == '\0') {continue;}
        printf("%d %s", tracked->command_count-i, tracked->commands[i]);
    }
}

bool replace_with_last_historical_command (CommandHistory *tracked, char *actual_command_buffer) {
    // if command is !!, overwrite the actual buffer with the last tacked comand
    // return true if there is at least 1 command
    if (tracked->command_count-1 >= 0) {strcpy(actual_command_buffer, tracked->commands[tracked->command_count-1 % 5]); return true;}
    return false;
}

int main(void){
    char *args[MAX_LINE/2 + 1]; /* command line arguments -> (x y z .... \0) is MAX_LINE / 2 arguments + 1 for termination
                                    (argument, space) can occur 40 times, which reaches 80 chars, then termination \0 added */
    int should_run = 1;         /* flag to determine when to exit program */

    CommandHistory history;
    initialise_command_history(&history);



    while (should_run) {
        printf("osh> ");
        fflush(stdout);

        /* read input -> fork w/ fork() -> child uses execvp() -> parent will wait() UNLESS command ends with & ??*/

        // first we need to parse input, put in args array

        char input[MAX_LINE]; // initialize input for reading

        if (!fgets(input, MAX_LINE, stdin)) {
            continue; // if no input or some fail skip ths iteration, go to the next and start over
        }

        // wewill match ONLY exactly !! for the command (it wasnt mentioned, but thats how the actual
        // command basically works). If we do, we replace the input with the last historical command
        if (strcmp(input, "!!\n") == 0) { if (!replace_with_last_historical_command(&history, input)) {printf("No commands in history\n"); continue;} }

        // copy input into a new untouched copy for storing in history if all operations complete successfully
        char raw_input_copy[MAX_LINE]; strcpy(raw_input_copy, input);

        char *token = strtok(input, " \n"); /* we split the input into tokens, so we strt at the first token,
                                            and we can iterate from there, by each space + \n */

        
        int i = 0;
        while (token != NULL && i < MAX_LINE / 2) {
            args[i] = token; // store the token as command arg
            i++;
            token = strtok(NULL, " \n"); // get the next token
        }

        args[i] = NULL; // terminate args with null character

        // if no args, kip execution
        if (args[0] == NULL) {continue; }
        // if we know args[0] is exit, dont do heavy lifting, just exit
        else if (strcmp(args[0], "exit") == 0) { should_run = 0; continue; }
        // check if the first command is history. We dont need to worry abt any
        // extra args or & since we wont be running this command async
        else if (strcmp(args[0], "history") == 0) { print_history(&history); continue; }

        // we know we have a clean command here, add it to history no matter what (crash, non sensical, whatevr)
        add_command_to_history(&history, raw_input_copy); 

        // args now contains all commands, we want to check if & is the second last arg, then branch
        // args is [x,y,"\0"] if there are n=2 elements, so if i > 0, n is at least 1, and then 
        // we can check the last element that is not "\0", if it is & we can do parallel logic
        bool parallel_exec = (i > 0 && strcmp(args[i - 1], "&") == 0);

        // now we fork. Here is how it works (my understanding):
        pid_t pid = fork();

        // here, we may hit an issue. If so, that means pid < 0, and we didnt split
        if (pid < 0) { perror("There was an issue when trying to create child process"); }

        // Now, if pid >= 0, we have created two parallel process reading our code
        // the child is reading the same exact code, but it recognizes pid == 0,
        // parent recognizes pid > 0. The child has one job: take the args and attempt
        // to execute the command. We will use execvp() as it will handle all the logic
        // and returns -1 if error, otherwise it just executes like normal

        else if (pid == 0) {
            // execvp executes command x (which should be first arg pased) with th rest of the args 

            // delete the last actual arg if we detect the last is &, since that isnt used by actual execution
            if (parallel_exec) { args[i-1] = NULL;}

            
            // if we werent able to execute, check the returned -1
            // make sure to return, since execvp doesnt gracefully like normal in this case
            if (execvp(args[0], args) == -1) {
                printf("There was an issue when parsing the command (is this a valid linux command?)\n");
                return 1;
            }
        }

        // if we enter here, we are the parent process, so we want to either waait or 
        // continue based on if & was the last argument entered
        else if (pid > 0) {

            // print child process before leaving so we can note whats running
            if (parallel_exec) { printf("Continuing execution while child process with PID %d executes\n", pid); }
            // we wait and record returned pid, if its the childs correct one we knwo we suceeded
            else { 
                int status; pid = wait(&status); 
                if (pid >= 0) { printf("Child process finished execution with PID %d\n", pid); }
                else { perror("Error while waiting for child process execution\n"); }
            }
        }
    }

    return 0;
}



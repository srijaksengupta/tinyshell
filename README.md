# Tinyshell

Tinyshell is a bash shell model based in C++ which is designed to work as command line interpreter. On inputting a command, tinyshell implements fork and creates a new child process. This child process executes the command and after it finishes, it prompts the user for the next input. This shell has the functionality of all general bash commands as well as 2 new commands - history and exit. 

--> The history command displays the last 5 commands inputted into the tinyshell. This command also has a flag "-sbu" which sorts the output of the history command by execution time in descending order. 

--> The exit command would exit out of the shell.


To execute the other general bash commands, we invoke system() which is used to invoke a command execution from a C/C++ program.

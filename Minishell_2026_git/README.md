
This project has been created as part of the 42 curriculum by myivanov and causilva

# Minishell

## Description

Minishell is a simplified Unix shell written in C, developed as part of the 42 curriculum.

The goal of this project is to reproduce the core behavior of Bash by implementing a command interpreter capable of parsing and executing user input while respecting Unix process and file descriptor management.

Minishell supports:

- Execution of system binaries
- Built-in commands
- Environment variable expansion
- Redirections (`<`, `>`, `>>`, `<<`)
- Pipes (`|`)
- Logical operators (`&&`, `||`)
- Parentheses for priority
- Wildcards (`*`)
- Signal handling (Ctrl-C, Ctrl-D, Ctrl-\)

The project focuses on deep understanding of:

- Process creation (`fork`)
- Program execution (`execve`)
- File descriptor manipulation (`dup`, `dup2`, `pipe`)
- Signal handling
- Parsing and tokenization
- Abstract syntax trees (AST)
- Memory management without leaks

The objective is to reproduce Bash-like behavior while respecting the constraints of the 42 Norm and allowed functions.

---

## Instructions

### Compilation

Clone the repository and compile using:


make


To clean object files:


make clean


To remove all generated files:

make fclean


To recompile:


make re


### Execution

After compilation, run:


./minishell


You can then enter commands as in a standard shell:


ls -la
echo "Hello World"
cat file.txt | grep word


To exit the shell:


exit


## Resources

Classic references used during development:

* `man bash`
* `man 2 fork`
* `man 2 execve`
* `man 2 pipe`
* `man 2 dup`
* `man 2 wait`
* GNU Bash Reference Manual
* The Linux Programming Interface — Michael Kerrisk
* Advanced Programming in the UNIX Environment — W. Richard Stevens

### Use of AI

AI tools were used during development for:

* Conceptual clarification of Unix system calls
* Understanding parsing strategies and AST structures
* Debugging edge cases
* Reviewing architectural decisions
* Improving documentation quality

All core logic, implementation, debugging, and design decisions were manually written and validated.

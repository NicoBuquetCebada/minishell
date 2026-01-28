*This project has been created as part of the 42 curriculum by irrevuel and nbuquet-.*

## Description

Minishell is a simplified Unix shell implemented in C as part of the 42 curriculum.
The goal of this project is to gain a deep understanding of how a shell works internally,
focusing on process creation, file descriptors, pipes, redirections, environment variables,
and signal handling.

The shell reproduces essential behaviors of bash, such as executing commands found in the
PATH, handling pipelines and redirections, managing built-in commands, and reacting
correctly to user signals in interactive mode.

This project strictly follows the 42 coding standard (Norm) and respects all constraints
defined in the official Minishell subject.

## Instructions

### Compilation

To compile the project, run the following command at the root of the repository:

```
make
```

This will generate the executable:

```
minishell
```

If you want to clean the `.o` files you can run the following command at the root of the repository:

```
make clean
```

Or if you want to clean also the executable file:

```
make fclean
```

### Execution

Launch the shell by running:

```
./minishell
```

Once started, the program will display a prompt and wait for user commands, behaving
similarly to bash within the scope defined by the subject.

## Resources

### Technical References

The following resources were used to understand and implement the project concepts:

* GNU Bash Manual [https://www.gnu.org/software/bash/manual/](https://www.gnu.org/software/bash/manual/)
* Linux manual pages
* 42 Minishell subject documentation

### Use of Artificial Intelligence

Artificial Intelligence tools were used strictly for educational and explanatory purposes.

AI was used to:

* Clarify technical concepts related to shell behavior and internal mechanisms.
* Better understand how bash handles features such as pipelines, redirections, and signals.
* Generate and reason about edge cases in order to improve the robustness of the project.

AI was not used to generate the final project code or to bypass the learning objectives of
the assignment. All implementation decisions and code were written and validated by the
project authors.

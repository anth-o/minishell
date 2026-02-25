*This project has been created as part of the 42 curriculum by abertolo & adumaine.*

# Minishell

## Description

This project is an implementation of a **minimal Unix shell**, developed as part of the 42 curriculum.

The goal of Minishell is to reproduce the core behavior of **bash**, focusing on how a shell:
> - Parses user input
> - Manages processes
> - Handles file descriptors
> - Executes commands
> - Deals with signals and environment variables

The program runs in interactive mode, displays a prompt, and allows the user to execute commands with redirections, pipes, and built-in functions.

This project is a deep dive into **process management**, **signals**, **pipes**, and **low-level I/O**, with strict constraints on allowed functions.

---

## Instructions

### Compilation

To compile the program, run:

```bash
make
```

This will generate an executable named `minishell`.


### Execution

Run the shell with:

```bash
./minishell
```

The shell will then wait for user input and behave similarly to bash for all required features.

---

## Features

### Command execution

> - Execution of binaries using `PATH`, relative paths, or absolute paths
> - Correct handling of exit statuses

### Built-in commands

> - `echo` (with `-n`)
> - `cd` (relative or absolute path only)
> - `pwd`
> - `export`
> - `unset`
> - `env`
> - `exit`

### Redirections

> - `<` input redirection
> - `>` output redirection
> - `>>` output redirection (append)
> - `<<` heredoc (without history update)

### Pipes

> - Support for pipelines using `|`

### Quotes & expansions

> - Single quotes `'` (no expansion)
> - Double quotes `"` (expansion except `$`)
> - Environment variable expansion (`$VAR`)
> - Exit status expansion (`$?`)

### Signals (interactive mode)

> - `Ctrl-C` : interrupts current command and displays a new prompt
> - `Ctrl-D` : exits the shell
> - `Ctrl-\` : ignored

---

## Technical Overview

> - Fork/exec model for command execution
> - Pipes implemented with `pipe()` and `dup2()`
> - Redirections handled via file descriptor manipulation
> - One global variable **only** for signal handling (signal number only)
> - Parsing separated from execution logic
> - Memory management strictly controlled (no leaks in user code)
> - Readline used for input and history

---

## Limitations

> - No interpretation of unclosed quotes
> - No support for `;`, `\`, `&&`, `||`, wildcards, or parentheses
> - Behavior strictly limited to what is required by the subject

If something is not explicitly required, it is **not implemented**.

---

## Resources

### References

> - GNU Bash Manual
>   - https://www.gnu.org/savannah-checkouts/gnu/bash/manual/
> - Advanced Programming in the UNIX Environment
>   - https://www.apuebook.com/
> - Linux process and signal documentation
>   - https://man7.org/linux/man-pages/

## Use of AI

AI tools were used to:

> - Clarify concepts related to processes, pipes, and signals
> - Help reason about parsing strategies and execution flow
> - Review architecture choices and documentation

All core logic, implementation, debugging, and validation were fully performed and understood by **adumaine** & **abertolo**.
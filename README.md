[![Review Assignment Due Date](https://classroom.github.com/assets/deadline-readme-button-22041afd0340ce965d47ae6ef1cefeee28c7c493a6346c4f15d667ab976d596c.svg)](https://classroom.github.com/a/WIXYXthJ)
# ICSH

## Milestone 1: Interactive Command-Line Interpreter

**Features:**
- Interactive prompt (`icsh $`)
- Built-in commands:
  - `echo <text>`: Prints the text.
  - `!!`: Repeats the last command.
  - `exit <num>`: Exits with the given code (0-255).
- Prints `bad command` for unknown commands.
- Ignores empty input.

**Assumptions/Implementation Notes:**
- Only the last non-empty command is stored for `!!`.
- If `!!` is used before any command, it does nothing.
- The exit code is always truncated to 8 bits.
- The shell prints a welcome message on start.
- The shell prints a bye on exit. 

---

## Milestone 2: Script Mode

**Features:**
- If a filename is given as an argument, the shell reads and executes commands from the file line by line.
- Behaves the same as interactive mode for commands.
- No prompt is shown in script mode.

**Assumptions/Implementation Notes:**
- The script file must be readable.
- The shell exits when it reaches the end of the script or encounters an `exit` command.
- If the file cannot be opened, an error message is printed and the shell exits with code 1.

---

## Milestone 3: Running External Programs

**Features:**
- If a command is not built-in, the shell attempts to execute it as an external program using `fork()` and `execvp()`.
- Waits for the external program to finish before showing the prompt again.

**Assumptions/Implementation Notes:**
- Arguments are passed to external programs.
- If the program cannot be executed, an error message is shown.

---

## Milestone 4: Signal Handler

**Features:**
- Handles `Ctrl+C` (SIGINT) and `Ctrl+Z` (SIGTSTP) to control foreground jobs.
- Built-in `echo $?` prints the exit status of the previous command.

**Assumptions/Implementation Notes:**
- Signal handling does not terminate the shell.

---

## Milestone 5: I/O Redirection

**Features:**
- Supports input (`<`) and output (`>`) redirection for external programs.

**Assumptions/Implementation Notes:**
- Output files are created if it doesn't exist.
- Input files doesn't exist an error is shown.

---

## Milestone 6: Background Jobs and Job Control

**Features:**
- Supports background jobs with `&`.
- Implements `jobs`, `fg`, and `bg` commands for job control.

**Assumptions/Implementation Notes:**
- Prints job status when background jobs complete.
- Only uncompleted jobs are listed with `jobs`.

---

## Milestone 7: Extra Features

**Features:**
- Adds a new built-in command: `game ls`
  - Lists available built-in games (e.g., `snake`).
- Adds a new built-in command: `game snake`
  - Launches a simple CLI version of the classic Snake game inside the shell.
# The GNU Debugger GDB

The GNU Debugger is called `gdb` and is a text mode source
level debugger. Graphical frontends exist but are not part
of GDB itself.

Compile your program with `-g` to include symbol information
and with `-O0` to disable optimizations (otherwise hard to debug).

Typical workflow is to (1) start `gdb` with the program to examine,
(2) set breakpoints, (3) run the program, (4) step through the
code, (5) print and set values, (6) quit GDB.

## Invocation

```sh
gdb                # (use file command to load program)
gdb program        # load program
gdb program PID    # load program and attach to process
gdb program core   # load program and core dump file
```

## Commands

Most commands can be abbreviated to a single character
or two (a unique prefix). Hitting Enter without a command
repeates the last command. Help on commands is available
from within GDB through the `help` command.

- **help** \[*cmd*] / get help (for command *cmd*)
- **file prog** / load *prog* for debugging
- **quit** / quit GDB

Setting breakpoints:

- **b main** / set breakpoint at start of `main` method
- **b fun** / set breakpoint at start of function *fun*
- **b file:line** / set breakpoint at *line* in *file*
- **b +N** / set breakpoint *N* lines down from current
- **info break** / list all breakpoints (note they are numbered)
- **d N** / delete breakpoint number *N*

Running (starting) a program:

- **run** / run loaded program (with no arguments)
- **run args** / run loaded program with given arguments
- **run < file** / run loaded program (redirect stdin to *file*)
- **cont** / continue running after a breakpoint

Stepping through a program:

- **step** / run the next line (stepping into functions)
- **step N** / run the next *N* lines
- **next** / run the next line (do not step into functions)
- **u N** / run until again *N* lines above current
- **fini** / finish running current function
- **return** / return early from current function
- **list** / type source code around where you are
- **l N** / type source code around line *N*
- **l fun** / type source code for function *fun*

Showing and modifying values:

- **p expr** / print value of variable or expression *expr*
- **set var=expr** / set (overwrite) a variable
- **bt** / print a stack trace (call sequence)
- **where** / same as `bt` and `backtrace`
- **up** / **down** / go up/down in the stack (to examine local variables)
- **watch expr** / break whenever the value of *expr* changes
- **display var** / constantly display the value of *var*
- **undisplay var** / stop displaying *var*

## References

Homepage of the GNU Project Debugger:  
<https://www.gnu.org/software/gdb/>

Debugging with GDB (documentation):  
<https://sourceware.org/gdb/download/onlinedocs/gdb/index.html>

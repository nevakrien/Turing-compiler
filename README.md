# Turing-compiler
an optimizing compiler to a binary turing machine

the syntax is fairly simplistic and we are stealing the perl lsp.
this is still in early devlopment so any bugs caught would be very apreshated.

mainly aimed at linux however nothing is inherently linux based in the compiler itself.
the build system just assumes bash instead of powershell. it should  be easy to fix if needed.
if there is demand I would open a windows brench

# usage

as of now there are 2 major tools:
1. tape_tool: used to mainpulate .tape files 
2. run_turing: runs a turing machine described in .t on a .tape file and writes the output to anoter .tape file
3. tmc0: not yet functional would compile a .t file into a binary when its done

# benchmarking
from the tests I ran on the interpeter it apears that not keeping track of the counter has a small yet semi consistent advantage.
the advantage could very well be caused by the need to parse another argument or something else that is as silly.
for most purposes it would probably not matter. the reason I am not using it is mainly because it simplifies some of the logic for the compiler.

# notes for devs
the make test system deletes the compiled files. since everything compiles under a second its not an issue. 
if this is not desirble test.py would work by itself. 

# general plan
the idea is to wrap all of the syscalls into io.o and have that be a stable abi on all platforms.
then we will generate intel asembly files that would need to be assembeled with nasm (I debated using opcodes but having it this way lets users read the assembly)
linking is still up in the air I think for now I will use gcc or ld but this could change.

# TODO 
1. fix major bugs in O0
2. unify the cli tools
3. rework the low level api in io.h

## parser
add better error handeling with a dedicated struct.

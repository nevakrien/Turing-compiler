# Turing-compiler
an optimizing compiler to a binary turing machine

mainly aimed at linux however nothing is inherently linux based in the compiler itself.
the build system just assumes bash instead of powershell. it should  be easy to fix if needed.
if there is demand I would open a windows brench

# benchmarking
from the tests I ran on the interpeter it apears that not keeping track of the counter has a small yet semi consistent advantage.
the advantage could very well be caused by the need to parse another argument or something else that is as silly.
for most purposes it would probably not matter. the reason I am not using it is mainly because it simplifies some of the logic for the compiler.

# notes for devs
the make test system deletes the compiled files. since everything compiles under a second its not an issue. 
if this is not desirble test.py would work by itself. 

# TODO 
1. unify the cli tools

## parser
add better error handeling with a dedicated struct.

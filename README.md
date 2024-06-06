# Turing-compiler
an optimizing compiler to a binary turing machine

# notes for devs
the make test system deletes the compiled files. since everything compiles under a second its not an issue. 
if this is not desirble test.py would work by itself. 

# TODO 
## major bugs
inital ir does not treat the name hault as unique

also there is an unexplained error on valid.t

## parser
add better error handeling with a dedicated struct.
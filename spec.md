# pasrsing

there are 3 types of values:
bits {1,0}
directions {R,S,L}
state identifier {any other word}

every non empty line is a single transition. it contains "Read : Write"
where ":" is the key seperator between the incoming state and the outgoing instructions.
every non empty line must contain exacly 1 ":"

the read part consists of:
1. state identifier.
2. read bit.
in any order

the write part consists of:
1. write bit
2. a direction
3. next state identifier
in any order

the state identifier "halt" may NOT be an incoming state.
every state must implement read=0 and read=1 exacly once (other than halt.)

the first state mentioned in the file (on the left.) is the start state.

"#" tokens are treated as comments similar to python
tabs and all of ;,"'()][{} would be treated as white space


## tape files
io.c is the defacto implementation for reading and writing tape files.
for the vas majority of use cases it should work as is. however there is still a full spec for those intrested.

a tape file contains a data header of the form 

```c
typedef struct  __attribute__((packed)) {
    int cur_index;
    int left_index;
    int right_index;

    int left_limit;
    int right_limit;
} MetaData;
```
followed by the bits of [left_index,right_index] 
the pading to byte length is done to the right. 

# formal definition
a turing machine is a program that takes in a tape file and outputs a tape file.
it may also:

1. run for ever or return a TIME_OUT, note that if the machine WOULD run forever with infinite tape TIME_OUT is still alowed and even prefered.
2. go out of the tapes bounds, in this case an out of tape error is returned (optionally a segfault or any OS error is also accptble for O3)

## excution
starting from the start state

1. decide which Write state to use based on the current bit
2. write the write bit 
3. move in the move direction
4. transition to next state (if halt exit sucessfuly)


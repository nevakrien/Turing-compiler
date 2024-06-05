#include "parser.h"
#include "turing.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// Function to read an entire file into memory
char *read_file_into_buffer(const char *filename, size_t *length) {
    FILE *file = fopen(filename, "rb");  // Open the file in binary mode
    if (file == NULL) {
        perror("Error opening file");
        return NULL;
    }

    // Seek to the end of the file to determine the file size
    fseek(file, 0, SEEK_END);
    *length = ftell(file);
    fseek(file, 0, SEEK_SET);  // Go back to the start of the file

    // Allocate memory for the file content
    char *buffer = null_check(malloc(*length));
    
    // Read the file into the buffer
    size_t read_size = fread(buffer, 1, *length, file);
    if (read_size < *length) {
        if (errno) {
            perror("File read error");
            free(buffer);
            fclose(file);
            return NULL;
        }
    }

    // Check if the file is already null-terminated
    if (buffer[read_size - 1] != '\0') {
        buffer = null_check(realloc(buffer, read_size + 1));
        buffer[read_size] = '\0'; // Manually null terminate if not already
    }

    fclose(file);  // Close the file
    *length = read_size; // Update length to actual bytes read
    return buffer;
}

// Function to print the details of a Transition with specified indentation
void print_trans(TuringMachine machine,Transition trans, int indent) {
    const char *dir_str;
    switch (trans.move) {
        case Right:
            dir_str = "R";
            break;
        case Left:
            dir_str = "L";
            break;
        case Stay:
            dir_str = "S";
            break;
        default:
            dir_str = "?";
    }

    for (int i = 0; i < indent; i++) {
        printf(" ");
    }

    int next_state_id;
    if(trans.NextState==NULL){next_state_id=-1;}
    else{next_state_id=trans.NextState -machine.states;}

    printf("Write: %d, Move: %s, Next State: %d\n", trans.write, dir_str, next_state_id);
}

// Function to print the states and their transitions with specified indentation
void print_state(TuringMachine machine,State s, int indent) {
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }
    printf("State:\n");
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < indent; j++) {
            printf(" ");
        }
        printf("  Transition %d:\n", i + 1);
        print_trans(machine,s.transitions[i], indent + 4);
    }
}

// Function to print the entire Turing machine's states and their details with specified indentation
void print_machine(TuringMachine machine, int indent) {
    for (int i = 0; i < indent; i++) {
        printf(" ");
    }
    printf("Turing Machine Size: %d\n", machine.size);
    for (int i = 0; i < machine.size; i++) {
        print_state(machine,machine.states[i], indent + 2);
    }
}

static inline void replace_seps(char* s){
    while(*s!='\0'){
        if(*s==';'||*s==','||*s=='"'||*s=='\''||*s=='\t'||*s=='('||*s==')'||*s=='['||*s==']'||*s=='{'||*s=='}'){
            *s=' ';
        }
        s++;
    }
}

static inline char* skip_spaces(char* s){
    while(*s==' '){
        s++;
    }
    return s;
}

static inline char* find_char(char* s,char c){
    while(*s!='\0'){
        if(*s==c){
            return s;
        }
        s++;
    }
    return NULL;
}

static inline void remove_comments(char* s){
    while(*s!='\0'){
        if(*s=='#'){
            *s='\0';
            return;
        }
        s++;
    }
}

typedef struct{
    char* str;
    int num;//1 indexed
}Line;

TuringMachineEncoding parse_text_with_prints(char* raw_text){
    replace_seps(raw_text);

    int len=0;
    int cap=128;
    Line* lines=null_check(malloc(cap*sizeof(Line)));

    //gather lines
    char* s=raw_text;
    int line_num=0;
    while(*s!='\0'){
        //printf("printing s:%s",s);
        char* head=s;
        char* tail=find_char(s,'\n');
        //printf("tail is:%s",tail);
        line_num++;

        if(len==cap){
            cap*=2;
            lines=null_check(realloc(lines,cap*sizeof(Line)));
        }
        lines[len]=(Line){head,line_num};
        len++;

        if(!tail){
            break;
        }
        *tail='\0';
        s=tail+1;
        //printf("printing s:%s",s); 
    }
    
    printf("found %d lines\n",len);
    //
    for(int i=0;i<len;i++){
        remove_comments(lines[i].str);
        printf("%s\n",lines[i].str);
    }

    //rn just the checks
    //TuringMachineEncoding ans;
    int errored=0;

    for(int i=0;i<len;i++){
        Line line=lines[i];
        
        char* base=skip_spaces(line.str);
        if(*base=='\0'){
            continue;
        }

        char* left[2];
        char* right[3];

        //split
        char* sep=find_char(base,':');
        if(sep==NULL){
            printf("ERROR at line[%d]: no \" : \" seperator\n",line.num);
            errored=1;
            continue;
        }
        *sep='\0';
        right[0]=skip_spaces(sep+1);
        if(sep==find_char(right[0],':')){
            printf("ERROR at line[%d]: more than one \" : \" seperator\n",line.num);
            errored=1;
            continue;
        }

        //validate left
        left[0]=base;

        left[1]=find_char(left[0],' ');
        if(left[1]==NULL){
            printf("ERROR at line[%d]: less than 2 left args\n",line.num);
            errored=1;
            continue;
        }
        *left[1]='\0';
        left[1]=skip_spaces(left[1]+1);
        if(*left[1]=='\0'){
            printf("ERROR at line[%d]: less than 2 left args\n",line.num);
            errored=1;
            continue;
        }

        char* bad=find_char(left[1],' ');
        if(bad!=NULL){
            if(*skip_spaces(bad)!='\0'){
                printf("ERROR at line[%d]: more than 2 left args\n",line.num);
                errored=1;
                continue;
            }
            *bad='\0';
        }

        //validate right
        if(*right[0]=='\0'){
            printf("ERROR at line[%d]: no right args\n",line.num);
            errored=1;
            continue;
        }

        right[1]=find_char(right[0],' ');
        if(right[1]==NULL){
            printf("ERROR at line[%d]: only 1\\3 right args\n",line.num);
            errored=1;
            continue;
        }
        *right[1]='\0';
        right[1]=skip_spaces(right[1]+1);

        right[2]=find_char(right[1],' ');
        if(right[2]==NULL){
            printf("ERROR at line[%d]: only 2\\3 right args\n",line.num);
            errored=1;
            continue;
        }
        *right[2]='\0';
        right[2]=skip_spaces(right[2]+1);

        bad=find_char(right[2],' ');
        if(bad!=NULL){
            if(*skip_spaces(bad)!='\0'){
                printf("ERROR at line[%d]: more than 2 left args\n",line.num);
                errored=1;
                continue;
            }
            *bad='\0';
        }

        //validation over
        printf("YAY at line[%d]\n",line.num);
    }
    if(errored){
        printf("errored\n");
    }
    return (TuringMachineEncoding){0};
}
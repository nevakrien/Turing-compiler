#include "IR.h"
#include "utils.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct HashNode HashNode;
struct HashNode{
	int id;
	bool seen[2];//for both read options
	const char* key;
	HashNode* next;
};

typedef struct{
	HashNode** data;
	const char** inv;//wont be freed with the table
	int size;
	int curid;
} HashTable;

//FNV-1a
static inline uint32_t hash(const char* str)
{
    uint32_t h = 0x811c9dc5u;
    for (unsigned char* p = (unsigned char*)str; *p != '\0'; p++) {
        h = (h ^ *p) * 0x1000193u;
    }
    return h;
}

static inline HashNode* get_id(HashTable* table,const char* key,int add){
	HashNode** entry=&(table->data[hash(key)%table->size]);
	while(*entry!=NULL){
		if(strcmp((*entry)->key,key)== 0){
			return (*entry);
		}
		entry=&((*entry)->next);
	}
	if(!add){
		return NULL;
	}
	*entry=null_check(malloc(sizeof(HashNode)));
	**entry=(HashNode){table->curid,{0},key,NULL};
	table->inv[table->curid]=key;
	table->curid++;
	return (*entry);
}

static inline void free_core_table(HashTable table){
	for(int i=0;i<table.size;i++){
		while(table.data[i]!=NULL){
			HashNode* temp=table.data[i]->next;
			free(table.data[i]);
			table.data[i]=temp;
		}
	}
	free(table.data);
}

static inline uint32_t warn_missing(HashTable table){
	uint32_t warned=0;

	for(int i=0;i<table.size;i++){
		HashNode* tail=table.data[i];
		while(tail!=NULL){
			for(int k=0;k<2;k++){
				//printf("DEBUG: Symbol [%s], input (%d), seen status: %d\n", tail->key, k, (tail->seen)[k]);
				if(tail->seen[k]==false){
					printf("ERROR: no definition for symbol [%s] on input (%d) \n",tail->key,k);
					warned++;
				}
			}
			tail=tail->next;
		}
	}
	return warned;
}

TuringIR make_initial_ir (TuringMachineEncoding encoding){
	if(encoding.trans==NULL){
		UNREACHABLE();
	}
	
	HashTable table;
	table.size=encoding.len;
	table.data=null_check(calloc(table.size,sizeof(HashNode*)));
	table.inv=null_check(malloc(table.size*sizeof(char*)));
	table.curid=0;


	for(int i=0;i<encoding.len;i++){
		TransitionEncoding trans=encoding.trans[i];
		HashNode* idNode=get_id(&table,trans.NameStr,1);
		if(idNode->seen[trans.read]){
			printf("ERROR: more than one definition for symbol [%s] on input (%d) \n",trans.NameStr,trans.read);
		exit_error:
			free(table.inv);
			free_core_table(table);
			return (TuringIR){0};
		}
		idNode->seen[trans.read]=true;
	}

	if(warn_missing(table)!=0){
		goto exit_error;
	}
	
	//we have each state with exacly 2 transitions
	if(table.curid!=encoding.len/2||encoding.len%2==1){
		UNREACHABLE();
	}

	table.inv=null_check(realloc(table.inv,table.curid*sizeof(char*)));
	
	TuringIR ans;
	ans.names=table.inv;
	ans.states=null_check(malloc(table.curid*sizeof(StateIR)));
	ans.len=table.curid;

	for(int i=0;i<encoding.len;i++){
		TransitionEncoding trans=encoding.trans[i];
		HashNode* outNode=get_id(&table,trans.NextStateStr,0);
		
		int outid;
		if(outNode==NULL){
			if(strcmp(trans.NextStateStr,"halt")==0){
				outid=-1;

			}
			else{
				printf("ERROR: no definition for symbol [%s]\n",trans.NextStateStr);
				free(ans.states);
				goto exit_error;
			}	
		}
		else{
			outid=outNode->id;
		}

		HashNode* inNode=get_id(&table,trans.NameStr,0);
		if(inNode==NULL){
			UNREACHABLE();//we already inserted it earlier
		}
		ans.states[inNode->id].stateId=inNode->id;
		ans.states[inNode->id].trans[trans.read]=(TransIR){trans.write,trans.move,outid};

	}

	free_core_table(table);
	return ans;
}




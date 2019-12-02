#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "manipulate_file.c"

// HASH-FUNCTION
unsigned int hash_function(const char *str, unsigned int s) {
    unsigned int h;
    for(h = 0u;*str != '\0';str++)
        h = 157u * h + (0xFFu & (unsigned int)*str); // arithmetic overflow may occur here (just ignore it!)
    return h % s; // due to the unsigned int data type, it is guaranteed that 0 <= h % s < s
}

// STRUCT THAT DEFINES THE STATS FOR THE WORDS IN THE HASH-TABLE
typedef struct word_stats {
    int number_occurrences;
    int first_appearence;
    int last_appearence;
    int s_distance; // smallest distance
    int l_distance; // largest distance
    int m_distance; //. medium distance
    int t_distance; // total distance
} word_stats;

// HASH-TABLE
typedef struct hash_data {
    struct hash_data *next; 
    char key[20];
    struct word_stats *word;
} hash_data;

// ALLOCATE NEW HASH-DATA
hash_data *new_hash_data(void) {
    hash_data *hd = (hash_data *)malloc(sizeof(hash_data)); 
    if(hd == NULL) {
        fprintf(stderr,"Out of memory\n");
        exit(1); 
    }
    return hd; 
}


#define hash_size 20000u

int main(int argc,char **argv){
    hash_data *hash_table[hash_size];
    file_data_t *fd=NULL;
    int index;
    int i=0;
    bool flag=false;
    word_stats *ws;
    open_text_file("AED.txt", fd); 
    while(read_word(fd) == 0) {
        index = hash_function(fd->word, hash_size);
        printf("%s",fd->word);
        if(hash_table[index]->key!=NULL) { // Verifica se o indice da Hash-Table é vazio
            while(hash_table[index]->next!=NULL){
                if(strcomp(hash_table[index]->key, fd->word)){
                    ws=hash_table[index]->word;
                    ws->number_occurrences+=1;
                    if(ws->number_occurrences==2){
                        ws->s_distance=i - ws->first_appearence;
                        ws->l_distance=i - ws->first_appearence;
                        ws->t_distance+=ws->s_distance;
                    } else{
                        if(ws->s_distance > i-ws->last_appearence)
                            ws->s_distance=i-ws->last_appearence;
                        if(ws->l_distance < i-ws->last_appearence)
                            ws->l_distance=i-ws->last_appearence;
                        ws->t_distance+=i-ws->last_appearence;
                    }
                    ws->m_distance=ws->t_distance/ws->number_occurrences;
                    ws->last_appearence=i;
                    flag=true;
                    break;
                }
                hash_table[index]=hash_table[index]->next;
            }
            if(!flag){
                hash_data *hd=new_hash_data();

                hash_table[index]->next=hd;
                hd->next=NULL;
                srtcpy(hd->key, fd->word);
                hd->word->number_occurrences=1;
                hd->word->first_appearence=i;
                hd->word->last_appearence=i;

            }
        } else { // Se o index da Hash-Table nao tiver nenhum linked-list
            strcpy(hash_table[index]->key, fd->word);
            ws->number_occurrences=1;
            ws->first_appearence=i;
            ws->last_appearence=i;
        }
        flag=false;
        i+=1;
        printf("hash: %s", hash_table[index]->key);
    }

    return 1;
}
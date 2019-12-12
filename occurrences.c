#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "manipulate_file.h"

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
    char key[64];
    struct word_stats word;
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
    file_data_t fd;
    //fd->fp=NULL;
    int index;
    int j=0;
    int i=0;
    for(int k = 0;k < hash_size;k++) hash_table[k] = NULL;
    open_text_file("SherlockHolmes.txt", &fd); 
    while(read_word(&fd) != -1) {
        index = hash_function(fd.word, hash_size);
        hash_data *hd;
        for(hd = hash_table[index];hd != NULL && strcmp(fd.word,hd->key) != 0;hd = hd->next)i++;
        if(hd == NULL ){
            if(j==0){
                hd=new_hash_data();
                strcpy(hd->key, fd.word);
                hd->word.number_occurrences=1;
                hd->word.first_appearence=i;
                hd->word.last_appearence=i;
                hash_table[index]=hd;
                
            }
            else{
                hash_data *hd=new_hash_data();
                hd->next=NULL;
                strcpy(hd->key, fd.word);
                hd->word.number_occurrences=1;
                hd->word.first_appearence=i;
                hd->word.last_appearence=i;
            }
        }
        else {
            hd->word.number_occurrences+=1;
            if(hd->word.number_occurrences==2){
                hd->word.s_distance=i - hd->word.first_appearence;
                hd->word.l_distance=i - hd->word.first_appearence;
                hd->word.t_distance+=hd->word.s_distance;
            } else{
                if(hd->word.s_distance > i-hd->word.last_appearence)
                    hd->word.s_distance=i-hd->word.last_appearence;
                if(hd->word.l_distance < i-hd->word.last_appearence)
                    hd->word.l_distance=i-hd->word.last_appearence;
                hd->word.t_distance+=i-hd->word.last_appearence;
            }
            hd->word.m_distance=hd->word.t_distance/hd->word.number_occurrences;
            hd->word.last_appearence=i;
        }
        j=0;
        i++;
        printf("hash: %s %d \n", hash_table[index]->key,hash_table[index]->word.number_occurrences);
    }
    close_text_file(&fd);
    return 1;
}
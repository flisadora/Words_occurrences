#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// FILE STRUCT
typedef struct file_data {
    // public data
    long word_pos;  // zero-based 
    long word_num;  // zero-based 
    char word[64]; 
    // private data 
    FILE *fp;
    long current_pos; // zero-based 
} file_data_t;

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



// OPEN FILE
int open_text_file(char *file_name,file_data_t *fd) {
    fd->fp = fopen(file_name,"r");
    if(fd->fp == NULL)
        return -1;
    fd->word_pos = -1;
    fd->word_num = -1;
    fd->word[0] = '\0';
    fd->current_pos = -1;
    return 0;
}

// CLOSE FILE
void close_text_file(file_data_t *fd) {
    fclose(fd->fp);
    fd->fp = NULL; 
}

// READ WORD
int read_word(file_data_t *fd) {
    int i,c;
  // skip white spaces
    do {
        c = fgetc(fd->fp);
        if(c == EOF)
            return -1; 
        fd->current_pos++;
    } while(!((c >= 48 && c<58) || (c>=65 && c<=90) || (c>=97 && c<=122) || (c>=192)));
    // record word
    fd->word_pos = fd->current_pos;
    fd->word_num++;
    fd->word[0] = (char)c;
    for(i = 1;i < (int)sizeof(fd->word) - 1;i++) {
        c = fgetc(fd->fp);
        if(c == EOF)
            break; // end of file 
        fd->current_pos++;
        if(!((c >= 48 && c<58) || (c>=65 && c<=90) || (c>=97 && c<=122) || (c>=192)))
            break; // terminate word 
        fd->word[i] = (char)c;
    }
    fd->word[i] = '\0'; 
    for(int j = 0; fd->word[j]; j++){
        fd->word[j] = tolower(fd->word[j]);
    }
    return 0;
}

//WORD STATS
void word_sts(file_data_t *fd, hash_data *hd, hash_data *hi, int i){
    if(hd == NULL ){
            if(i==0){
                hd=new_hash_data();
                strcpy(hd->key, fd->word);
                hd->word.number_occurrences=1;
                hd->word.first_appearence=i;
                hd->word.last_appearence=i;
                hi=hd;
                
            }
            else{
                hash_data *hd=new_hash_data();
                hd->next=NULL;
                strcpy(hd->key, fd->word);
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
}
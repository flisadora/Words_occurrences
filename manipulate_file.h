#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

// OPEN FILE
int open_text_file(char *file_name,file_data_t *fd) {
    fd->fp = fopen(file_name,"rb");
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
    } while(c <= 32);
    // record word
    fd->word_pos = fd->current_pos;
    fd->word_num++;
    fd->word[0] = (char)c;
    for(i = 1;i < (int)sizeof(fd->word) - 1;i++) {
        c = fgetc(fd->fp);
        if(c == EOF)
            break; // end of file 
        fd->current_pos++;
        if(c <= 32)
            break; // terminate word 
        fd->word[i] = (char)c;
    }
    fd->word[i] = '\0'; 
    return 0;
}
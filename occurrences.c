#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "manipulate_file.h"

int main(int argc,char **argv){
    hash_data *hash_table[hash_size];
    file_data_t fd;
    //fd->fp=NULL;
    int idx;
    int j=0;
    int i=0;
    for(int k = 0;k < hash_size;k++) hash_table[k] = NULL;
    open_text_file("SherlockHolmes.txt", &fd); 
    while(read_word(&fd) != -1) {
        idx = hash_function(fd.word, hash_size);
        hash_data *hd;
        for(hd = hash_table[idx];hd != NULL && strcmp(fd.word,hd->key) != 0;hd = hd->next)
            i++;
        word_sts(&fd, hd, hash_table[idx], i);
        i=0;
        j++;
        printf("hash: %s %d \n", hash_table[idx]->key,hash_table[idx]->word.number_occurrences);
    }
    close_text_file(&fd);
    return 1;
}
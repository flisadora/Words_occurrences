#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "manipulate_file.h"

int main(int argc,char **argv){
    file_data_t fd;
    //fd->fp=NULL;
    int idx;
    int j=0;
    int count=0;
    int i=0;
    struct hash_data **hash_table=  malloc(hash_size*sizeof(struct hash_data));
    for(int k = 0;k < hash_size;k++) hash_table[k] = NULL;
    open_text_file("SherlockHolmes.txt", &fd); 
    while(read_word(&fd) != -1) {
        idx = hash_function(fd.word, hash_size);
        hash_data *hd;
        for(hd = hash_table[idx];hd != NULL && strcmp(fd.word,hd->key) != 0;hd = hd->next)
            i++;
         if(hd == NULL ){
            if(i==0){
                hd=new_hash_data();
                strcpy(hd->key, fd.word);
                hd->word.number_occurrences=1;
                hd->word.first_appearence=count;
                hd->word.last_appearence=count;
                hd->next=NULL;
                hash_table[idx]=hd;
                
            }
            else{
                hd=new_hash_data();
                hd->next=hash_table[idx];
                strcpy(hd->key, fd.word);
                hd->word.number_occurrences=1;
                hd->word.first_appearence=count;
                hd->word.last_appearence=count;
                hash_table[idx]=hd;
            }
            if (j>hash_size*5){

                hash_table=hash_resize(hash_table,1000u);
            }
            j++;
        }
        else {
            hd->word.number_occurrences+=1;
            if(hd->word.number_occurrences==2){
                hd->word.s_distance=count - hd->word.first_appearence;
                hd->word.l_distance=count - hd->word.first_appearence;
                hd->word.t_distance+=hd->word.s_distance;
            } else{
                if(hd->word.s_distance > count-hd->word.last_appearence)
                    hd->word.s_distance=count-hd->word.last_appearence;
                if(hd->word.l_distance < count-hd->word.last_appearence)
                    hd->word.l_distance=count-hd->word.last_appearence;
                hd->word.t_distance+=count-hd->word.last_appearence;
            }
            hd->word.m_distance=hd->word.t_distance/hd->word.number_occurrences;
            hd->word.last_appearence=count;
        }
        i=0;
        count++;
        printf("hash: %s %d \n", hash_table[idx]->key,hash_table[idx]->word.number_occurrences);
        if(hash_table[idx]->next!=NULL){
            printf("%s\n" , hash_table[idx]->next->key);
        }
    }
    close_text_file(&fd);
    return 1;
}
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
         if(hd == NULL ){
            if(i==0){
                if (j>hash_size*5){
                    printf("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA");
                    hash_data *hd_copy;
                    hash_data *hash_table_copy[hash_size];
                    *hash_table_copy=*hash_table;
                    hash_size= hash_size+100u;
                    int new_idx;
                    hash_data *hash_table[hash_size];
                    for(int m = 0;m < hash_size;m++) hash_table[m] = NULL;
                    for(int l=0;l<hash_size-100u;l++){
                        hd_copy=hash_table_copy[l];
                        while(hd_copy!=NULL){
                            new_idx= hash_function(hd_copy->key, hash_size);
                            hash_data *hc;
                            for(hc=hash_table[new_idx];hc != NULL ;hc = hc->next){}
                            hc=hd_copy;
                            hc->next==NULL;
                            hd_copy=hd_copy->next;
                        }
                    }

                }
                hd=new_hash_data();
                strcpy(hd->key, fd.word);
                hd->word.number_occurrences=1;
                hd->word.first_appearence=i;
                hd->word.last_appearence=i;
                hash_table[idx]=hd;
                j++;
                
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
        i=0;
        printf("hash: %s %d \n", hash_table[idx]->key,hash_table[idx]->word.number_occurrences);
    }
    close_text_file(&fd);
    return 1;
}
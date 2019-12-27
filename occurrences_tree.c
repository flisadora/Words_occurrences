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
    int i=0;
    struct hash_data_bt **hash_table=  malloc(hash_size*sizeof(struct hash_data));
    for(int k = 0;k < hash_size;k++) hash_table[k] = NULL;
    open_text_file("SherlockHolmes.txt", &fd); 
    while(read_word(&fd) != -1) {
        idx = hash_function(fd.word, hash_size);
        hash_data_bt *hd=hash_table[idx];
        while (hd != NULL && strcmp(fd.word,hd->key) != 0)
        {
            if(strcmp(fd.word,hd->key)<0 ){
                hd=hd->left;
            }
            else
            {
                hd=hd->right;
            }
            
            i++;
        }
            
        if(hd == NULL ){
            if(i==0){
                hd=new_hash_data_bt();
                strcpy(hd->key, fd.word);
                hd->word.number_occurrences=1;
                hd->word.first_appearence=i;
                hd->word.last_appearence=i;
                hd->right=NULL;
                hd->left=NULL;
                hash_table[idx]=hd;
                
            }
            else{
                hash_data_bt *parent=NULL;
                hash_data_bt *hb=hash_table[idx];
                hd=new_hash_data_bt();
                while (hb != NULL )
                {
                    parent=hb;
                    if(strcmp(fd.word,hb->key)<0 ){
                        hb=hb->left;
                    }
                    else
                    {
                        hb=hb->right;
                    }
                }
                
                strcpy(hd->key, fd.word);
                hd->word.number_occurrences=1;
                hd->word.first_appearence=i;
                hd->word.last_appearence=i;
                hash_table[idx]=hd;
            }
            /*
            if (j>hash_size*5){

                hash_table=hash_resize(hash_table,1000u);
                hash_size+=1000u;
            }
            */
            j++;
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
        if(hash_table[idx]->next!=NULL){
            printf("%s\n" , hash_table[idx]->next->key);
        }
    }
    close_text_file(&fd);
    return 1;
}
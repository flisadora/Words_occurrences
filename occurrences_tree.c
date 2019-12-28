#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h> 
#include "manipulate_file.h"
#include <time.h>

static double elapsed_time(void)
{
  static struct timespec last_time,current_time;

  last_time = current_time;
  if(clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&current_time) != 0)
    return -1.0; // clock_gettime() failed!!!
  return            ((double)current_time.tv_sec - (double)last_time.tv_sec)
         + 1.0e-9 * ((double)current_time.tv_nsec - (double)last_time.tv_nsec);
}


int main(int argc,char **argv){
    file_data_t fd;
    //fd->fp=NULL;
    int idx;
    int j=0;
    int i=0;
    int count=0;
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
                hd->word.first_appearence=count;
                hd->word.last_appearence=count;
                hd->right=NULL;
                hd->left=NULL;
                hash_table[idx]=hd;
                
            }
            else{
                hd=new_hash_data_bt();
                hd->word.number_occurrences=1;
                hd->word.first_appearence=count;
                hd->word.last_appearence=count;
                hd->right=NULL;
                hd->left=NULL;
                strcpy(hd->key, fd.word);
                insert_bt(hash_table[idx],hd);
            }
            if (j>hash_size*5){

                hash_table=hash_resize_bt(hash_table,1000u);
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
        if(hash_table[idx]->left!=NULL){
            printf("%s\n" , hash_table[idx]->left->key);
        }
    }
    close_text_file(&fd);
    return 1;
}
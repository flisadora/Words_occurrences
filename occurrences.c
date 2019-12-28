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
    //FILE *out= fopen("outLinked_inc.txt", "w");
    FILE *out= fopen("large_distance.txt", "w");
    for(int x=1;x<argc;x++){
        //printf("%d\n",(unsigned int)atoi(argv[x]));
        file_data_t fd;
        int idx;
        int j=0;
        int count=0;
        int i=0;
        int r_count=0;
        (void)elapsed_time();
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

                    hash_table=hash_resize(hash_table,(unsigned int)atoi(argv[x]));
                    r_count++;
                }
                j++;
            }
            else {
                hd->word.number_occurrences+=1;
                if(hd->word.number_occurrences==2){
                    hd->word.s_distance=count - hd->word.first_appearence;
                    hd->word.l_distance=count - hd->word.first_appearence;
                    hd->word.t_distance=hd->word.s_distance;
                } else{
                    if(hd->word.s_distance > count-hd->word.last_appearence)
                        hd->word.s_distance=count-hd->word.last_appearence;
                    if(hd->word.l_distance < count-hd->word.last_appearence)
                        hd->word.l_distance=count-hd->word.last_appearence;
                    hd->word.t_distance+=count-hd->word.last_appearence;
                }
                hd->word.m_distance=hd->word.t_distance/(hd->word.number_occurrences -1);
                hd->word.last_appearence=count;
            }
            i=0;
            count++;
            //printf("hash: %s %d \n", hash_table[idx]->key,hash_table[idx]->word.number_occurrences);
            //if(hash_table[idx]->next!=NULL){
            //    printf("%s\n" , hash_table[idx]->next->key);
            //}
        }
        double cpu_time = elapsed_time();
        close_text_file(&fd);
        //fprintf(out, "%d %.10f %d\n",(unsigned int)atoi(argv[x]), cpu_time,r_count);
        for(int l=0;l<hash_size;l++){
            while(hash_table[l]!=NULL){
                fprintf(out,"%d\n",hash_table[l]->word.l_distance);
                hash_table[l]=hash_table[l]->next;
            }
            
        }
        
        free(hash_table);
        hash_size=2000u;
    }
    fclose(out);
    return 1;
}
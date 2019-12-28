#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

// FILE STRUCT
typedef struct file_data_t {
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

// HASH-TABLE NODE LINKED LIST
typedef struct hash_data {
    struct hash_data *next; 
    char key[64];
    struct word_stats word;
} hash_data;

// HASH-TABLE NODE BINARY TREE
typedef struct hash_data_bt {
    struct hash_data_bt *left; 
    struct hash_data_bt *right;
    char key[64];
    struct word_stats word;
} hash_data_bt;

// ALLOCATE NEW HASH-DATA
hash_data *new_hash_data(void) {
    hash_data *hd = (hash_data *)malloc(sizeof(hash_data)); 
    if(hd == NULL) {
        fprintf(stderr,"Out of memory\n");
        exit(1); 
    }
    return hd; 
}
// ALLOCATE NEW HASH-DATA
hash_data_bt *new_hash_data_bt(void) {
    hash_data_bt *hd = (hash_data_bt *)malloc(sizeof(hash_data_bt)); 
    if(hd == NULL) {
        fprintf(stderr,"Out of memory\n");
        exit(1); 
    }
    return hd; 
}

unsigned int hash_size= 2000u;

// INSERT
void insert_bt(hash_data_bt *root, hash_data_bt *hd ){
    if(strcmp(hd->key,root->key)<0)
	{
		if(root->left!=NULL)
			insert_bt(root->left,hd);
		else
			root->left=hd;
	}
	
	if(strcmp(hd->key,root->key)>0)
	{
		if(root->right!=NULL)
			insert_bt(root->right,hd);
		else
			root->right=hd;
	}
}

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


//RESIZE LINKED LIST
 struct hash_data ** hash_resize(struct hash_data **hash_table, unsigned int inc){
    //printf("Resizing............................................................................................\n");
    hash_data *next;
    hash_size+=inc;
    struct hash_data **hash_table_new=  malloc((hash_size)*sizeof(struct hash_data));
    int new_idx;
    for(int m = 0;m < hash_size;m++) hash_table_new[m] = NULL;
    for(int l=0;l<hash_size-inc;l++){
        while(hash_table[l]!=NULL){
            new_idx= hash_function(hash_table[l]->key, hash_size);
            next=hash_table[l]->next;
            hash_table[l]->next=hash_table_new[new_idx];
            hash_table_new[new_idx]=hash_table[l];
            hash_table[l]=next;
        }
        
    }
    free(hash_table);
    return hash_table_new;
}
//TRAVERSE_TREE
void traverse_tree(struct hash_data_bt *hb, struct hash_data_bt **hash_table){
    if(hb!=NULL){
        int new_idx= hash_function(hb->key, hash_size);
        if(hash_table[new_idx]==NULL){
            hash_table[new_idx]=new_hash_data_bt();
            strcpy(hash_table[new_idx]->key,hb->key);
            hash_table[new_idx]->left=NULL;
            hash_table[new_idx]->right=NULL;
            hash_table[new_idx]->word=hb->word;
        }
        else{
            hash_data_bt *temp =new_hash_data_bt();
            temp->left=NULL;
            temp->right=NULL;
            temp->word=hb->word;
            strcpy(temp->key,hb->key);
            insert_bt(hash_table[new_idx],temp);
        }
        traverse_tree(hb->left, hash_table);
        traverse_tree(hb->right, hash_table);
    }
}
//RESIZE BINARY TREE
 struct hash_data_bt ** hash_resize_bt(struct hash_data_bt **hash_table, unsigned int inc){
    //printf("Resizing............................................................................................\n");
    hash_data_bt *next;
    struct hash_data_bt **hash_table_new=  malloc((hash_size+inc)*sizeof(struct hash_data_bt));
    int new_idx;
    hash_size+=inc;
    for(int m = 0;m < hash_size;m++) hash_table_new[m] = NULL;
    for(int l=0;l<hash_size-inc;l++){
        traverse_tree(hash_table[l],hash_table_new);        
    }
    free(hash_table);
    return hash_table_new;
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



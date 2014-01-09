#include <stdio.h>
#include <stdlib.h>
#define MAX_NUM 65535

struct bheap_node
{
    struct bheap_node* p;
    int key;//primary key
    int value;//addition value
    int degree;
    struct bheap_node* sibling;
    struct bheap_node* child;
};
typedef struct bheap_node BHNODE;

struct bheap
{
    BHNODE* head;
};

typedef struct bheap BH;

BH* bheap_create();
void bheap_free(BH*);
void bheap_node_free(BHNODE*);
BHNODE* bheap_get_min(BH* );
void bheap_link(BHNODE *,BHNODE *);
BH* bheap_union(BH *,BH *);
BH* bheap_merge(BH*, BH*);
BH* bheap_insert(BH *,BHNODE *);
BH* bheap_extract_min(BH*,int*,int*);
int bheap_decrease(BH *, BHNODE *, int);
int bheap_delete(BH *, BHNODE *);
int bheap_is_empty(BH*);
void bheap_print(BH *root);
void bheap_node_print(BHNODE* p);

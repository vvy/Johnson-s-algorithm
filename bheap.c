/*
 * =====================================================================================
 *       Filename:  bheap.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2013年12月19日 18时43分11秒
 *         Author:  wuyue (wy), vvuyve@gmail.com
 *        Company:  UESTC
 * =====================================================================================
 */
#include "bheap.h"
int bheap_is_empty(BH* h)
{
	if(h==NULL || h->head == NULL)
		return 1;
	else
		return 0;
}


BH* bheap_create()
{
    BH* bhptr = malloc(sizeof(BH));
    if(bhptr==NULL)
    {
        perror("[ERROR]bheap_create : malloc failed.\n");
        exit(-1);
    }
    bhptr->head = NULL;
    return bhptr;
}

void bheap_free(BH* h)
{
	if(h == NULL)
		return;
	BHNODE* p = h->head;
	BHNODE* p_next;
	while(p)
	{
		p_next = p->sibling;
		bheap_node_free(p);
		p=p_next;
	}
	free(h);
    h = NULL;
    return;
}

void bheap_node_free(BHNODE* p)
{
	if(p == NULL)
		return;
	BHNODE* pchild = p->child;
	BHNODE* pchildn;
	while(pchild != NULL)
	{
		pchildn = pchild->sibling;
		bheap_node_free(pchild);
		free(pchild);
		pchild = pchildn;
	}
	return;
}

BHNODE* bheap_get_min(BH* bhptr)
{
    BHNODE* y = NULL;
    BHNODE* x = bhptr->head;
    int min = MAX_NUM;
    while(x!=NULL)
    {
        if(x->key < min)
        {
            min = x->key;
            y = x;
        }
        x=x->sibling;
    }
    return y;
}

void bheap_link(BHNODE *y,BHNODE *z)
{
    y->p = z;
    y->sibling = z->child;
    z->child = y;
    z->degree++;
    return;
}

BH* bheap_union(BH *h1,BH *h2)
{
    BH *h = bheap_merge(h1,h2);
    if(h->head == NULL)
        return h;
    BHNODE* x_prev = NULL;
    BHNODE* x = h->head;
    BHNODE* x_next = x->sibling;
    while(x_next != NULL)
    {
        if(x->degree != x_next->degree || 
                    (x_next->sibling !=NULL && x_next->sibling->degree == x->degree))
        {
           x_prev = x;
           x=x_next;
        }
        else if (x->key < x_next->key)
        {
            x->sibling = x_next->sibling;
            bheap_link(x_next,x);
        }
        else {
            if(x_prev == NULL)
                h->head = x_next;
            else
                x_prev->sibling = x_next;
            bheap_link(x,x_next);
            x = x_next;
        }
        x_next = x->sibling;
    }
    return h;
}

BH* bheap_merge(BH* h1, BH* h2)
{
	int i;
	BH *h;
	BHNODE *p;
	h = bheap_create();
	if (h1 == NULL || h1->head == NULL)
		return h2;
	if (h2 == NULL || h2->head == NULL)
		return h1;
	

	BHNODE *ptrh1 = h1->head;
	BHNODE *ptrh2 = h2->head;
	while((ptrh1 != NULL)&&(ptrh2 != NULL)) {
		if (ptrh1->degree <= ptrh2->degree)	{
			if(h->head == NULL) {
				h->head = ptrh1;
				p = h->head;
			}
			else {
				p->sibling = ptrh1;
				p = ptrh1;
			}
			ptrh1 = ptrh1->sibling;
		}
		else {
			if(h->head == NULL) {
				h->head = ptrh2;
				p = h->head;
			}
			else {
				p->sibling = ptrh2;
				p = ptrh2;
			}
			ptrh2 = ptrh2->sibling;
		}
	}
	if (ptrh1!=NULL)
		p->sibling = ptrh1;
	else
		p->sibling = ptrh2;
	return h;
}

BH* bheap_insert(BH *h,BHNODE *x)
{
    BH *h_temp = bheap_create();
    x->p = NULL;
    x->child = NULL;
    x->sibling = NULL;
    x->degree = 0;
    h_temp->head = x;
    h = bheap_union(h,h_temp);
    return h;
}

BH* bheap_extract_min(BH* h,int *key,int *value)
{
	BHNODE *x,*x_prev,*y,*y_prev,*z;
	int min = MAX_NUM;
	x = h->head;
	x_prev = NULL;
	y = NULL;
	y_prev = NULL;
	while(x!=NULL) {
		if(x->key<min) {
			y_prev = x_prev;
			y = x;
			min = x->key;
		}
		x_prev = x;
		x = x->sibling;
	}

	if (y==NULL) {
		printf("bheap is empty.\n");
		return h;
	}
	//printf("min is %d.\nremoving it.\n",y->key);
    *key = y->key;
    *value = y->value;

	if(y_prev == NULL) //y是第一个
		h->head = y->sibling;
	else
		y_prev->sibling = y->sibling;//摘下y

	for(x=y;x!=NULL;x=x->child) {//y所在堆减小度数
		z=x;
		while(z!=NULL) {
			z->degree--;
			z=z->sibling;
		}
	}
	
	x=y->child;
	x_prev = NULL;
	while(x!=NULL) { //反向连接y的孩子
		z=x->sibling;
		x->sibling = x_prev;
		x_prev = x;
		x = z;
	}
	BH* temp = bheap_create();
	temp->head = x_prev;
	h = bheap_union(h,temp);
	return h;
}

int bheap_decrease(BH *root, BHNODE *x, int key)
{
	BHNODE *y,*z;
	int temp_key;
	int temp;
	if (key>x->key) {
		printf("new key is greater than current key.\n");
		return 0;
	}
	x->key = key;
	y = x;
	z = y->p;
	while((z!=NULL)&&(y->key<z->key)) {
		temp_key = y->key; //exchange the key of y and z
		y->key = z->key;
		z->key = temp_key;
		
		y=z;
		z=y->p;
	}
	return 1;
}	
		
int bheap_delete(BH *root, BHNODE *x)
{
    int key,value;
	bheap_decrease(root,x,-MAX_NUM);
	bheap_extract_min(root,&key,&value);
	return 1;
}

void bheap_print(BH *root)
{
    if(bheap_is_empty(root))
        return;
    BHNODE* p = root->head;
    bheap_node_print(p);
    printf("\n");
    return;
}

void bheap_node_print(BHNODE* p)
{
    while(p != NULL)
    {
        printf("[K:V]%d:%d  ",p->key,p->value);
        bheap_node_print(p->child);
        p=p->sibling;
    }
    return;

}

//int main() {
//	BH *root;
//	BHNODE *p,*temp;
//	int i;
//	int s[10] = {37,10,29,77,13,6,18,25,4,1};
//	root = bheap_create();
//	for(i=0;i<10;i++) {
//		p=(BHNODE*)malloc(sizeof(BHNODE));
//		p->key = s[i];
//		root = bheap_insert(root,p);
//		if(p->key == 18)
//			temp = p;
//		
//	}
//
//
//
//	//test for bheap_decrease
//	bheap_decrease(root,temp,5);
//	while(bheap_is_empty(root)) {
//		p = bheap_get_min(root);
//		if(p!=NULL)
//			printf("[main]min is %d\n",p->key);
//		root = bheap_extract_min(root);
//	
//	}
//
//}

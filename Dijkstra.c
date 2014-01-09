/*
 * =====================================================================================
 *       Filename:  Dijkstra.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2014年01月09日 00时34分40秒
 *         Author:  wuyue (wy), vvuyve@gmail.com
 *        Company:  UESTC
 * =====================================================================================
 */
#include "bheap.h" //heap is bheap
#include "graph.h"

int bheap_decrease_special(BH *root, BHNODE *x, int key,GVDis* GVDisTable);
int Dijkstra(GraphL *g,int s,GVDis* GVDisTable);

int Dijkstra(GraphL *g,int s,GVDis* GVDisTable)
{
    GVDis_init(g,s,GVDisTable);
    int key,value;
	int i;
    //build a bheap
    BH* bh = bheap_create();
    GVLNode *p;
    BHNODE *node;
    for(i=0;i<g->nV;i++)
    {
        node = malloc(sizeof(BHNODE));
        if(node == NULL)
        {
            perror("[ERROR]Dijkstra(): malloc failed.\n");
            exit(-1);
        }
        if(i==s)
            node->key = 0;
        else
            node->key = MAX_NUM;
        node->value = i;
        bh = bheap_insert(bh,node);
        GVDisTable[i].ptr = node;
    }

    //bheap_print(bh);
    while(!bheap_is_empty(bh))
    {
        //printf("[get]endpoint:%d weight:%d\n",node->value,node->key);
        bh = bheap_extract_min(bh,&key,&value);
        //printf("endpoint:%d weight:%d\n",value,key);
        p = g->adjlist + value;
        p = p->next;
        while(p!=NULL)
        {
            //bheap_print(bh);
            //printf("Relax(%d,%d):%d\n",value,p->vertex,p->weight);
            if(Relax(g,value,p->vertex,p->weight,GVDisTable)) 
            {
                //printf("decrease (%d,%d) %d to %d\n",value,p->vertex,((BHNODE*)(GVDisTable[p->vertex].ptr))->key,GVDisTable[p->vertex].distance);
                bheap_decrease_special(bh,(BHNODE*)(GVDisTable[p->vertex].ptr),GVDisTable[p->vertex].distance,GVDisTable);
                //bheap_print(bh);
            }

            p = p->next;
        }
        //printf("***\n");
        //for(i=0;i<g->nV;i++)
    	//	printf("[dijkstra]%d to %d : %d\n",s,i,GVDisTable[i].distance);
    }

    bheap_free(bh);
    return 0;
}

int bheap_decrease_special(BH *root, BHNODE *x, int key,GVDis* GVDisTable)
{
	BHNODE *y,*z;
	int temp;
    BHNODE *ptemp;
	if (key>x->key) {
		printf("new key is greater than current key.\n");
		return 0;
	}
	x->key = key;
	y = x;
	z = y->p;
	while((z!=NULL)&&(y->key<z->key)) {
		temp = y->key; //exchange the key of y and z
		y->key = z->key;
		z->key = temp;
		
		temp = y->value;
        y->value = z->value;
        z->value = temp;

        ptemp = GVDisTable[y->value].ptr;
        GVDisTable[y->value].ptr = GVDisTable[z->value].ptr;
        GVDisTable[z->value].ptr = ptemp;

		y=z;
		z=y->p;
	}
	return 1;
}	

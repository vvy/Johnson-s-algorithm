/*
 * =====================================================================================
 *       Filename:  johnson.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2014年01月08日 20时46分12秒
 *         Author:  wuyue (wy), vvuyve@gmail.com
 *        Company:  UESTC
 * =====================================================================================
 */
#include "graph.h"
int** Johnson(GraphL*);
GraphL* GetSourceGraph(GraphL*);

int** Johnson(GraphL* g)
{
    int **d;
    int i,j;
    d = malloc(sizeof(int*)* g->nV);
    if(d==NULL)
    {
        perror("[ERROR]Johnson(): malloc failed.\n");
        exit(-1);
    }
    for(i=0;i<g->nV;i++)
    {
         d[i] = malloc(sizeof(int) * g->nV);
         if(d[i]==NULL)
        {
            perror("[ERROR]Johnson(): malloc failed.\n");
            exit(-1);
        }
    }
    for(i=0;i<g->nV;i++)
        for(j=0;j<g->nV;j++)
            d[i][j] = MAX_NUM;

    GVDis* GVDisTable,*GVDisTable1;

    //compute G' by G
    GraphL* g0 = GetSourceGraph(g);

    GraphL* g1 = GraphL_Create(directed,g->nV);
    GVDisTable = malloc(sizeof(GVDis)*(g0->nV));
    GVDisTable1 = malloc(sizeof(GVDis)*(g->nV));
    if(Bellman_Ford(g0,0,GVDisTable) == 0)
    {
        perror("[ERROR]Johnson(): there is a negative cylce.\n");
        exit(-1);
    }

    int NewWeight;
    GVLNode *p;

    for(i=1;i<g0->nV;i++)
    {
        p = g0->adjlist + i;
        p = p->next;
        while(p!=NULL)
        {
            NewWeight = p->weight+GVDisTable[i].distance-GVDisTable[p->vertex].distance;
            if(NewWeight>MAX_NUM)
                NewWeight = MAX_NUM;
            GraphL_EdgeInsert(g1,i-1,p->vertex - 1,NewWeight);
            p = p->next;
        }
    }


    //GraphL_Print(g1);
    //Dijkstra on g1
    for(i=0;i<g1->nV;i++)
    {        
        Dijkstra(g1,i,GVDisTable1);
		//for(j=0;j<g1->nV;j++)
		//	printf("%d to %d:%d\n",i,j,GVDisTable1[j].distance);
        for(j=0;j<g1->nV;j++)
        {
            d[i][j] = GVDisTable1[j].distance + GVDisTable[j+1].distance - GVDisTable[i+1].distance;
            if(d[i][j] > MAX_NUM)
                d[i][j] = MAX_NUM;
        }
    }
    return d;
}

GraphL* GetSourceGraph(GraphL* g)
{
    if(g->type == undirected)
    {
        perror("[ERROR]GetSourceGraph(): graph is undirected.\n");
        exit(-1);
    }
    GraphL *g0 = GraphL_Create(directed,g->nV + 1);
    int i;
    GVLNode *p;
    for(i=1;i<g0->nV;i++)
        GraphL_EdgeInsert(g0,0,i,0);
    for(i=0;i<g->nV;i++)
    {
        p = g->adjlist + i;
        p = p->next;
        while(p != NULL)
        {
            GraphL_EdgeInsert(g0,i+1,p->vertex + 1,p->weight);
            p=p->next;
        }
    }
    return g0;
}

int main()
{
    int i,j,**d;
    GraphL *g = GraphL_Create(directed,5);
    GraphL_EdgeInsert(g,0,1,3);
    GraphL_EdgeInsert(g,0,4,-4);
    GraphL_EdgeInsert(g,0,2,8);
    GraphL_EdgeInsert(g,1,3,1);
    GraphL_EdgeInsert(g,1,4,7);
    GraphL_EdgeInsert(g,2,1,4);
    GraphL_EdgeInsert(g,3,2,-5);
    GraphL_EdgeInsert(g,3,0,2);
    GraphL_EdgeInsert(g,4,3,6);
    d = Johnson(g);
    for(i=0;i<g->nV;i++)
    {
        for(j=0;j<g->nV;j++)
            printf(" %d ",d[i][j]);
        printf("\n");
    }
    return 1;
}

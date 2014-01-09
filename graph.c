/*
 * =====================================================================================
 *       Filename:  graph.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2014年01月07日 22时25分01秒
 *         Author:  wuyue (wy), vvuyve@gmail.com
 *        Company:  UESTC
 * =====================================================================================
 */
#include "graph.h"

GraphL *GraphL_Create(enum GraphType type,unsigned int nVertex)
{
    GraphL* GLptr = malloc(sizeof(GraphL));
    if(GLptr == NULL)
    {
        perror("[ERROR]GraphL_Create : graph ptr malloc failed.\n");
        return NULL;
    }
    GLptr->type = type;
    GLptr->nV = nVertex;
    GLptr->nE = 0;
    GLptr->adjlist = malloc(sizeof(GVLNode) * nVertex);
    if(GLptr->adjlist == NULL)
    {
        perror("[ERROR]GraphL_Create : adjlist malloc failed.\n");
        return NULL;
    }
    int i;
    for(i=0;i<nVertex;i++)
    {
        (GLptr->adjlist)[i].vertex = -1;//List's start node
        (GLptr->adjlist)[i].next = NULL;
    }
    return GLptr;
}

void GraphL_EdgeInsert(GraphL* g,int start,int end,int weight)
{
    if(g == NULL)
    {
        perror("[ERROR]GraphL_EdgeInsert: pointer of the graph is null.\n");
        return;
    }

    if(start> g->nV || end>g->nV || start<0 || end < 0)
    {
        perror("[ERROR]GraphL_EdgeInsert: illegal vertex.\n");
        return;
    }

    GVLNode *p,*ptemp;
	p = g->adjlist + start;
    while(p->vertex != end && p->next != NULL)
        p = p->next;
    if(p->vertex == end)
    {
        printf("[WARNING]GraphL_EdgeInsert: the edge %d to %d already exists.\n",start,end);
        printf("reweight it instead.\n");
        p->weight = weight;
    }
    else
    {
        ptemp = malloc(sizeof(GVLNode));
        if(ptemp == NULL)
        {
            perror("[ERROR]GraphL_EdgeInsert: malloc failed.\n");
            return;
        }
        ptemp->vertex = end;
        ptemp->weight = weight;
        ptemp->next = NULL;
        p->next = ptemp;
        g->nE++;
    }

    if(g->type == undirected)
    {
        p = g->adjlist + end;
        while(p->vertex != start && p->next != NULL)
            p = p->next;
        if(p->vertex == start)
           p->weight = weight;
        else
        {
            ptemp = malloc(sizeof(GVLNode));
            if(ptemp == NULL)
            {
                perror("[ERROR]GraphL_EdgeInsert: malloc failed.\n");
                return;
            }
            ptemp->vertex = start;
            ptemp->weight = weight;
            ptemp->next = NULL;
            p->next = ptemp;
            //g->nE++;//do not add up in undirected graph
        }
    }
    return;
}

void GraphL_ReWeight(GraphL *g,int start,int end, int weight)
{
    if(start> g->nV || end>g->nV || start<0 || end < 0)
    {
        perror("[ERROR]GraphL_ReWeight: illegal vertex.\n");
        return;
    }

    GVLNode* p = g->adjlist + start;
    while(p->vertex != end && p->next != NULL)
        p = p->next;
    if(p->vertex == end)
        p->weight = weight;
    else
    {
        perror("[ERROR]GraphL_ReWeight: illegal edge.\n");
        return;
    }

    if(g->type == undirected)
    {
        p = g->adjlist + end;
        while(p->vertex != start && p->next != NULL)
            p = p->next;
        if(p->vertex == start)
            p->weight = weight;
        else
        {
            perror("[ERROR]GraphL_ReWeight: illegal edge.\n");
            return;
        }
    }
    return;
}

void GraphL_Free(GraphL *g)
{
    if(g == NULL)
    {
        perror("[ERROR]pointer of g is NULL.\n");
        return;
    }
    int i;
    GVLNode *p,*pnext;
    for(i=0;i<g->nV;i++)
    {
        p = g->adjlist + i;
        p=p->next;
        while(p!=NULL)
        {
            pnext = p->next;
            free(p);
            p = pnext;
        }
    }
    free(g->adjlist);
    free(g);
}

void GraphL_Print(GraphL *g)
{
    if(g == NULL)
    {
        printf("null pointer.\n");
        return;
    }
    printf("%s graph\n",g->type == directed?"directed":"undirected");
    printf("%d vertexs\n",g->nV);
    printf("%d edges\n",g->nE);
    printf("all edges:\n");
    int i,j;
    GVLNode *p;
    for(i=0;i<g->nV;i++)
    {
        p = g->adjlist + i;
        p = p->next;
        j = 0;
        while(p!=NULL)
        {
            if(g->type == undirected && i > p->vertex)
            {
                p = p->next;
                continue; // do not print the same undirected edge
            }
            printf("(%d to %d,weight=%d)",i,p->vertex,p->weight);
            if(j%4 == 3)
                printf("\n");
            else
                printf("\t");
            p = p->next;
            j++;
        }
        printf("\n");
    }
    return;
}

int GraphL_Weight(GraphL* g,int start,int end)
{
    if(start> g->nV || end>g->nV || start<0 || end < 0)
    {
        perror("[ERROR]GraphL_ReWeight: illegal vertex.\n");
        return;
    }
    GVLNode* p = g->adjlist + start;
    while(p->vertex != end && p->next != NULL)
        p = p->next;
    if(p->vertex == end)
        return p->weight;
    else
        return MAX_NUM;
}

void GVDis_init(GraphL* g,int s,GVDis* GVDTable)
{
    int i;
    for(i=0;i<g->nV;i++)
    {
        GVDTable[i].parent = -1;
        GVDTable[i].distance = MAX_NUM;
        GVDTable[i].ptr = NULL;
    }
    GVDTable[s].parent = s;
    GVDTable[s].distance = 0;
    return;
}

int Relax(GraphL *g,int start,int end,int w,GVDis* GVDisptr)
{
    if(g == NULL || GVDisptr == NULL)
    {
        printf("[ERROR]Relax(): null pointer.\n");
        exit(-1);
    }
    int dv,du;
    dv = (GVDisptr+end)->distance;
    du = (GVDisptr+start)->distance;
    if( dv > du + w)
    {
        (GVDisptr+end)->distance = du + w;
        (GVDisptr+end)->parent = start;
        return 1;
    }
    return 0;
}

//directed graph test
//int main()
//{
//    GraphL *g = GraphL_Create(directed,5);
//    GraphL_EdgeInsert(g,0,1,3);
//    GraphL_EdgeInsert(g,0,2,5);
//    GraphL_EdgeInsert(g,1,3,6);
//    GraphL_EdgeInsert(g,1,2,2);
//    GraphL_EdgeInsert(g,2,1,1);
//    GraphL_EdgeInsert(g,2,3,4);
//    GraphL_EdgeInsert(g,2,4,6);
//    GraphL_EdgeInsert(g,3,4,2);
//    GraphL_EdgeInsert(g,4,0,3);
//    GraphL_EdgeInsert(g,4,3,99);
//    GraphL_ReWeight(g,4,3,7);
//    GraphL_Print(g);
//    GraphL_Free(g);
//}

//undirected graph test
//int main()
//{
//    GraphL *g = GraphL_Create(undirected,5);
//    GraphL_EdgeInsert(g,0,1,1);
//    GraphL_EdgeInsert(g,1,2,1);
//    GraphL_EdgeInsert(g,0,4,1);
//    GraphL_EdgeInsert(g,1,3,1);
//    GraphL_EdgeInsert(g,1,4,1);
//    GraphL_EdgeInsert(g,2,3,1);
//    GraphL_EdgeInsert(g,3,4,1);
//    GraphL_EdgeInsert(g,3,4,2);
//    GraphL_Print(g);
//    GraphL_Free(g);
//}

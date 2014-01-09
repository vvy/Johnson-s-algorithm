/*
 * =====================================================================================
 *       Filename:  Bellman-Ford.c
 *    Description:  
 *        Version:  1.0
 *        Created:  2014年01月08日 22时08分53秒
 *         Author:  wuyue (wy), vvuyve@gmail.com
 *        Company:  UESTC
 * =====================================================================================
 */
#include "graph.h"

int Bellman_Ford(GraphL *g,int s,GVDis* GVDisTable)
{   
    int i,j;
    GVDis_init(g,s,GVDisTable);


    GVLNode *p;
    for(i=1;i <= g->nV-1;i++)
    {
        for(j=0;j<g->nV;j++)
        {
            p = g->adjlist + j;
            p = p->next;
            while(p!=NULL)
            {
                Relax(g,j,p->vertex,p->weight,GVDisTable);
                p = p->next;

            }
        }

    }

    for(j=0;j<g->nV;j++)
    {
        p = g->adjlist + j;
        p = p->next;
        while(p!=NULL)
        {
            if(GVDisTable[p->vertex].distance > GVDisTable[j].distance + p->weight)
                return 0;
            p = p->next;
        }
    }
    return 1;
}


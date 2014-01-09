#include <error.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_NUM 65535
enum GraphType {undirected,directed};//undirected/directed graph

struct GraphVertexListNode
{
    int vertex;
	int weight;
    struct GraphVertexListNode *next;
};
typedef struct GraphVertexListNode GVLNode;

struct graph_adj_list
{
    enum GraphType  type;
    unsigned int    nV;
    unsigned int    nE;
    GVLNode         *adjlist;
};
typedef struct graph_adj_list GraphL;

struct GraphVertexDistance
{
    int parent;
    int distance;
    void* ptr; //extra data
};
typedef struct GraphVertexDistance GVDis;

GraphL *GraphL_Create(enum GraphType,unsigned int);
void GraphL_EdgeInsert(GraphL*,int,int,int);
void GraphL_ReWeight(GraphL*,int,int,int);
void GraphL_Free(GraphL*);
void GraphL_Print(GraphL *);
int GraphL_Weight(GraphL*,int,int);
void GVDis_init(GraphL*,int,GVDis*);
int Relax(GraphL*,int,int,int,GVDis*);
int Bellman_Ford(GraphL *,int,GVDis*);
int Dijkstra(GraphL *,int,GVDis*);

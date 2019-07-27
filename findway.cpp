#include<stdio.h>
#include<stdlib.h>
#define Max
#define INF 32767 
#define M 4 
#define N 4 
int mg[M+2][N+2]=       //建立该全局数组提供给函数使用，后续将准备副本为复原使用 
{
	{1,1,1,1,1,1},
	{1,0,0,0,1,1},
	{1,0,1,0,0,1},
	{1,0,0,0,1,1},
	{1,1,0,0,0,1},
	{1,1,1,1,1,1},
};
typedef struct ANode
{
	int i,j;
	struct ANode *nextarc;
}ArcNode;
typedef struct Vnode
{
	ArcNode *firstarc;
}VNode;
typedef struct
{
	VNode adjlist[M+2][N+2];
}AdjGraph;
void CreatAdj(AdjGraph *&G,int Lab[M+2][N+2],int a,int b)  //如何由迷宫数组创建邻接表,难点 
{                                                          //是整个代码的核心算法 
	int i,j;ArcNode *p;
	G=(AdjGraph *)malloc(sizeof(AdjGraph));
	for(i=0;i<a;i++)
	   for(j=0;j<b;j++)
	   G->adjlist[i][j].firstarc=NULL;
	for(i=0;i<a;i++)
	   for(j=0;j<b;j++)
	   {
	     if(Lab[i][j]==0)
	   	 {
	   	   	if(Lab[i+1][j]==0)
	   	   	{
	   	       	p=(ArcNode *)malloc(sizeof(ArcNode));
	   	   	  	p->i=i+1;p->j=j;
	   	   		p->nextarc=G->adjlist[i][j].firstarc;
        	    G->adjlist[i][j].firstarc=p;
		    }
	   	   	if(Lab[i-1][j]==0)
	   	   	{
	   	       	p=(ArcNode *)malloc(sizeof(ArcNode));
	   	   	  	p->i=i-1;p->j=j;
	   	   		p->nextarc=G->adjlist[i][j].firstarc;
        	    G->adjlist[i][j].firstarc=p;
		    }
	   	   	if(Lab[i][j+1]==0)
	   	   	{
	   	       	p=(ArcNode *)malloc(sizeof(ArcNode));
	   	   	  	p->i=i;p->j=j+1;
	   	   		p->nextarc=G->adjlist[i][j].firstarc;
        	    G->adjlist[i][j].firstarc=p;
		    }
	   	   	if(Lab[i][j-1]==0)
	   	   	{
	   	       	p=(ArcNode *)malloc(sizeof(ArcNode));
	   	   	  	p->i=i;p->j=j-1;
	   	   		p->nextarc=G->adjlist[i][j].firstarc;
        	    G->adjlist[i][j].firstarc=p;
		    }
		 }
       }
}
void DestroyAdj(AdjGraph *&G,int a,int b)
{
	int i,j;ArcNode *pre, *p;
	for(i=0;i<a;i++)
	  for(j=0;j<b;j++)
	  {
	  	pre=G->adjlist[i][j].firstarc;
	  	if(pre!=NULL)
	  	{
	  		p=pre->nextarc;
	  		while(p!=NULL)
	  		{
	  			free(pre);
	  			pre=p;
	  			p=p->nextarc;
			}
			free(pre);
	    }
	  }
	  free(G);
}
void DisPlayAdj(AdjGraph *G,int a,int b)
{
	int i,j;ArcNode *p;
	for(i=0;i<a;i++)
	   for(j=0;j<b;j++)
	   {
	   	 p=G->adjlist[i][j].firstarc;
	   	 if(p!=NULL)
	   	 printf("(%d %d):",i,j);
	   	 while(p!=NULL)
	   	 {
	   	 	printf("(%d %d),",p->i,p->j);
	   	 	p=p->nextarc;
		 }
		 printf("\n");
	   }
}
int main()
{
	AdjGraph *G;
	CreatAdj(G,mg,M+2,N+2);
	DisPlayAdj(G,M+2,N+2);
	DestroyAdj(G,M+2,N+2);
}

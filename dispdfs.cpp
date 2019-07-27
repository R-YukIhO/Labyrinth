#define M 4 
#define N 4 
#include<stdio.h>
#include<stdlib.h>
int mg[M+2][N+2]=       //建立该全局数组提供给函数使用，后续将准备副本为复原使用 
{
	{1,1,1,1,1,1},
	{1,0,0,0,1,1},
	{1,0,1,0,0,1},
	{1,0,0,0,1,1},
	{1,1,0,0,0,1},
	{1,1,1,1,1,1},
};
int mgre[M+2][N+2]=       //副本
{
	{1,1,1,1,1,1},
	{1,0,0,0,1,1},
	{1,0,1,0,0,1},
	{1,0,0,0,1,1},
	{1,1,0,0,0,1},
	{1,1,1,1,1,1},
};
//---------------------------------
typedef struct
{
	int i;
	int j;
}Box;
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
{
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
//-------------------------------------------------------
void DisplayLab(int a,int b)
{
	for(int i=0;i<a;i++)
	{
	  for(int j=0;j<b;j++)
	  {
	  	if(mg[i][j]==1)
	  	printf("■"); 
		if(mg[i][j]==0) 
		printf("  "); 
		if(mg[i][j]==-1)
		printf("* ");
	  }
	  printf("\n");
    }
}
void recover(int a,int b)
{
	for(int i=0;i<a;i++)
	{
	  for(int j=0;j<b;j++)
	  {
	  	mg[i][j]=mgre[i][j];
	  }
    }
} 
int visited[M+2][N+2];
void FindaPath(AdjGraph *G,Box u,Box v,Box path[],int d)
{
	int i;ArcNode *p;Box w;
	visited[u.i][u.j]=1;
	d++;path[d].i=u.i;path[d].j=u.j;
	if(u.i==v.i&&u.j==v.j)
	{
		for(i=0;i<=d;i++)
		   mg[path[i].i][path[i].j]=-1;
		   DisplayLab(M+2,N+2);
		   recover(M+2,N+2);
	}
		p=G->adjlist[u.i][u.j].firstarc;
		while(p!=NULL)
		{
			w.i=p->i;w.j=p->j;
			if(visited[w.i][w.j]==0)
			   FindaPath(G,w,v,path,d);
			p=p->nextarc;
		}
		visited[u.i][u.j]=0;
}
//------------------------------------------------------------------
typedef struct
{
	Box data;
	int parent;
}quere;

void ShortPath(AdjGraph *G,Box u,Box v)
{
	ArcNode *p;
	int i,j;Box w;
	quere qu[100];
	int front=-1,rear=-1;
	int visited[100][100];
	for(i=0;i<100;i++)
	  for(j=0;j<100;j++)
	    visited[i][j]=0;
	rear++;
	qu[rear].data.i=u.i;qu[rear].data.j=u.j;
	qu[rear].parent=-1;
	visited[u.i][u.j]=1;
	while(front!=rear)
	{
		front++;
		w=qu[front].data;
		if(w.i==v.i&&w.j==v.j)
		{
			i=front;
			while(qu[i].parent!=-1)
			{
				mg[qu[i].data.i][qu[i].data.j]=-1;
				i=qu[i].parent;
			}
			mg[qu[i].data.i][qu[i].data.j]=-1;
			DisplayLab(M+2,N+2);
		    recover(M+2,N+2);
			return;
		}
		p=G->adjlist[w.i][w.j].firstarc;
		while(p!=NULL)
		{
			if(visited[p->i][p->j]==0)
			{
				visited[p->i][p->j]=1;
				rear++;
				qu[rear].data.i=p->i;qu[rear].data.j=p->j;
				qu[rear].parent=front;
			}
			p=p->nextarc;
		}
	}
}
//---------------------------------------------------
int main()
{
   for(int i=0;i<M+2;i++)
   for(int j=0;j<N+2;j++)
   visited[i][j]=0;	
   Box path[100];
   Box u;u.i=1;u.j=1;
   Box v;v.i=4;v.j=4;
   	AdjGraph *G;
	CreatAdj(G,mg,M+2,N+2);
	FindaPath(G,u,v,path,-1);
	ShortPath(G,u,v);
}

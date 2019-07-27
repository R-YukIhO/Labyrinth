typedef struct
{
	int i;
	int j;
}Box;
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
	int front=-1;rear=-1;
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
				printf("(%d %d)"qu[i].data.i,qu[i].data.j);
				i=qu[i].parent;
			}
			printf("(%d %d)\n"qu[i].data.i,qu[i].data.j);
			return;
		}
		p=G->adjlist[w.i][w.j].firstarc;
		while(p!=NULL)
		{
			if(visited[p->adjvex]==0)
			{
				visited[p->adjvex]=1;
				rear++;
				qu[rear].data.i=p->i;qu[rear].data.j=p->j;
				qu[rear].parent=front;
			}
			p=p->nextarc;
		}
	}
}

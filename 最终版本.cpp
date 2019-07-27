#include<string.h>
#define M 8 
#define N 8 
#include <stdio.h>            
#include <pthread.h>
#include <stdlib.h>
#include<time.h>
#include <malloc.h>
#define MaxSize 100	
#define ElemType Box
typedef struct
{
	int i;
	int j;
}Box;
//-----------------------------------------------------------------------------
//�������� 
void DisplayLab(int a,int b);
void recover(int a,int b);
//------------------------------------------------------------------------------
//˳��ջ�Ļ������� 
typedef struct
{
	Box data[MaxSize];
	int top;
}StType;		
void InitStack(StType *&s)   //��ʼ��˳��ջ
{
	s=(StType *)malloc(sizeof(StType));
	s->top=-1;
} 
void DestroyStack(StType *&s) //����˳��ջ
{
	free(s);
}
bool StackEmpty(StType *s)		//�ж�ջ�շ�
{
	return(s->top==-1);
}
bool Push(StType *&s,ElemType e)	 //��ջ
{
	if (s->top==MaxSize-1)    //ջ�����������ջ�����
		return false;
	s->top++;
	s->data[s->top]=e;
	return true;
}
bool Pop(StType *&s,ElemType &e)	 //��ջ
{
	if (s->top==-1)		//ջΪ�յ��������ջ�����
		return false;
	e=s->data[s->top];
	s->top--;
	return true;
} 
bool GetTop(StType *s,ElemType &e)	 //ȡջ��Ԫ��
{
	if (s->top==-1) 		//ջΪ�յ��������ջ�����
		return false;
	e=s->data[s->top];
	return true;
}
//----------------------------------------------------------------------------------
//�ڽӱ�Ķ���
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
typedef struct
{
	Box data;
	int parent;
}quere;
//----------------------------------------------------------------------------------
//ȫ�ֱ����Ķ��� 
int timefg=0,findfg=0,mainfg=0,fgrecover=0;  //�ĸ���Ҫ��ȫ�ֱ��� 
int visited[M+2][N+2];
Box path[100];
Box u;      //�ṹ�����͵�ȫ�ֱ������ȶ��壬�������ں������ڸ�ֵ�� 
Box v;
int mg[M+2][N+2]=       //������ȫ�������ṩ������ʹ�ã�������׼������Ϊ��ԭʹ�� 
{
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,1,1,1,1,0,0,0,1},
  {1,0,1,0,0,1,1,1,0,1},
  {1,0,0,0,0,0,0,1,0,1},
  {1,1,0,1,1,0,0,1,0,1},
  {1,0,0,0,1,0,1,1,1,1},
  {1,1,1,0,1,0,0,0,1,1},
  {1,0,1,0,0,0,1,0,0,1},
  {1,0,0,0,1,1,0,1,0,1},
  {1,1,1,1,1,1,1,1,1,1},
};
int mgre[M+2][N+2]=     //���� 
{
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,1,1,1,1,0,0,0,1},
  {1,0,1,0,0,1,1,1,0,1},
  {1,0,0,0,0,0,0,1,0,1},
  {1,1,0,1,1,0,0,1,0,1},
  {1,0,0,0,1,0,1,1,1,1},
  {1,1,1,0,1,0,0,0,1,1},
  {1,0,1,0,0,0,1,0,0,1},
  {1,0,0,0,1,1,0,1,0,1},
  {1,1,1,1,1,1,1,1,1,1},
};
//---------------------------------------------------------------------------
//ͼ�Ĳ�����������
void CreatAdj(AdjGraph *&G,int Lab[M+2][N+2],int a,int b)  //������Թ����鴴���ڽӱ�,�ѵ� 
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
		   printf("\n");
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
//---------------------------------------------------------------------------
//�Թ�������һϵ�к������� 
void recover(int a,int b)    //��ԭ�Թ� 
{
	for(int i=0;i<a;i++)
	{
	  for(int j=0;j<b;j++)
	  {
	  	mg[i][j]=mgre[i][j];
	  }
    }
} 
void recover2(int a,int b)   //�޸��Թ���·��ǽ��ǽ��· 
{
	for(int i=1;i<a;i++)
	{
	  for(int j=1;j<b;j++)
	  {
	  	if(mg[i][j]==1)
	  	mg[i][j]=0;
	    else
		if(mg[i][j]==0) 
		mg[i][j]=1; 
	  }
    }
    for(int i=1;i<a;i++)
	{
	  for(int j=1;j<b;j++)
	  {
	  	if(mgre[i][j]==1)
	  	mgre[i][j]=0;
	    else
		if(mgre[i][j]==0) 
		mgre[i][j]=1; 
	  }
    }
    if(fgrecover==0)
    fgrecover=1;
    else
    fgrecover=0;
} 
void DisplayLab(int a,int b)
{
	for(int i=0;i<a;i++)
	{	
	  for(int j=0;j<b;j++)
	  {
	  	if(mg[i][j]==1)
	  	printf("��"); 
		if(mg[i][j]==0)
		printf("  ");
		if(mg[i][j]==-1)     //��¼�߹���·�� 
		printf("* ");
		if(mg[i][j]==-2)     //��¼�߹���·�� 
		printf("& ");
		if(mg[i][j]==-3)
		printf("$ ");
	  }
	  printf("\n");
    }
}
void movemos(int xi,int yi,int xe,int ye)   
{ 
    timefg=0;   //����ʱ���ǣ��Ա�ѭ������op1
	Box path[MaxSize],e;     int fg=0;//��fg�ж������Ƿ���Ч 
	int i,j,i1,j1;            
    char di;
	bool find;
	StType *st;
	InitStack(st);
	e.i=xi; e.j=yi;
	Push(st,e);  
	while(!StackEmpty(st))
	{ 
	    if(timefg==1)   //ʱ������� ��ο��ƣ����̶Ƚ�ʡʱ��
	    break;
		GetTop(st,e);
		i=e.i; j=e.j;
		if(i==xe&&j==ye)
		{
			printf("�ɹ��߳��Թ�,�������ز˵�\n");
			findfg=1;mainfg=0;
			 break;  
		}
		else     
		{
		find=false;
		while(!find)
		{
			if(timefg==1)   //ʱ�������  ��ο��ƣ����̶Ƚ�ʡʱ�� 
	        break;
			scanf("%s",&di);
			switch(di)
			{
				case 'w': i1=i-1; j1=j;fg=0; break;//�� 
				case 'd': i1=i; j1=j+1;fg=0; break;//ǰ 
				case 's': i1=i+1; j1=j;fg=0; break;//�� 
				case 'a': i1=i; j1=j-1;fg=0; break;//��
				default : fg=1; 
			}
			if(timefg==1)   //ʱ������� ��ο��ƣ����̶Ƚ�ʡʱ��
	        break;
			if((mg[i1][j1]!=1)&&fg==0)   //��������Чʱ�ı�find��ֵ 
			find=true;
			if((find!=true)||fg==1)
			printf("��������Ч·��������������\n");
		}
		if(find)
		{
			e.i=i1; e.j=j1;
			Push(st,e);
			mg[i1][j1]=-2;     //������� 
			system("cls");     //������Ҫ������ʹ���Թ�ˢ�³��� 
			DisplayLab(M+2,N+2);
			mg[i1][j1]=-1;    //�����߹���· 
		}
		else
		{
			Pop(st,e);
			mg[e.i][e.j]=0;
		}
	    }
	}
	DestroyStack(st);
}
void *thread1(void *arg)      //��ʱ������������һ�߳��м�ʱ 
{
    int beinT=0,lastT=0,t,n=30;
    while(1)
    {
        if(beinT==0)
            beinT=t=time(NULL);
        else
            t=time(NULL);
        if(lastT!=t)
        {
            printf("\t\t\t\tʣ��%d��\r",n--);
            lastT=t;
        }
        if(t-beinT==30||findfg==1)
        {    if(findfg!=1)
            printf("\t\t\t\t��Ϸʧ�ܣ��밴��������ز˵�\n");
            timefg=1;
		    mainfg=0;
		    break;
        } 
    }
    beinT=0;lastT=0;t,n=10;
}
//---------------------------------------------------------------------------
//�������������Ķ��� 
void op1()
{   int re1,re2,ov1,ov2;   //�����޸��Թ������յ� 
    if(fgrecover==0)
    {
    	re1=3;re2=3;ov1=8;ov2=8;
        mg[8][8]=-3;
	    mg[3][3]=-2;     //������� 
	    DisplayLab(M+2,N+2);
	    mg[3][3]=-1;     //�����߹���·��
    }
    else
    {
    	re1=2;re2=2;ov1=6;ov2=8;
        mg[6][8]=-3;
	    mg[2][2]=-2;     //������� 
	    DisplayLab(M+2,N+2);
	    mg[2][2]=-1;     //�����߹���·��
	}
	  //�����̵߳�id�� 
	  pthread_t t1;   //������Ҫ��������ִ��һ�������������߳�Ҳͬʱ����
	  //����һ���߳� 
	  pthread_create(&t1,NULL,thread1,NULL);
	  pthread_detach(t1);
	  movemos(re1,re2,ov1,ov2);
  	  recover(M+2,N+2);
}
void op2()
{
	DisplayLab(M+2,N+2);
}
void op3()
{
	recover2(M+1,N+1);
	DisplayLab(M+2,N+2);
}
void op4()
{
    if(fgrecover==0)
	{
	    u.i=3;u.j=3;
	    v.i=8;v.j=8;
    }
    else
    {
    	u.i=2;u.j=2;
	    v.i=6;v.j=8;
	}         
	AdjGraph *G;
	CreatAdj(G,mg,M+2,N+2);
	FindaPath(G,u,v,path,-1);
}
void op5()
{
	if(fgrecover==0)
	{
	    u.i=3;u.j=3;
	    v.i=8;v.j=8;
    }
    else
    {
    	u.i=2;u.j=2;
	    v.i=6;v.j=8;
	}
	AdjGraph *G;
	CreatAdj(G,mg,M+2,N+2); 
	ShortPath(G,u,v);
}
//--------------------------------------------------------------------------------- 
int main()
{
	char judge; // ���ó�char�ͷ�ֹ������ĸ�����ڴ����
	int g=0;      
	char kaishi;
	while(g!=1)
	{
		if(mainfg!=1)
		{
			mainfg=1;
			printf("-------------Labyrinth Game-------------\n");
			printf("���������ֲ������ţ�\n����1�����Թ�����ʱ30s\n"); 
			printf("����2���޸��Թ���·��ǽ��ǽ��·\n");
			printf("����3����ʾ�Թ�������·��\n");
			printf("����4����ʾ�Թ������·��\n");
			printf("����5���˳���Ϸ\n") ;
			printf("----------------------------------------\n"); 
		 } 
		else
		printf("\t����������������:\n");
		scanf("%s",&judge);
		switch(judge)
	    {
		case '1':
			printf("����w s a d��ʾ�������ң�&��ʾ����*��ʾ����·��\n"); 
			printf("��ʱ30s������������ĸ��ʼ��Ϸ\n");
			scanf("%s",&kaishi);
			op1();
			findfg=0;        //�����ҵ���ǣ��Ա�ѭ������op1 
		    break;
		case '2':
			printf("�޸�ǰ�Թ����£�\n"); 
			op2(); 
			printf("�޸ĺ��Թ����£���·��ǽ��ǽ��·��\n"); 
			op3();
			break;
		case '3':
			printf("�Թ�������·������\n");
			op4();
            break;
        case '4':
        	printf("�Թ������·������\n");
			op5();
            break;
		case '5':
			printf("��Ϸ�����������˳�\n");
			g=1;
			break;
		default:
			printf("��Ϸ�����������˳�\n");
			g=1;
			break;
	    }
	}
}

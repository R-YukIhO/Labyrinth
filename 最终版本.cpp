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
//函数声明 
void DisplayLab(int a,int b);
void recover(int a,int b);
//------------------------------------------------------------------------------
//顺序栈的基本操作 
typedef struct
{
	Box data[MaxSize];
	int top;
}StType;		
void InitStack(StType *&s)   //初始化顺序栈
{
	s=(StType *)malloc(sizeof(StType));
	s->top=-1;
} 
void DestroyStack(StType *&s) //销毁顺序栈
{
	free(s);
}
bool StackEmpty(StType *s)		//判断栈空否
{
	return(s->top==-1);
}
bool Push(StType *&s,ElemType e)	 //进栈
{
	if (s->top==MaxSize-1)    //栈满的情况，即栈上溢出
		return false;
	s->top++;
	s->data[s->top]=e;
	return true;
}
bool Pop(StType *&s,ElemType &e)	 //出栈
{
	if (s->top==-1)		//栈为空的情况，即栈下溢出
		return false;
	e=s->data[s->top];
	s->top--;
	return true;
} 
bool GetTop(StType *s,ElemType &e)	 //取栈顶元素
{
	if (s->top==-1) 		//栈为空的情况，即栈下溢出
		return false;
	e=s->data[s->top];
	return true;
}
//----------------------------------------------------------------------------------
//邻接表的定义
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
//全局变量的定义 
int timefg=0,findfg=0,mainfg=0,fgrecover=0;  //四个重要的全局变量 
int visited[M+2][N+2];
Box path[100];
Box u;      //结构体类型的全局变量可先定义，单必须在函数体内赋值； 
Box v;
int mg[M+2][N+2]=       //建立该全局数组提供给函数使用，后续将准备副本为复原使用 
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
int mgre[M+2][N+2]=     //副本 
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
//图的操作函数定义
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
//迷宫操作的一系列函数定义 
void recover(int a,int b)    //复原迷宫 
{
	for(int i=0;i<a;i++)
	{
	  for(int j=0;j<b;j++)
	  {
	  	mg[i][j]=mgre[i][j];
	  }
    }
} 
void recover2(int a,int b)   //修改迷宫，路变墙，墙变路 
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
	  	printf("■"); 
		if(mg[i][j]==0)
		printf("  ");
		if(mg[i][j]==-1)     //记录走过的路径 
		printf("* ");
		if(mg[i][j]==-2)     //记录走过的路径 
		printf("& ");
		if(mg[i][j]==-3)
		printf("$ ");
	  }
	  printf("\n");
    }
}
void movemos(int xi,int yi,int xe,int ye)   
{ 
    timefg=0;   //重置时间标记，以便循环调用op1
	Box path[MaxSize],e;     int fg=0;//用fg判断输入是否有效 
	int i,j,i1,j1;            
    char di;
	bool find;
	StType *st;
	InitStack(st);
	e.i=xi; e.j=yi;
	Push(st,e);  
	while(!StackEmpty(st))
	{ 
	    if(timefg==1)   //时间控制器 多次控制，最大程度节省时间
	    break;
		GetTop(st,e);
		i=e.i; j=e.j;
		if(i==xe&&j==ye)
		{
			printf("成功走出迷宫,即将返回菜单\n");
			findfg=1;mainfg=0;
			 break;  
		}
		else     
		{
		find=false;
		while(!find)
		{
			if(timefg==1)   //时间控制器  多次控制，最大程度节省时间 
	        break;
			scanf("%s",&di);
			switch(di)
			{
				case 'w': i1=i-1; j1=j;fg=0; break;//上 
				case 'd': i1=i; j1=j+1;fg=0; break;//前 
				case 's': i1=i+1; j1=j;fg=0; break;//下 
				case 'a': i1=i; j1=j-1;fg=0; break;//后
				default : fg=1; 
			}
			if(timefg==1)   //时间控制器 多次控制，最大程度节省时间
	        break;
			if((mg[i1][j1]!=1)&&fg==0)   //当输入有效时改变find的值 
			find=true;
			if((find!=true)||fg==1)
			printf("输入了无效路径，请重新输入\n");
		}
		if(find)
		{
			e.i=i1; e.j=j1;
			Push(st,e);
			mg[i1][j1]=-2;     //变成老鼠 
			system("cls");     //至关重要，清屏使得迷宫刷新成立 
			DisplayLab(M+2,N+2);
			mg[i1][j1]=-1;    //老鼠走过的路 
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
void *thread1(void *arg)      //计时器函数，在另一线程中计时 
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
            printf("\t\t\t\t剩余%d秒\r",n--);
            lastT=t;
        }
        if(t-beinT==30||findfg==1)
        {    if(findfg!=1)
            printf("\t\t\t\t游戏失败，请按任意键返回菜单\n");
            timefg=1;
		    mainfg=0;
		    break;
        } 
    }
    beinT=0;lastT=0;t,n=10;
}
//---------------------------------------------------------------------------
//基本操作函数的定义 
void op1()
{   int re1,re2,ov1,ov2;   //方便修改迷宫起点和终点 
    if(fgrecover==0)
    {
    	re1=3;re2=3;ov1=8;ov2=8;
        mg[8][8]=-3;
	    mg[3][3]=-2;     //变成老鼠 
	    DisplayLab(M+2,N+2);
	    mg[3][3]=-1;     //老鼠走过的路径
    }
    else
    {
    	re1=2;re2=2;ov1=6;ov2=8;
        mg[6][8]=-3;
	    mg[2][2]=-2;     //变成老鼠 
	    DisplayLab(M+2,N+2);
	    mg[2][2]=-1;     //老鼠走过的路径
	}
	  //创建线程的id号 
	  pthread_t t1;   //至关重要！主函数执行一旦结束，其他线程也同时结束
	  //创建一条线程 
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
	char judge; // 设置成char型防止输入字母导致内存溢出
	int g=0;      
	char kaishi;
	while(g!=1)
	{
		if(mainfg!=1)
		{
			mainfg=1;
			printf("-------------Labyrinth Game-------------\n");
			printf("请输入数字操作代号：\n输入1：走迷宫，限时30s\n"); 
			printf("输入2：修改迷宫，路变墙，墙变路\n");
			printf("输入3：显示迷宫的所有路径\n");
			printf("输入4：显示迷宫的最短路径\n");
			printf("输入5：退出游戏\n") ;
			printf("----------------------------------------\n"); 
		 } 
		else
		printf("\t请继续输入操作代号:\n");
		scanf("%s",&judge);
		switch(judge)
	    {
		case '1':
			printf("输入w s a d表示上下左右，&表示老鼠，*表示已走路径\n"); 
			printf("限时30s，输入任意字母开始游戏\n");
			scanf("%s",&kaishi);
			op1();
			findfg=0;        //重置找到标记，以便循环调用op1 
		    break;
		case '2':
			printf("修改前迷宫如下：\n"); 
			op2(); 
			printf("修改后迷宫如下：（路变墙，墙变路）\n"); 
			op3();
			break;
		case '3':
			printf("迷宫的所有路径如下\n");
			op4();
            break;
        case '4':
        	printf("迷宫的最短路径如下\n");
			op5();
            break;
		case '5':
			printf("游戏结束，即将退出\n");
			g=1;
			break;
		default:
			printf("游戏结束，即将退出\n");
			g=1;
			break;
	    }
	}
}

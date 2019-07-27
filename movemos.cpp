//顺序栈运算算法
#include<stdlib.h>
#include <stdio.h>
#include <malloc.h>
#define MaxSize 100	
#define ElemType Box
typedef struct
{
	int i;
	int j;
}Box;
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
	  }
	  printf("\n");
    }
}
void movemos(int xi,int yi,int xe,int ye)
{
	Box path[MaxSize],e;     int fg=0;//用fg判断输入是否有效 
	int i,j,i1,j1;            
    char di;
	bool find;
	StType *st;
	InitStack(st);
	e.i=xi; e.j=yi;
	Push(st,e); 
	mg[xi][yi]=-2;     //变成老鼠 
	DisplayLab(M+2,N+2);
	mg[xi][yi]=-1;     //老鼠走过的路径 
	while(!StackEmpty(st))
	{
		GetTop(st,e);
		i=e.i; j=e.j;
		if(i==xe&&j==ye)
		{
			printf("成功走出迷宫:\n"); break;  
			printf("\n");
		//	Pop(st,e);
	//		mg[e.i][e.j]=0;
		}
		else     //修改处1 
		{
		find=false;
		while(!find)
		{
			scanf("%s",&di);
			switch(di)
			{
				case 'w': i1=i-1; j1=j;fg=0; break;//上 
				case 'd': i1=i; j1=j+1;fg=0; break;//前 
				case 's': i1=i+1; j1=j;fg=0; break;//下 
				case 'a': i1=i; j1=j-1;fg=0; break;//后
				default : fg=1; 
			}
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
			system("cls");     //关键函数，清屏使得迷宫刷新成立 
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
int main()
{
	movemos(3,3,M,N);
	   return 1;
}






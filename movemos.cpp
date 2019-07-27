//˳��ջ�����㷨
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
#define M 4 
#define N 4 
int mg[M+2][N+2]=       //������ȫ�������ṩ������ʹ�ã�������׼������Ϊ��ԭʹ�� 
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
	  	printf("��"); 
		if(mg[i][j]==0)
		printf("  ");
		if(mg[i][j]==-1)     //��¼�߹���·�� 
		printf("* ");
		if(mg[i][j]==-2)     //��¼�߹���·�� 
		printf("& ");
	  }
	  printf("\n");
    }
}
void movemos(int xi,int yi,int xe,int ye)
{
	Box path[MaxSize],e;     int fg=0;//��fg�ж������Ƿ���Ч 
	int i,j,i1,j1;            
    char di;
	bool find;
	StType *st;
	InitStack(st);
	e.i=xi; e.j=yi;
	Push(st,e); 
	mg[xi][yi]=-2;     //������� 
	DisplayLab(M+2,N+2);
	mg[xi][yi]=-1;     //�����߹���·�� 
	while(!StackEmpty(st))
	{
		GetTop(st,e);
		i=e.i; j=e.j;
		if(i==xe&&j==ye)
		{
			printf("�ɹ��߳��Թ�:\n"); break;  
			printf("\n");
		//	Pop(st,e);
	//		mg[e.i][e.j]=0;
		}
		else     //�޸Ĵ�1 
		{
		find=false;
		while(!find)
		{
			scanf("%s",&di);
			switch(di)
			{
				case 'w': i1=i-1; j1=j;fg=0; break;//�� 
				case 'd': i1=i; j1=j+1;fg=0; break;//ǰ 
				case 's': i1=i+1; j1=j;fg=0; break;//�� 
				case 'a': i1=i; j1=j-1;fg=0; break;//��
				default : fg=1; 
			}
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
			system("cls");     //�ؼ�����������ʹ���Թ�ˢ�³��� 
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
int main()
{
	movemos(3,3,M,N);
	   return 1;
}





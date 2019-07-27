#define M 10
#define N 10
#include<stdio.h> 
int Lab[M][N]=
{
  {1,1,1,1,1,1,1,1,1,1},
  {1,0,1,0,1,1,0,0,0,1},
  {1,0,1,0,0,1,0,1,0,1},
  {1,0,0,0,0,0,0,1,0,1},
  {1,1,0,1,1,1,0,1,0,1},
  {1,0,0,0,1,0,1,1,0,1},
  {1,1,1,0,1,0,0,0,1,1},
  {1,0,1,0,0,0,1,0,0,1},
  {1,0,0,0,1,1,0,1,0,1},
  {1,1,1,1,1,1,1,1,1,1},
};
void DisplayLab(int a,int b)
{
	for(int i=0;i<a;i++)
	{
	  for(int j=0;j<b;j++)
	  {
	  	if(Lab[i][j]==1)
	  	printf("¡ö"); 
		if(Lab[i][j]==0) 
		printf("  "); 
	  }
	  printf("\n");
    }
}
int main()
{
	DisplayLab(M,N);
	printf("\r6");
}

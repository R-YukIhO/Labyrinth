#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include<time.h>
//ִ���̵߳�һ������
//void *thread(void *arg);
int fg=1;
void *timelast(void *arg);
void *abc(void *arg)
{
	while(1)
	printf("hhhh");
}
int main()
{
	//�����̵߳�id�� 
	pthread_t a ; 
	int ret ; 
	//����һ���߳� 
	ret = pthread_create(&a , NULL ,   abc, NULL);
	if(ret != 0)
	{
		return -1 ; 
	}                      //���߳̽���ʱ�Զ��ͷ�id�� 
	ret = pthread_detach(a);	
//	while(fg)
	{
		printf("Hello world1!\n\n");
	}
	return 0 ; 
}
void *timelast(void *arg)
{
    int beinT=0,lastT=0,t,n=10;
    while(1)
    {
        if(beinT==0)
            beinT=t=time(NULL);
        else
            t=time(NULL);
        if(lastT!=t)
        {
         //   system("cls");
            printf("%d\n",n--);
            lastT=t;
        }
        if(t-beinT==10)
        {
			fg=0;break;
		}
    }
}

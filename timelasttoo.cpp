#include <stdio.h>           //至关重要！主函数执行一旦结束，其他线程也同时结束 
#include <pthread.h>
#include <stdlib.h>
#include<time.h>
//执行线程的一个函数
void *do_thread(void *arg);
 int fg=10;
int main(void)
{
	int a;
	//创建线程的id号 
	pthread_t    tid ; 
	int ret ; 
	//创建一条线程 
	ret = pthread_create(&tid , NULL , 	do_thread , NULL);
	if(ret != 0)
	{
		return -1 ; 
	}
	ret = pthread_detach(tid);	
	while(fg<19)
	{
		scanf("%d",&a);
	}
	return 0 ; 
}
void *do_thread(void *arg)
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
            printf("剩余%d秒\n",n--);
            fg++;
            lastT=t;
        }
        if(t-beinT==10)
            break;
    }
    printf("ok");
}



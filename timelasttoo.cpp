#include <stdio.h>           //������Ҫ��������ִ��һ�������������߳�Ҳͬʱ���� 
#include <pthread.h>
#include <stdlib.h>
#include<time.h>
//ִ���̵߳�һ������
void *do_thread(void *arg);
 int fg=10;
int main(void)
{
	int a;
	//�����̵߳�id�� 
	pthread_t    tid ; 
	int ret ; 
	//����һ���߳� 
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
            printf("ʣ��%d��\n",n--);
            fg++;
            lastT=t;
        }
        if(t-beinT==10)
            break;
    }
    printf("ok");
}



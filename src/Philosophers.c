
#include "../philosophers.h"

void	*test(void *arg)
{
	char	*str;
	
	write(1, "Hello World!\n", 13);
	str = (char *)arg;
	printf("%s\n", str);
	return ((void*)"goo");
}

int	Philosophers(void)
{
	pthread_t	thread;
	pthread_mutex_t	mutex;
//	pthread_t	thread2;
	char	*ret;


	if (pthread_create(&thread, NULL, test, "Oh, hi! (:") != 0)
		exit (1);
//	if (pthread_create(&thread2, NULL, test, "Oh, hi! (:") != 0)
//		exit (1);
	pthread_mutex_init(&mutex, NULL);
//	pthread_detach(thread);
	pthread_join(thread, (void**)&ret);
//	pthread_join(thread2, (void**)&ret);
//	printf("ret = %s\n", ret);
	return (0);
}

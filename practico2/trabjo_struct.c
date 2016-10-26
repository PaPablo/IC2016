#include <stdio.h>
#include <pthread.h>

typedef struct {
	int x;
	int y;
    pthread_mutex_t mutex;
    pthread_cond_t vacio, lleno;
    int arreglos[10];
} ejemplo_t;

void recibe_ejemplo_t(ejemplo_t *ej) {
	printf("%d %d\n", ej.x, ej->y);
    pthread_mutex_lock(&ej->mutex);
}
int main (int argc, char const *argv[])
{
	ejemplo_t e = {1, 2, PTHREAD_MUTEX_INITIALIZER,
                  PTHREAD_COND_INITIALIZER,
                  PTHREAD_COND_INITIALIZER
                  };
	/* code */
	recibe_ejemplo_t(&e);
	return 0;
}
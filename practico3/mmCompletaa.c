
#include <stdio.h>

#define N 5000

double a[N][N];
double b[N][N];
double c[N][N];

void iniMat(double mat[][N], int len, double iniVal)
{
	int i, j;

	for(i = 0; i < len; ++i)
		for(j = 0; j < len; ++j)
			mat[i][j] = iniVal;
}

/* Check1Mat: Chequea que la matriz resultante quede con
 *            el mismo valor en todas sus celdas.
 * Es un chequeo básico para el caso particular en que
 * se sabe de ante mano que el resultado tendrá esa forma.
 * Retorna: 0 en caso correcto (todas las celdas contiene
 *          finVal. 1 En caso de error (al menos una celda
 * contiene un valor diferente de finVal).
 */
int Check1Mat(double mat[][N], int len, double finVal)
{
	int i, j;

	for(i = 0; i < len; ++i)
		for(j = 0; j < len; ++j)
			if(mat[i][j] != finVal)
				return 1;
	return 0;
}

void printMat(double mat[][N], int len)
{
	int i, j;

	for(i = 0; i < len; ++i)
	{
		for(j = 0; j < len; ++j)
			printf( "%.1f ", mat[i][j]);
		printf("\n");
	}
}

void mm(void)
{
	int i, j, k;

	for(i = 0; i < N; ++i)
		for(j = 0; j < N; ++j)
			for(k = 0; k < N; ++k)
				c[i][j] += a[i][k] * b[k][j];
}

int main(void)
{
	printf("Comienzo ...\n");
	iniMat(a, N, 1.0);
	iniMat(b, N, 1.0);
	iniMat(c, N, 0.0);

	printf("Multiplicando ... %d\n", N);

	mm();

	if(Check1Mat(c, N, N) == 0)
		printf("Fin Multiplicación (Resultado correcto)\n");
	else
		printf("Fin Multiplicación (Resultado INCORRECTO!)\n");

/*
	printf("Matriz a:\n");
	printMat(a, N);
	printf("Matriz b:\n");
	printMat(b, N);
	printf("Matriz c:\n");
	printMat(c, N);
*/

	printf("Fin del programa\n");
}
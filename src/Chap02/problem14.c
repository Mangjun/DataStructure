#include <stdio.h>

double harmonic_series(int n);

int main()
{
	int n;
	scanf("%d", &n);
	printf("%lf\n", harmonic_series(n));

	return 0; 
}

double harmonic_series(int n)
{
	double sum = 0;
	for(int i = 1; i <= n; i++) {
		sum += (double) 1 / i;
	}
	return sum;
}

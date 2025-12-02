#include <stdio.h>

int binomial_coefficient(int n, int k);

int main()
{
	int n, k;
	scanf("%d %d", &n, &k);
	printf("%d\n", binomial_coefficient(n, k));

	return 0;
}

int binomial_coefficient(int n, int k)
{
	if (k == n || k == 0) {
		return 1;
	}

	return binomial_coefficient(n - 1, k - 1) + binomial_coefficient(n - 1, k); 
}

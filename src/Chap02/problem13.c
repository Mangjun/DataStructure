#include <stdio.h>

int sum(int n);

int main()
{
	int n;
	scanf("%d", &n);
	printf("%d\n", sum(n));

	return 0; 
}

int sum(int n)
{
	int sum = 0;
	for(int i = 1; i <= n; i++) {
		sum += i;	
	}
	return sum;
}

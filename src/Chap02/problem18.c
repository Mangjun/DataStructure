#include <stdio.h>

int ackermann(int m, int n);

int main()
{
	int m, n;
	scanf("%d %d", &m, &n);
	printf("%d\n", ackermann(m, n));

	return 0;
}

int ackermann(int m, int n)
{
	if (m == 0) {
		return (n + 1);
	}

	if (n == 0) {
		return ackermann(m - 1, 1);
	}

	return ackermann(m - 1, ackermann(m, n - 1));
}

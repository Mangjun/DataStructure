#include <stdio.h>
#define MAX(a, b) (((a) > (b))? (a) : (b))
#define MAX_DEGREE 101

// 메모리를 많이 차지하는 것부터 선언
typedef struct {
	float coef[MAX_DEGREE]; // 다항식의 계수
	int degree; // 다항식의 차수
} polynomial;

polynomial poly_add1(polynomial A, polynomial B)
{
	polynomial C; // 결과 다항식
	int Apos = 0, Bpos = 0, Cpos = 0; // 배열 인덱스 변수
	int degree_a = A.degree;
	int degree_b = B.degree;
	C.degree = MAX(A.degree, B.degree); // 결과 다항식 차수

	while (Apos <= A.degree && Bpos <= B.degree) {
		if (degree_a > degree_b) {
			C.coef[Cpos++] = A.coef[Apos++];
			degree_a--;
		}
		else if (degree_a == degree_b) {
			C.coef[Cpos++] = A.coef[Apos++] + B.coef[Bpos++];
			degree_a--; degree_b--;
		}
		else {
			C.coef[Cpos++] = B.coef[Bpos++];
			degree_b--;
		}
	}
	
	return C;
}

void print_poly(polynomial p)
{
	for (int i = p.degree; i > 0; i--) {
		printf("%3.1fx^%d + ", p.coef[p.degree - i], i);
	}
	printf("%3.1f \n", p.coef[p.degree]);
}

int main()
{
	polynomial a = { { 3, 6, 0, 0, 0, 10 }, 5};
	polynomial b = { { 7, 0, 5, 0, 1 }, 4};
	polynomial c;

	print_poly(a);
	print_poly(b);
	c = poly_add1(a, b);
	printf("-----------------------------------------------------------------------------\n");
	print_poly(c);
	
	return 0;
}

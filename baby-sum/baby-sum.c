//gcc baby-sum.c -o baby-sum -fno-stack-protector
#include <stdio.h>
#include <string.h>

#define sz 2

void vuln(void * param_1)
{
	printf(param_1);
	puts("");
}

void welcome()
{
	char name[0x40];
	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
	setbuf(stderr, NULL);
	printf("Welcome to my useless adder. Currently it supports only %d numbers to be added at a time. I hope you can make use of it somewhere and please don't overflow me :(\n", sz+1);
	printf("Here is a very generous leak for you: 0x%lx\n", name);
	puts("BTW, what should I call you?");
	scanf("%63s", name);
	printf("Nice to meet you, %s\n", name);
	return;
}

void calc()
{
	char fmt[] = "%8s\x00\x00\x00\x00";
	long nums[sz] = {0};
	long * iter = nums;
	long i;
	long sum;

	puts("Please enter your numbers one by one.");

	i = 0;
	while(1)
	{
		printf("> ");
		scanf(fmt, iter);
		vuln(iter);
		if (i > sz)	break;
		iter++;
		i++;
	}

	for (i = 0, sum = 0 ; i!=sz ; i++)	sum += nums[i];
	printf("The sum of numbers entered is: %ld\n", sum);
	return;
}

int main()
{
	char bla[0x10];
	memset(bla, 0, 0x100);
	
	welcome();

	calc();
	puts("Good bye.");
	return 0;
}
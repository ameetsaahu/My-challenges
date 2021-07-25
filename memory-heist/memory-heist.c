// gcc memory-heist -o memory-heist
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/mman.h>

#define max_allowed_chunks 12
char * chunks[max_allowed_chunks] = {NULL};
long free_hook = 0;

void initialize (void) __attribute__ ((constructor));

void initialize()
{
	setvbuf(stdin, NULL, _IONBF, 0);
	setvbuf(stdout, NULL, _IONBF, 0);
	setvbuf(stderr, NULL, _IONBF, 0);
	free_hook   = ((long)rand + 0x1a3c98) % 0x0000800000000000;
}

void welcome()
{
	alarm((rand()%1337)/20);
	puts("\nWelcome to Memory Heist.\n");
}

unsigned long menu()
{
	unsigned long choice = 0;
	puts("1. Allocate");
	puts("2. Delete");
	puts("3. Print");
	printf("> ");
	scanf("%lu", &choice);
	return choice;
}

void allocate()
{
	size_t size = 0;
	unsigned long idx = 0;

	puts("Enter the index for memory.");
	printf("> ");
	scanf("%lu", &idx);

	puts("Enter the size of memory.");
	printf("> ");
	scanf("%lu", &size);
	
	if (idx >= max_allowed_chunks || idx < 0 || chunks[idx] != NULL)
	{
		puts("Duh!");
		_exit(1);
	}

	chunks[idx] = (char *)malloc(size + 2);

	printf("Memory> ");
	size = read(0, (char *)(chunks + 20), size);
	((char *)(chunks + 20))[size - 1] = '\x00';
	memcpy(chunks[idx], ((char *)(chunks + 20)), size);
	puts("Saved.");
}

void delete()
{
	unsigned long idx = 0;
	puts("Enter the index.");
	printf("> ");
	scanf("%lu", &idx);

	if (idx < 0 || idx >= max_allowed_chunks || chunks[idx] == NULL || *(long *)free_hook != 0)
	{
		puts("Duh!");
		_exit(1);
	}

	free(chunks[idx]);
	puts("Done.");
}

void print()
{
	unsigned long idx, percent_present;
	char * iter = NULL;
	char accept[8];
	
	puts("Re-visting memories comes at a cost.");
	puts("Should you choose to accept to re-visit, half of your memories will be lost.");
	printf("[Y/N]> ");

	read(0, accept, 6);
	if (accept[0] == 'N' || accept[0] == 'n')
	{
		puts("Thats alright.");
		return;
	}

	printf("Index> ");
	scanf("%lu", &idx);

	//printf hardening done.
	iter = (char *)chunks[idx];
	percent_present = 0;
	while(*iter)
	{
		if (*iter == '%')
			percent_present = 1;
		if (percent_present && *iter == 'n')
		{
			puts("Whoaa! Whatcha doin'?");
			_exit(1);
		}
		iter = iter + 1;
	}

	printf("Contents:");
	printf(chunks[idx]);

	//Erase half memories.
	for ( idx = idx%2 ; idx < max_allowed_chunks ; idx += 2 )
		chunks[idx] = (char *)0x0054696d616f6877;
}

int main()
{
	welcome();
	unsigned long choice = 0;
	while(1)
	{
		choice = menu();
		switch(choice)
		{
			case 1:
				allocate();
				break;

			case 2:
				delete();
				break;
			
			case 3:
				print();
				break;

			default:
				puts("Duh!");
				_exit(1);
		}
	}
}
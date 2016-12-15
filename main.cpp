#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct S_Book {
	
	char author[10];
	char name[10];
	char publ[10];
	char year[4];
	char pages[4];
	struct S_Book *next;
	struct S_Book *previous;

} Book;

void PrintList(Book *start) {
	Book *currentBook = start;
	int count = 0;

	Book *ahead = NULL;
	Book *behind = NULL;

	while (currentBook != NULL) {
		count++;

		ahead = currentBook->next;
		behind = currentBook->previous;

		printf("Book:%d Author:%s Name:%s Publisher:%s Year:%s Pages:%s Ahead:%s Behind:%s\n", count,
			currentBook->author,
			currentBook->name,
			currentBook->publ,
			currentBook->year,
			currentBook->pages,
			(ahead == NULL) ? "None" : ahead->name,
			(behind == NULL) ? "None" : behind->name
		);
		currentBook = currentBook->next;
		ahead = NULL;
		behind = NULL;
	}
	printf("Total books:%d\n", count);
}

Book *MakeNewBook() {

	printf("Enter Author Name Publisher Year Pages: ");
	char input[256];
	fgets(input, 255, stdin);
	
	Book *newBook = new Book;
	sscanf(input, "%s %s %s %s %s", 
		newBook->author, 
		newBook->name, 
		newBook->publ, 
		newBook->year, 
		newBook->pages);
	printf("Added:%s Name:%s Publisher:%s Year:%s Pages:%s", 
		newBook->author, 
		newBook->name, 
		newBook->publ, 
		newBook->year, 
		newBook->pages);

	newBook->next = NULL;
	newBook->previous = NULL;

	return newBook;
}

Book *AddToStart(Book*startPtr) {
	Book *newBook = MakeNewBook();
	if (startPtr != NULL) {
		startPtr->previous = newBook;
		newBook->next = startPtr;
	}

	return newBook;
}

Book *AddToEnd(Book *startPtr) {
	Book *returnPtr = startPtr;
	Book *newBook = NULL;

	if (startPtr == NULL) {
		newBook = AddToStart(startPtr);
		returnPtr = newBook;
	}
	else {
		Book *indexBook = startPtr;
		while (indexBook->next != NULL) {
			indexBook = indexBook->next;
		}
		newBook = MakeNewBook();
		indexBook->next = newBook;
		newBook->next = NULL;
		newBook->previous = indexBook;
	}
	return returnPtr;
}

void CleanUp(Book *start) {

	Book *freeMe = start;
	Book *holdMe = NULL;
	while (freeMe != NULL) {
		holdMe = freeMe->next;
		printf("Free Author:%s Name:%s Publ:%s Year:%s Pages:%s\n",
			freeMe->author,
			freeMe->name,
			freeMe->publ,
			freeMe->year,
			freeMe->pages);
		free(freeMe);
		freeMe = holdMe;
	}
}

void WriteListToFile(Book *start) {
	FILE *pFile;
	pFile = fopen("myList.bin", "w");

	if (pFile != NULL) {
		Book *currentBook = start;

		Book *holdNext = NULL;
		Book *holdPrevious = NULL;

		while (currentBook != NULL) {
			holdNext = currentBook->next;
			holdPrevious = currentBook->previous;

			currentBook->next = NULL;
			currentBook->previous = NULL;

			fseek(pFile, 0, SEEK_END);
			fwrite(currentBook, sizeof(Book), 1, pFile);

			printf("Writing:%s to file\n", currentBook->name);

			currentBook->next = holdNext;
			currentBook->previous = holdPrevious;

			holdNext = NULL;
			holdPrevious = NULL;

			currentBook = currentBook->next;
		}
		fclose(pFile);
		pFile = NULL;
	}
	else {
		printf("FILE OPEN ERROR\n");
	}

}

int main() {

	char command[16];
	char input[16];

	Book *start = NULL;

	while (fgets(input, 15, stdin)) {

		sscanf(input, "%s", command);

		if (strncmp(command, "quit", 4) == 0) {
			printf("\n\nBreaking...\n");
			break;
		}
		else if (strncmp(command, "print", 5) == 0) {
			PrintList(start);
		}
		else if (strncmp(command, "write", 5) == 0) {
			WriteListToFile(start);
		}
		else if (strncmp(command, "addstart", 8) == 0) {
			start = AddToStart(start);
			PrintList(start);
		}
		else if (strncmp(command, "add", 3) == 0) {
			start = AddToEnd(start);
			PrintList(start);
		}
	}

	CleanUp(start);

	return 0;

}

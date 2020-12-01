/*
Assignment 4: Threads
Creator: Josh Bell
Date of Creation: November 14th 2020
Date of Completion: ---
File: main.c
*/

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define NUM_THREADS 4
#define MAX_CHAR_OUTPUT 80
#define MAX_CHAR_INPUT 1000

char* buffer_1[MAX_CHAR_INPUT];
int count1 = 0;
int prod1 = 0;
int con1 = 0;
pthread_mutex_t mutex_1 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_1 = PTHREAD_COND_INITIALIZER;

char* buffer_2[MAX_CHAR_INPUT];
int count2 = 0;
int prod2 = 0;
int con2 = 0;
pthread_mutex_t mutex_2 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_2 = PTHREAD_COND_INITIALIZER;

char* buffer_3[MAX_CHAR_INPUT];
int count3 = 0;
int prod3 = 0;
int con3 = 0;
pthread_mutex_t mutex_3 = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full_3 = PTHREAD_COND_INITIALIZER;

// Using example code from...
// https://repl.it/@cs344/62threadexamplec#main.c
// https://repl.it/@cs344/65prodconspipelinec#main.c

//void* perform_work(void* argument) {
//	pthread_t pw_tid = pthread_self();
//	int passed_in_value;
//	// We cast the void* to int*, and then dereference the int* to get the int value the pointer is pointing to
//	passed_in_value = *((int*)argument);
//	printf("It's me, thread with argument %d!\n", passed_in_value);
//	return NULL;
//}

// ============================================BUFFERS=============================================
void put_buff_1(char* item) {
	// Lock the mutex before putting the item in the buffer
	pthread_mutex_lock(&mutex_1);
	// Put the item in the buffer
	buffer_1[prod1] = item;
	// Increment the index where the next item will be put.
	prod1 = prod1 + 1;
	count1++;
	// Signal to the consumer that the buffer is no longer empty
	pthread_cond_signal(&full_1);
	// Unlock the mutex
	pthread_mutex_unlock(&mutex_1);
}

char* get_buff_1() {
	// Lock the mutex before checking if the buffer has data
	pthread_mutex_lock(&mutex_1);
	while (count1 == 0)
		// Buffer is empty. Wait for the producer to signal that the buffer has data
		pthread_cond_wait(&full_1, &mutex_1);
	char* item = buffer_1[con1];
	// Increment the index from which the item will be picked up
	con1 = con1 + 1;
	count1--;
	// Unlock the mutex
	pthread_mutex_unlock(&mutex_1);
	// Return the item
	return item;
}
// ------------------------------------------------------------------------------------------------
void put_buff_2(char* item) {
	// Lock the mutex before putting the item in the buffer
	pthread_mutex_lock(&mutex_2);
	// Put the item in the buffer
	buffer_2[prod2] = item;
	// Increment the index where the next item will be put.
	prod2 = prod2 + 1;
	count2++;
	// Signal to the consumer that the buffer is no longer empty
	pthread_cond_signal(&full_2);
	// Unlock the mutex
	pthread_mutex_unlock(&mutex_2);
}

char* get_buff_2() {
	// Lock the mutex before checking if the buffer has data
	pthread_mutex_lock(&mutex_2);
	while (count2 == 0)
		// Buffer is empty. Wait for the producer to signal that the buffer has data
		pthread_cond_wait(&full_2, &mutex_2);
	char* item = buffer_2[con2];
	// Increment the index from which the item will be picked up
	con2 = con2 + 1;
	count2--;
	// Unlock the mutex
	pthread_mutex_unlock(&mutex_2);
	// Return the item
	return item;
}
// ------------------------------------------------------------------------------------------------
void put_buff_3(char* item) {
	// Lock the mutex before putting the item in the buffer
	pthread_mutex_lock(&mutex_3);
	// Put the item in the buffer
	buffer_3[prod3] = item;
	// Increment the index where the next item will be put.
	prod3 = prod3 + 1;
	count3++;
	// Signal to the consumer that the buffer is no longer empty
	pthread_cond_signal(&full_3);
	// Unlock the mutex
	pthread_mutex_unlock(&mutex_3);
}

char* get_buff_3() {
	// Lock the mutex before checking if the buffer has data
	pthread_mutex_lock(&mutex_3);
	while (count3 == 0)
		// Buffer is empty. Wait for the producer to signal that the buffer has data
		pthread_cond_wait(&full_3, &mutex_3);
	char* item = buffer_3[con3];
	// Increment the index from which the item will be picked up
	con3 = con3 + 1;
	count3--;
	// Unlock the mutex
	pthread_mutex_unlock(&mutex_3);
	// Return the item
	return item;
}
// ================================================================================================
/*
void* inputThread
Reads in lines of characters from the standard input
Parameters: ---
Returns:	NULL
*/
void* inputThread() {
	// Initilizes Values
	// NULL instead of calloc
	char* input = calloc(MAX_CHAR_INPUT + 1, sizeof(char));
	char* currLine = calloc(MAX_CHAR_INPUT + 1, sizeof(char));
	size_t maxSize = MAX_CHAR_INPUT;

	// Recieves input from standard input
	while (getline(&currLine, &maxSize, stdin)) {
		if (strcmp(currLine, "STOP\n") == 0)
			break;
		strcat(input, currLine);
	}

	// Prints out input
	//printf("\nInput:\n%s\n", input);
	//fflush(stdout);

	put_buff_1(input);

	return NULL;
}

// ================================================================================================

/*
void* lineSeperator
Replaces every line separator '\n' in the input by a space ' '.
Parameters: ---
Returns:	NULL
*/
void* lineSeperator() {
	// Initilize Values
	int sizeOfBuffer = 0;
	char* beforeLine = calloc(MAX_CHAR_INPUT + 1, sizeof(char));
	beforeLine = get_buff_1();
	sizeOfBuffer = strlen(beforeLine);

	// Finds any '\n' and changes it into ' '
	for (int i = 0; i < sizeOfBuffer; i++) {
		if (beforeLine[i] == '\n')
			beforeLine[i] = ' ';
	}

	// Print the new buffer
	//printf("Input in Line:\n%s\n\n", beforeLine);
	//fflush(stdout);

	// Place value into buffer 2
	put_buff_2(beforeLine);

	return NULL;
}

// ================================================================================================

/*
void* plusSign
Replaces every pair of plus signs, i.e., "++", by a "^".
Parameters: ---
Returns:	NULL
*/
void* plusSign() {
	// Initialize Variables
	int sizeOfBuffer = 0;
	char* beforePlus = calloc(MAX_CHAR_INPUT + 1, sizeof(char));
	beforePlus = get_buff_2();
	sizeOfBuffer = strlen(beforePlus);

	// Changing variables
	for (int i = 0; i < sizeOfBuffer; i++) {
		// Checks to see if there is ++
		if (beforePlus[i] == '+') {
			if (beforePlus[i + 1] == '+') {
				// Changes '++' to '^'
				beforePlus[i] = '^';
				memmove(&beforePlus[i + 1], &beforePlus[i + 2], strlen(beforePlus) - i + 1);
			}
		}
	}

	// Print the new buffer
	//printf("Input in Plus:\n%s\n\n", beforePlus);
	//fflush(stdout);

	// Place Value in buffer 3
	put_buff_3(beforePlus);

	return NULL;
}

// ================================================================================================

/*
void* output
Writes this processed data to standard output as lines of exactly 80 characters.
Parameters: ---
Returns:	NULL
*/
void* output() {
	// Initialize Values
	int count = 0;
	int loopValue = 0;
	int index = 0;
	int sizeOfBuffer = 0;
	char* beforeOutput = calloc(MAX_CHAR_INPUT + 1, sizeof(char));
	beforeOutput = get_buff_3();
	sizeOfBuffer = strlen(beforeOutput);

	// Finds the number of lines to create
	loopValue = sizeOfBuffer / MAX_CHAR_OUTPUT;

	//printf("Input in Output:\n");
	//fflush(stdout);

	while (loopValue != 0) {
		// Prints out the char
		printf("%c", beforeOutput[index]);
		fflush(stdout);
		count++;
		index++;

		// If we have printed 80 chars then we start the next line and decrease the loopvalue
		if (count >= MAX_CHAR_OUTPUT) {
			printf("\n");
			fflush(stdout);
			count = 0;
			loopValue--;
		}
	}

	printf("\n");
	fflush(stdout);

	return NULL;
}

// ================================================================================================

int main() {
	pthread_t threads[NUM_THREADS];
	int thread_args[NUM_THREADS];
	int result_code, index;

	for (index = 0; index < NUM_THREADS; ++index) {
		if (index == 0) { // Input Thread reads in lines of characters from the standard input.
			thread_args[index] = index;
			result_code = pthread_create(&threads[index], NULL, inputThread, NULL);
			assert(0 == result_code);
		}
		if (index == 1) { // Line Separator Thread, replaces every line separator in the input by a space.
			thread_args[index] = index;
			result_code = pthread_create(&threads[index], NULL, lineSeperator, NULL);
			assert(0 == result_code);
		}
		if (index == 2) { // Plus Sign thread, replaces every pair of plus signs, i.e., "++", by a "^".
			thread_args[index] = index;
			result_code = pthread_create(&threads[index], NULL, plusSign, NULL);
			assert(0 == result_code);
		}
		if (index == 3) { // Output Thread, writes this processed data to standard output as lines of exactly 80 characters.
			thread_args[index] = index;
			result_code = pthread_create(&threads[index], NULL, output, NULL);
			assert(0 == result_code);
		}
		else {
			// Do Nothing
		}

		//thread_args[index] = index;
		//printf("In main: creating thread %d\n", index);
		//result_code = pthread_create(&threads[index], NULL, perform_work, (void*)&thread_args[index]);
		//assert(0 == result_code);
	}

	// Wait for each thread to complete
	for (index = 0; index < NUM_THREADS; ++index) {
		result_code = pthread_join(threads[index], NULL);
		assert(0 == result_code);
	}
	exit(EXIT_SUCCESS);
}
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include<unistd.h>

#if defined(DYNAMIC)
    #include "DynamicArrayStack.h"
    #include "DynamicArrayStackFunctions.h"
#elif defined(LINKED)
    #include "LinkedListStack.h"
    #include "LinkedListStackFunctions.h"
#endif

#include "Tests.h"

#define TEST_REPEATS 20
#define CONST_DIGIT  52
#define AIM_STACK_SIZE (size_t)1e6

int main() {
    double first_test_time  = 0;
    double second_test_time = 0;
    double third_test_time  = 0;
    for(int i = 0; i < TEST_REPEATS; i++) {
        first_test_time += firstTest();
    }

    first_test_time /= TEST_REPEATS;
    printf("FIRST TEST: %lf\n", first_test_time);

    for(int i = 0; i < TEST_REPEATS; i++) {
        second_test_time += secondTest();
    }

    second_test_time /= TEST_REPEATS;
    printf("SECOND TEST: %lf\n", second_test_time);


    for(int i = 0; i < TEST_REPEATS; i++) {
        third_test_time += thirdTest();
    }

    third_test_time /= TEST_REPEATS;
    printf("THIRD TEST: %lf\n", third_test_time);

    fourthTest();

    return 0;
}

double firstTest() {
    #if defined(DYNAMIC)
        Stack* stack = stackCtr(1, sizeof(int));
    #elif defined(LINKED)
        Stack* stack = stackCtr(sizeof(int));
    #endif

    size_t number_of_operations = AIM_STACK_SIZE;
    size_t big_iteration   = 100000;
    void*  buffer_int      = calloc(1, sizeof(int));
    if(!buffer_int) {
        stackDtr(stack);
        customAssert(buffer_int, 0);
    }

    clock_t start_time = clock();

    for(size_t i = 0; i < number_of_operations; i++) {
        *(int*)buffer_int = i;
        push(stack, buffer_int);
    }

    while(stack->size >= big_iteration) {
        number_of_operations = stack->size / 2;
        for(size_t i = 0; i < number_of_operations; i++) {
            pop(stack);
        }
        number_of_operations = stack->size / 2;
        for(size_t i = 0; i < number_of_operations; i++) {
            *(int*)buffer_int = i;
            push(stack, buffer_int);
        }
    }

    clock_t end_time = clock();
    double total_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;

    FREE(buffer_int);

    stackDtr(stack);

    return total_time;
}

double secondTest() {
    #if defined(DYNAMIC)
        Stack* stack = stackCtr(1, sizeof(int));
    #elif defined(LINKED)
        Stack* stack = stackCtr(sizeof(int));
    #endif

    size_t number_of_operations = AIM_STACK_SIZE;
    size_t small_iteration = 10000;
    size_t big_iteration   = 100000;
    void*  buffer_int      = calloc(1, sizeof(int));
    if(!buffer_int) {
        stackDtr(stack);
        customAssert(buffer_int, 0);
    }

    clock_t start_time = clock();

    for(size_t i = 0; i < number_of_operations; i++) {
        *(int*)buffer_int = i;
        push(stack, buffer_int);
    }

    for(size_t i = 0; i < 100; i++) {
        for(size_t j = 0; j < small_iteration; j++) {
            pop(stack);
        }
        for(size_t j = 0; j < small_iteration; j++) {
            *(int*)buffer_int = i;
            push(stack, buffer_int);
        }
    }

    while(stack->size >= big_iteration) {
        number_of_operations = stack->size / 2;
        for(size_t i = 0; i < number_of_operations; i++) {
            pop(stack);
        }
        number_of_operations = stack->size / 2;
        for(size_t i = 0; i < number_of_operations; i++) {
            *(int*)buffer_int = i;
            push(stack, buffer_int);
        }
    }

    for(size_t i = 0; i < 100; i++) {
        for(size_t j = 0; j < small_iteration; j++) {
            pop(stack);
        }
        for(size_t j = 0; j < small_iteration; j++) {
            *(int*)buffer_int = i;
            push(stack, buffer_int);
        }
    }

    clock_t end_time = clock();
    double total_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;

    FREE(buffer_int);

    stackDtr(stack);

    return total_time;
}

double thirdTest() {
    #if defined(DYNAMIC)
        Stack* stack = stackCtr(1, sizeof(int));
    #elif defined(LINKED)
        Stack* stack = stackCtr(sizeof(int));
    #endif

    srand(42);

    size_t number_of_operations = AIM_STACK_SIZE;
    void*  buffer_int      = calloc(1, sizeof(int));
    if(!buffer_int) {
        stackDtr(stack);
        customAssert(buffer_int, 0);
    }

    for(size_t i = 0; i < number_of_operations; i++) {
        *(int*)buffer_int = (rand() % 2) + 1;
        push(stack, buffer_int);
    }

    clock_t start_time = clock();

    for (size_t i = 0; i < number_of_operations; i++) {
        top(stack, buffer_int);

        if (*(int*)buffer_int == 1) {
            push(stack, buffer_int);
        } else {
            pop(stack);
        }
    }

    clock_t end_time = clock();
    double total_time = (double) (end_time - start_time) / CLOCKS_PER_SEC;

    FREE(buffer_int);

    stackDtr(stack);

    return total_time;
}

int fourthTest () {
    #if defined(DYNAMIC)
        Stack* stack = stackCtr(1, sizeof(int));
        #define STACK "Dynamic"
    #elif defined(LINKED)
        Stack* stack = stackCtr(sizeof(int));
        #define STACK "Linked"
    #endif

    size_t number_of_operations = AIM_STACK_SIZE;
    void*  buffer_int      = calloc(1, sizeof(int));
    if(!buffer_int) {
        stackDtr(stack);
        customAssert(buffer_int, 0);
    }

    system("mkdir -p Tests/TestsOutput");

    FILE* test_output = fopen("Tests/TestsOutput/FourthTestOutput" STACK ".txt", "w");
    if(!test_output) {
        fprintf(stderr, "ERROR: can't open Tests/TestsOutput/FourthTestOutput" STACK ".txt\n");
        customAssert(test_output, 0);
    }

    clock_t start_time   = clock();
    clock_t current_time = 0;
    double  output_time  = 0;
    int     stack_size   = 0;

    while(stack->size <= AIM_STACK_SIZE) {
        stack_size = (int)stack->size + 1;

        *(int*)buffer_int = stack_size;
        push(stack, buffer_int);

        if(stack_size % 1000 == 0) {
            current_time = clock();
            output_time = (double)(current_time - start_time) / CLOCKS_PER_SEC;
            fprintf(test_output, "%d %lf\n", stack_size, output_time);
        }
    }

    fclose(test_output);
    #undef STACK

    return 0;
}

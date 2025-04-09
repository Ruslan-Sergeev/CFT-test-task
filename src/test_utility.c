#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdbool.h>
#include <math.h>
#include <time.h>
#include "../include/stat_data_dump.h"
#include "../tests/stat_data_test_1.h"
#include "../tests/stat_data_test_2.h"
#include "../tests/stat_data_test_3.h"

#define NUM_TESTS 3
#define EPSILON 0.001f

static long get_file_size(const char *filename);
static stat_data_dump* load_dump(const char *file_name);
static void store_dump(stat_data_dump *dump, const char *file_name);
struct timespec diff_timespec(struct timespec start, struct timespec end);
static bool compare_stat_data(const stat_data *a, const stat_data *b);
static void dump_cleanup(stat_data_dump *dump);

int main()
{
    stat_data_dump dump;
    stat_data_dump *dump_ptr = &dump;
    stat_data_dump *dump_ptr_result = NULL;
    struct timespec start, end, elapsed;
    stat_data *element_a = NULL;
    stat_data *element_b = NULL;

    printf("Test utility\n");

    printf("TEST #1 N = 10\n");
    dump_ptr->array = case_1_in_a;
    dump_ptr->num_elements = sizeof(case_1_in_a)/sizeof(case_1_in_a[0]);
    store_dump(dump_ptr, "dump_1_a.bin");
    dump_ptr->array = case_1_in_b;
    dump_ptr->num_elements = sizeof(case_1_in_b)/sizeof(case_1_in_b[0]);
    store_dump(dump_ptr, "dump_1_b.bin");

    clock_gettime(CLOCK_MONOTONIC, &start);
    if (system("./processing_utility dump_1_a.bin dump_1_b.bin dump_1_out.bin") != EXIT_SUCCESS)
    {
        fprintf(stderr, "Failed to execute processing_utility!\n");
        exit(EXIT_FAILURE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = diff_timespec(start, end);
    printf("./processing_utility execution time: %ld seconds and %ld nanoseconds\n",
            elapsed.tv_sec, elapsed.tv_nsec);

    dump_ptr_result = load_dump("dump_1_out.bin");
    dump_ptr->array = case_1_out;
    dump_ptr->num_elements = sizeof(case_1_out)/sizeof(case_1_out[0]);
    element_a = dump_ptr_result->array;
    element_b = dump_ptr->array;
    for (unsigned long i = 0; i < dump_ptr_result->num_elements; i++)
    {
        if (!compare_stat_data(element_a, element_b))
        {
            fprintf(stderr, "TEST #1 FAILED!\n");
            fprintf(stderr, "id = %lu\n", i);
            goto exit;
        }
        element_a += 1;
        element_b += 1;
    }
    dump_cleanup(dump_ptr_result);
    fprintf(stdout, "TEST #1 PASSED!\n\n");

    printf("TEST #2 N = 100000\n");
    dump_ptr->array = case_2_in_a;
    dump_ptr->num_elements = sizeof(case_2_in_a)/sizeof(case_2_in_a[0]);
    store_dump(dump_ptr, "dump_2_a.bin");
    dump_ptr->array = case_2_in_b;
    dump_ptr->num_elements = sizeof(case_2_in_b)/sizeof(case_2_in_b[0]);
    store_dump(dump_ptr, "dump_2_b.bin");

    clock_gettime(CLOCK_MONOTONIC, &start);
    if (system("./processing_utility dump_2_a.bin dump_2_b.bin dump_2_out.bin") != EXIT_SUCCESS)
    {
        fprintf(stderr, "Failed to execute processing_utility!\n");
        exit(EXIT_FAILURE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = diff_timespec(start, end);
    printf("./processing_utility execution time: %ld seconds and %ld nanoseconds\n",
            elapsed.tv_sec, elapsed.tv_nsec);

    dump_ptr_result = load_dump("dump_2_out.bin");
    dump_ptr->array = case_2_out;
    dump_ptr->num_elements = sizeof(case_2_out)/sizeof(case_2_out[0]);
    element_a = dump_ptr_result->array;
    element_b = dump_ptr->array;
    for (unsigned long i = 0; i < dump_ptr_result->num_elements; i++)
    {
        if (!compare_stat_data(element_a, element_b))
        {
            fprintf(stderr, "TEST #2 FAILED!\n");
            fprintf(stderr, "id = %lu\n", i);
            goto exit;
        }
        element_a += 1;
        element_b += 1;
    }
    dump_cleanup(dump_ptr_result);
    fprintf(stdout, "TEST #2 PASSED!\n\n");

    printf("TEST #3 N = 0\n");
    dump_ptr->array = case_3_in_a;
    dump_ptr->num_elements = 0;
    store_dump(dump_ptr, "dump_3_a.bin");
    dump_ptr->array = case_3_in_b;
    dump_ptr->num_elements = 0;
    store_dump(dump_ptr, "dump_3_b.bin");

    clock_gettime(CLOCK_MONOTONIC, &start);
    if (system("./processing_utility dump_3_a.bin dump_3_b.bin dump_3_out.bin") != EXIT_SUCCESS)
    {
        fprintf(stderr, "Failed to execute processing_utility!\n");
        exit(EXIT_FAILURE);
    }
    clock_gettime(CLOCK_MONOTONIC, &end);
    elapsed = diff_timespec(start, end);
    printf("./processing_utility execution time: %ld seconds and %ld nanoseconds\n",
           elapsed.tv_sec, elapsed.tv_nsec);

    dump_ptr_result = load_dump("dump_3_out.bin");
    dump_ptr = NULL;
    if (dump_ptr == dump_ptr_result)
    {
        fprintf(stdout, "TEST #3 PASSED!\n");
    }
    else
    {
        fprintf(stderr, "TEST #3 FAILED!\n");
        dump_cleanup(dump_ptr_result);
        goto exit;
    }

exit:
    exit(EXIT_SUCCESS);
}

static long get_file_size(const char *filename) {
    struct stat st;

    if (stat(filename, &st) == 0)
    {
        return st.st_size;
    }
    else
    {
        perror("Failed to get size of file");
        return 0;
    }
}

static stat_data_dump* load_dump(const char *file_name)
{
    size_t ret;
    FILE *file;
    unsigned long file_size;
    unsigned long num_elements;
    stat_data *array = NULL;
    stat_data_dump *dump = NULL;

    if ((file = fopen(file_name, "rb")) == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }

    file_size = get_file_size(file_name);
    if (file_size == 0)
        return NULL;

    num_elements = file_size / sizeof(stat_data);
    array = (stat_data *)malloc(num_elements * sizeof(stat_data));
    if (array == NULL)
    {
        fprintf(stderr, "Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }
    dump = (stat_data_dump *)malloc(sizeof(stat_data_dump));
    if (dump == NULL)
    {
        fprintf(stderr, "Failed to allocate memory!\n");
        exit(EXIT_FAILURE);
    }

    ret = fread(array, sizeof(stat_data), num_elements, file);
    if (ret != num_elements)
    {
        fprintf(stderr, "Failed to read data from file\n");
        exit(EXIT_FAILURE);
    }

    fclose(file);
    dump->array = array;
    dump->num_elements = num_elements;
    return dump;
}

static void store_dump(stat_data_dump *dump, const char *file_name)
{
    size_t ret;
    FILE *file;

    if ((file = fopen(file_name, "wb")) == NULL)
    {
        perror("Failed to open file");
        exit(EXIT_FAILURE);
    }
    if(dump != NULL)
    {
        ret = fwrite(dump->array, sizeof(stat_data), dump->num_elements, file);
        if (ret != dump->num_elements)
        {
            fprintf(stderr, "Failed to write data to file\n");
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);
    return;
}

static bool compare_stat_data(const stat_data *a, const stat_data *b)
{
    float k = fabsf(a->cost - b->cost);
    return (a->id == b->id) &&
           (a->count == b->count) &&
           (k < EPSILON) &&
           (a->primary == b->primary) &&
           (a->mode == b->mode);
}


struct timespec diff_timespec(struct timespec start, struct timespec end) {
    struct timespec diff;
    diff.tv_sec = end.tv_sec - start.tv_sec;
    diff.tv_nsec = end.tv_nsec - start.tv_nsec;

    if (diff.tv_nsec < 0) {
        diff.tv_sec -= 1;
        diff.tv_nsec += 1000000000;
    }

    return diff;
}

static void dump_cleanup(stat_data_dump *dump)
{
    free(dump->array);
    free(dump);
    return;
}
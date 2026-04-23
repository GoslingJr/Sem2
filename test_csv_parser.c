#include <assert.h>
#include <stdio.h>
#include "csv_parser.h"

int main() {
    // Создаем тестовый CSV файл
    FILE* test = fopen("test.csv", "w");
    fprintf(test, "Name,Age,Score\n");
    fprintf(test, "Alice,25,95.5\n");
    fprintf(test, "Bob,30,87.3\n");
    fclose(test);
    
    CSVData* data = parse_csv("test.csv");
    assert(data != NULL);
    assert(data->row_count == 3);
    assert(data->col_count == 3);
    assert(strcmp(data->rows[0][0], "Name") == 0);
    assert(strcmp(data->rows[1][0], "Alice") == 0);
    assert(strcmp(data->rows[1][1], "25") == 0);
    
    printf("All tests passed!\n");
    free_csv_data(data);
    remove("test.csv");
    return 0;
}

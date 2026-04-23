#include "csv_parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Вспомогательная функция: разделяет строку на поля
static char** split_line(const char* line, int* count) {
    // Простая реализация без поддержки кавычек
    int capacity = 10;
    char** fields = malloc(capacity * sizeof(char*));
    *count = 0;
    
    const char* start = line;
    const char* current = line;
    
    while (*current) {
        if (*current == ',') {
            int len = current - start;
            fields[*count] = malloc(len + 1);
            strncpy(fields[*count], start, len);
            fields[*count][len] = '\0';
            (*count)++;
            
            if (*count >= capacity) {
                capacity *= 2;
                fields = realloc(fields, capacity * sizeof(char*));
            }
            start = current + 1;
        }
        current++;
    }
    
    // Последнее поле
    if (start <= current) {
        int len = current - start;
        fields[*count] = malloc(len + 1);
        strncpy(fields[*count], start, len);
        fields[*count][len] = '\0';
        (*count)++;
    }
    
    return fields;
}

CSVData* parse_csv(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        return NULL;
    }
    
    CSVData* data = malloc(sizeof(CSVData));
    data->rows = NULL;
    data->row_count = 0;
    data->col_count = 0;
    
    char* line = NULL;
    size_t len = 0;
    ssize_t read;
    
    // Читаем первую строку (заголовок)
    if ((read = getline(&line, &len, file)) != -1) {
        // Удаляем символ новой строки
        line[strcspn(line, "\n")] = 0;
        
        int cols;
        char** fields = split_line(line, &cols);
        data->col_count = cols;
        
        data->rows = malloc(sizeof(char**));
        data->rows[0] = fields;
        data->row_count = 1;
    }
    
    // Читаем остальные строки
    while ((read = getline(&line, &len, file)) != -1) {
        line[strcspn(line, "\n")] = 0;
        
        int cols;
        char** fields = split_line(line, &cols);
        
        if (cols != data->col_count) {
            // Несоответствие количества столбцов
            for (int i = 0; i < data->row_count; i++) {
                for (int j = 0; j < data->col_count; j++) {
                    free(data->rows[i][j]);
                }
                free(data->rows[i]);
            }
            free(data->rows);
            free(data);
            fclose(file);
            free(line);
            return NULL;
        }
        
        data->rows = realloc(data->rows, (data->row_count + 1) * sizeof(char**));
        data->rows[data->row_count] = fields;
        data->row_count++;
    }
    
    free(line);
    fclose(file);
    return data;
}

void free_csv_data(CSVData* data) {
    if (!data) return;
    
    for (int i = 0; i < data->row_count; i++) {
        for (int j = 0; j < data->col_count; j++) {
            free(data->rows[i][j]);
        }
        free(data->rows[i]);
    }
    free(data->rows);
    free(data);
}

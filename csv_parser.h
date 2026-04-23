#ifndef CSV_PARSER_H
#define CSV_PARSER_H

typedef struct {
    char*** rows;      // Двумерный массив строк
    int row_count;     // Количество строк (включая заголовок)
    int col_count;     // Количество столбцов
} CSVData;

// Парсит CSV файл и возвращает структуру с данными
CSVData* parse_csv(const char* filename);

// Освобождает память, выделенную под CSVData
void free_csv_data(CSVData* data);

#endif

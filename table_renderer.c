#include "table_renderer.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Проверяет, является ли строка числом
static int is_number(const char* str) {
    if (!str || *str == '\0') return 0;
    
    // Пропускаем минус в начале
    if (*str == '-') str++;
    
    int has_digit = 0;
    int has_decimal = 0;
    
    while (*str) {
        if (isdigit(*str)) {
            has_digit = 1;
        } else if (*str == '.' && !has_decimal) {
            has_decimal = 1;
        } else {
            return 0;
        }
        str++;
    }
    return has_digit;
}

// Вычисляет максимальную ширину для каждого столбца
static int* calculate_column_widths(const CSVData* data) {
    int* widths = calloc(data->col_count, sizeof(int));
    
    for (int col = 0; col < data->col_count; col++) {
        // Заголовок
        int len = strlen(data->rows[0][col]);
        if (len > widths[col]) widths[col] = len;
        
        // Данные
        for (int row = 1; row < data->row_count; row++) {
            len = strlen(data->rows[row][col]);
            if (len > widths[col]) widths[col] = len;
        }
    }
    
    return widths;
}

// Рисует разделительную линию
static void draw_line(FILE* out, const int* widths, int col_count, char corner, char horizontal, char intersection) {
    fprintf(out, "%c", corner);
    for (int i = 0; i < col_count; i++) {
        for (int j = 0; j < widths[i] + 2; j++) { // +2 для отступов
            fprintf(out, "%c", horizontal);
        }
        if (i < col_count - 1) {
            fprintf(out, "%c", intersection);
        }
    }
    fprintf(out, "%c\n", corner);
}

// Рисует строку таблицы
static void draw_row(FILE* out, const int* widths, char** fields, int col_count, int is_header) {
    fprintf(out, "|");
    for (int i = 0; i < col_count; i++) {
        const char* text = fields[i];
        int len = strlen(text);
        int width = widths[i];
        
        // Определяем выравнивание
        int is_num = is_number(text);
        
        if (is_header) {
            // Заголовок всегда выравнивается влево
            fprintf(out, " %-*s ", width, text);
        } else if (is_num) {
            // Числа выравниваются вправо
            fprintf(out, " %*s ", width, text);
        } else {
            // Текст выравнивается влево
            fprintf(out, " %-*s ", width, text);
        }
        fprintf(out, "|");
    }
    fprintf(out, "\n");
}

int render_table_to_file(const CSVData* data, const char* filename) {
    if (!data || data->row_count < 1) return -1;
    
    FILE* out = fopen(filename, "w");
    if (!out) return -1;
    
    // Вычисляем ширины столбцов
    int* widths = calculate_column_widths(data);
    
    // Верхняя граница
    draw_line(out, widths, data->col_count, '+', '-', '+');
    
    // Заголовок
    draw_row(out, widths, data->rows[0], data->col_count, 1);
    
    // Разделитель после заголовка
    draw_line(out, widths, data->col_count, '+', '=', '+');
    
    // Данные
    for (int row = 1; row < data->row_count; row++) {
        draw_row(out, widths, data->rows[row], data->col_count, 0);
        
        // Разделитель между строками (кроме последней)
        if (row < data->row_count - 1) {
            draw_line(out, widths, data->col_count, '+', '-', '+');
        }
    }
    
    // Нижняя граница
    draw_line(out, widths, data->col_count, '+', '-', '+');
    
    free(widths);
    fclose(out);
    return 0;
}

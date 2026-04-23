#ifndef TABLE_RENDERER_H
#define TABLE_RENDERER_H

#include "csv_parser.h"

// Рендерит таблицу в файл
int render_table_to_file(const CSVData* data, const char* filename);

#endif

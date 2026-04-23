#include <stdio.h>
#include <stdlib.h>
#include "csv_parser.h"
#include "table_renderer.h"

int main(int argc, char* argv[]) {
    const char* input_file = "input.csv";
    const char* output_file = "output.txt";
    
    CSVData* data = parse_csv(input_file);
    if (!data) {
        fprintf(stderr, "Error: Failed to parse CSV file\n");
        return 1;
    }
    
    int result = render_table_to_file(data, output_file);
    if (result != 0) {
        fprintf(stderr, "Error: Failed to render table\n");
        free_csv_data(data);
        return 1;
    }
    
    printf("Success! Table written to %s\n", output_file);
    free_csv_data(data);
    return 0;
}

#include "csv.h"

void **csv_parse(const char *filename, int *record_count, size_t record_size, RowParser parser) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Unable to open file");
        return NULL;
    }

    // Count the number of lines (records) first
    char line[MAX_LINE_SIZE];
    int count = 0;
    while (fgets(line, sizeof(line), fp)) {
        count++;
    }
    count--; // exclude header
    if (count == 0) {
        printf("No records found\n");
        fclose(fp);
        return NULL;
    }

    // Reset the file pointer to the start
    rewind(fp);

    // Allocate memory for an array of pointers
    void **records = malloc(count * sizeof(void *));
    if (!records) {
        perror("Memory allocation failed");
        fclose(fp);
        return NULL;
    }

    // Parse and populate the records
    int idx = 0;
    fgets(line, sizeof(line), fp); // skip header

    while (fgets(line, sizeof(line), fp) && idx < count) {
        records[idx] = malloc(record_size);
        if (!records[idx]) {
            perror("Memory allocation failed");
            fclose(fp);
            for (int i = 0; i < idx; i++) {
                free(records[i]);
            }
            free(records);
            return NULL;
        }

        int status = parser(line, records[idx]);
        if (status) {
            printf("Failed to parse line %d\n", idx);
            fclose(fp);
            for (int i = 0; i < idx; i++) {
                free(records[i]);
            }
            free(records);
            return NULL;
        }
        idx++;
    }

    fclose(fp);
    *record_count = count;
    return records;
}


void csv_add_new_line(const char *filename, const char *line) {
    FILE *file = fopen(filename, "a");
    if (file == NULL) {
        perror("Failed to open file");
        return;
    }
    fprintf(file, "%s\n", line); 
    fclose(file);
}


void csv_erase_content(const char *filename) {
    FILE *file = fopen(filename, "w"); // Open file in write mode (which truncates the file)

    if (file == NULL) {
        perror("Failed to open file");
        return;
    }

    fclose(file);
}

int csv_parse_foreach(const char *filename, Mtqueue_list *queues, RowParserForeach parser) {

    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("Unable to open file");
        return 1;
    }

    char line[MAX_LINE_SIZE];
    int idx = 0;
    fgets(line, sizeof(line), fp); // skip header

    while (fgets(line, sizeof(line), fp)) {
        int status = parser(line, queues);
        if (status) {
            printf("Failed to parse line %d\n", idx);
            fclose(fp);
            return 1;
        }
        idx++;
    }

    fclose(fp);
    return 0;
}

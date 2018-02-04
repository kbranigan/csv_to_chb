// generated automatically from /Users/kevin/work/branigan.ca/prototypes/csv_to_chb/examples/routes.csv

#include "routes.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct s_routes * load_routes(int * ret_count)
{
  char file_name[500];
  snprintf(file_name, sizeof(file_name), "/Users/kevin/work/branigan.ca/prototypes/csv_to_chb/examples/routes.b");
  printf("loading data from '%s'\n", file_name);
  FILE * fp = fopen(file_name, "rb");
  if (fp == NULL) { fprintf(stderr, "find could not be opened, perhaps you gave it a different relative path?\n"); return NULL; }
  int32_t row_width = 0, num_rows = 0;
  fread(&row_width, sizeof(row_width), 1, fp);
  fread(&num_rows, sizeof(num_rows), 1, fp);
  if (row_width != sizeof(struct s_routes) /*60*/) { fprintf(stderr, "file row_width (%d) seems invalid\n", row_width); fclose(fp); return 0; }
  struct s_routes * rows = (struct s_routes *)malloc(sizeof(struct s_routes) * num_rows);
  if (rows == NULL) { fprintf(stderr, "couldnt alloc memory for file data\n"); fclose(fp); return 0; }
  int32_t ret_num_rows = fread(rows, row_width, num_rows, fp);
  if (ret_num_rows != num_rows) { fprintf(stderr, "couldnt read all of the file data (%d vs %d)\n", ret_num_rows, num_rows); fclose(fp); return 0; }
  fclose(fp);
  *ret_count = num_rows;
  return rows;
}


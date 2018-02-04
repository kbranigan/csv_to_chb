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

int num_routes = 20;
struct s_routes routes[20] = {
  { 53420, 1, 1, "LINE 1 (YONGE-UNIVERSITY)", 1, "D5C82B", "000000" },
  { 53209, 1, 10, "VAN HORNE", 3, "804000", "FFFFFF" },
  { 53210, 1, 100, "FLEMINGDON PARK", 3, "FF00FF", "FFFFFF" },
  { 53211, 1, 101, "DOWNSVIEW PARK", 3, "00FF00", "FFFFFF" },
  { 53212, 1, 102, "MARKHAM RD.", 3, "00FF00", "FFFFFF" },
  { 53213, 1, 104, "FAYWOOD", 3, "800080", "FFFFFF" },
  { 53214, 1, 105, "DUFFERIN NORTH", 3, "804000", "FFFFFF" },
  { 53215, 1, 106, "SENTINEL", 3, "00FFFF", "FFFFFF" },
  { 53216, 1, 107, "ST REGIS", 3, "FF00FF", "FFFFFF" },
  { 53217, 1, 108, "DRIFTWOOD", 3, "800080", "FFFFFF" },
  { 53218, 1, 109, "RANEE", 3, "008000", "FFFFFF" },
  { 53219, 1, 11, "BAYVIEW", 3, "FF0000", "FFFFFF" },
  { 53220, 1, 110, "ISLINGTON SOUTH", 3, "804000", "FFFFFF" },
  { 53221, 1, 111, "EAST MALL", 3, "804000", "FFFFFF" },
  { 53222, 1, 112, "WEST MALL", 3, "800080", "FFFFFF" },
  { 53223, 1, 113, "DANFORTH ROAD", 3, "808000", "FFFFFF" },
  { 53224, 1, 115, "SILVER HILLS", 3, "008000", "FFFFFF" },
  { 53225, 1, 116, "MORNINGSIDE", 3, "804000", "FFFFFF" },
  { 53226, 1, 117, "ALNESS-CHESSWOOD", 3, "008000", "FFFFFF" },
  { 53227, 1, 118, "THISTLE DOWN", 3, "800080", "FFFFFF" }
};

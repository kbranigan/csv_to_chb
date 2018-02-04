
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
// #include <string.h>

#include <string>
#include <vector>

std::vector<std::string> get_fields(char * line)
{
  if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = 0;
  if (line[strlen(line)-1] == '\r') line[strlen(line)-1] = 0;
  if (line[strlen(line)-1] == '\n') line[strlen(line)-1] = 0;
  if (line[strlen(line)-1] == '\r') line[strlen(line)-1] = 0;
  std::vector<std::string> ret;
  char * pch = line;
  char * token = line;

  while ((token = strsep(&pch, ",")) != NULL)
    ret.push_back(token);

  return ret;
}

int get_index_of_field(std::vector<std::string> & array, const char * field)
{
  for (int i = 0 ; i < array.size() ; i++)
    if (array[i].compare(field) == 0)
      return i;
  return -1;
}

void scan_past_any_possible_byte_order_marks(FILE * fp)
{
  fpos_t pos;
  fgetpos(fp, &pos);
  int c;
  c = fgetc(fp);
  if (c == 0xEF) // utf-8
  {
    c = fgetc(fp);
    if (c != 0xBB) { fprintf(stderr, "%s: odd byte order mark\n", __func__); fsetpos(fp, &pos); return; }
    c = fgetc(fp);
    if (c != 0xBF) { fprintf(stderr, "%s: odd byte order mark\n", __func__); fsetpos(fp, &pos); return; }
  }
  else if (c == 0xFE) // utf16 big endian
  {
    c = fgetc(fp);
    if (c != 0xFF) { fprintf(stderr, "%s: odd byte order mark\n", __func__); fsetpos(fp, &pos); return; }
  }
  else if (c == 0xFF) // utf16 little endian
  {
    c = fgetc(fp);
    if (c != 0xFE) { fprintf(stderr, "%s: odd byte order mark\n", __func__); fsetpos(fp, &pos); return; }
  }
  else
  {
    fsetpos(fp, &pos);
  }
}

int csv_to_cbh(std::string folder, std::string base_name)
{
  if (folder[folder.size()-1] != '/') folder += '/';
  printf("____________\n%s%s.csv\n", folder.c_str(), base_name.c_str());

  char temp[500];
  sprintf(temp, "%s%s.csv", folder.c_str(), base_name.c_str());
  FILE * fp_in = fopen(temp, "r");
  if (fp_in == NULL) { printf("File doesn't exist.\n"); return 0; }
  sprintf(temp, "%s%s.h", folder.c_str(), base_name.c_str());
  FILE * fp_h = fopen(temp, "w");
  if (fp_h == NULL) { printf("Couldn't open file for saving\n"); return 0; }
  sprintf(temp, "%s%s.c", folder.c_str(), base_name.c_str());
  FILE * fp_c = fopen(temp, "w");
  if (fp_c == NULL) { printf("Couldn't open file for saving\n"); return 0; }
  sprintf(temp, "%s%s.b", folder.c_str(), base_name.c_str());
  FILE * fp_b = fopen(temp, "wb");
  if (fp_b == NULL) { printf("Couldn't open file for saving\n"); return 0; }

  scan_past_any_possible_byte_order_marks(fp_in);

  std::string data_name = base_name;

  if (!isalpha(data_name[0]))
    data_name = "data_" + base_name;

  char header_char[500];
  fgets(header_char, sizeof(header_char), fp_in);
  if (strlen(header_char) == 0) { printf("File has no data in it.\n"); return 0; }
  std::vector<std::string> header = get_fields(header_char);
  
  std::vector<int> field_length; // bytes
  std::vector<int> field_type;   // 0 = char, 1 = int, 2 = float

  for (int i = 0 ; i < header.size() ; i++)
  {
    for (int j = 0 ; j < header[i].size() ; j++)
      if (!isalpha(header[i][j]))
        header[i][j] = '_';
    field_length.push_back(0); // calc in a bit
    field_type.push_back(1);   // 0 = char, 1 = int, 2 = float
  }

  int32_t num_rows = 0;
  char line[500];
  while (fgets(line, sizeof(line), fp_in))
  {
    std::vector<std::string> row = get_fields(line);
    for (int i = 0 ; i < row.size() ; i++)
    {
      for (int j = 0 ; j < row[i].size() ; j++)
        if (!isdigit(row[i][j]) && row[i][j] != '.' && row[i][j] != '-')
          field_type[i] = 0; // char
        else if (field_type[i] == 1 && (row[i][j] == '.' || row[i][j] == '-'))
          field_type[i] = 2; // float

      // make sure floats only have a single . or - (it used to mess up on phone numbers)
      if (field_type[i] == 2)
      {
        int count_dots = 0, count_dashes = 0;
        for (int j = 0 ; j < row[i].size() ; j++)
          if (row[i][j] == '.')      count_dots++;
          else if (row[i][j] == '-') count_dashes++;
        if (count_dots > 1 || 
            count_dashes > 1 || 
            (count_dashes == 1 && row[i][0] != '-')) field_type[i] = 0;
      }

      if (field_length[i] < row[i].size())
          field_length[i] = row[i].size();
    }
    num_rows++;
  }

  int num_fields = 0;
  for (int i = 0 ; i < header.size() ; i++)
    if (field_length[i] != 0)
      num_fields ++;

  fprintf(fp_h, "// generated automatically from %s%s.csv\n", folder.c_str(), base_name.c_str());
  fprintf(fp_h, "\n");
  fprintf(fp_h, "#ifndef GENERATED_AUTOMATICALLY_FROM_%s_CSV\n", base_name.c_str());
  fprintf(fp_h, "#define GENERATED_AUTOMATICALLY_FROM_%s_CSV\n", base_name.c_str());
  fprintf(fp_h, "\n");
  // fprintf(fp_h, "extern int num_fields_%s;\n", base_name.c_str());
  // fprintf(fp_h, "extern char fields_%s[%d][50];\n\n", base_name.c_str(), num_fields);

  fprintf(fp_c, "// generated automatically from %s%s.csv\n", folder.c_str(), base_name.c_str());
  fprintf(fp_c, "\n");
  fprintf(fp_c, "#include \"%s.h\"\n", base_name.c_str());
  fprintf(fp_c, "\n");
  // fprintf(fp_c, "int num_fields_%s = %d;\n", base_name.c_str(), num_fields);
  // fprintf(fp_c, "char fields_%s[%d][50] = {", base_name.c_str(), num_fields);
  // for (int i = 0 ; i < header.size() ; i++)
  //   if (field_length[i] == 0)
  //     fprintf(fp_c, "%s\n// Empty field dropped: \"%s\"", (i==0?"":","), header[i].c_str()); // indicate in the header file the empty fields
  //   else
  //     fprintf(fp_c, "%s\n  \"%s\"", (i==0?"":","), header[i].c_str());
  // fprintf(fp_c, "\n};\n\n");

  int32_t row_width = 0;
  fprintf(fp_h, "struct s_%s {\n", base_name.c_str());
  for (int i = 0 ; i < header.size() ; i++)
  {
    if (field_length[i] == 0) continue;
    field_length[i] += 1;                                  // trailing null
    while (field_length[i] % 4 != 0) field_length[i] ++;   // memory aligned
    if (field_type[i] == 1 || field_type[i] == 2) field_length[i] = 4;

    // printf("%s %d %d %d\n", header[i].c_str(), i, field_type[i], field_length[i]);
    row_width += field_length[i];

    if (field_type[i] == 0)      fprintf(fp_h, "  char  %s[%d];\n", header[i].c_str(), (int)field_length[i]);
    else if (field_type[i] == 1) fprintf(fp_h, "  int   %s;\n", header[i].c_str());
    else if (field_type[i] == 2) fprintf(fp_h, "  float %s;\n", header[i].c_str());
  }

  fprintf(fp_h, "};\n");
  fprintf(fp_h, "\n");

  rewind(fp_in);
  scan_past_any_possible_byte_order_marks(fp_in);
  fgets(header_char, sizeof(header_char), fp_in);

  fwrite(&row_width, sizeof(row_width), 1, fp_b);
  fwrite(&num_rows, sizeof(num_rows), 1, fp_b);

  fprintf(fp_h, "extern struct s_%s * load_%s(int * ret_count);\n", base_name.c_str(), base_name.c_str());
  fprintf(fp_h, "\n");
  #ifdef COMPILE_IN_DATA
  fprintf(fp_h, "#ifdef COMPILE_IN_DATA\n");
  fprintf(fp_h, "extern int num_%s;\n", base_name.c_str());
  fprintf(fp_h, "extern struct s_%s %s[%d];\n", base_name.c_str(), data_name.c_str(), num_rows);
  fprintf(fp_h, "#endif // COMPILE_IN_DATA\n");
  fprintf(fp_h, "\n");
  #endif // COMPILE_IN_DATA
  fprintf(fp_h, "#endif // GENERATED_AUTOMATICALLY_FROM_%s_CSV\n", base_name.c_str());

  fprintf(fp_c, "#include <stdio.h>\n");
  fprintf(fp_c, "#include <stdlib.h>\n");
  fprintf(fp_c, "#include <string.h>\n");
  fprintf(fp_c, "\n");
  fprintf(fp_c, "struct s_%s * load_%s(int * ret_count)\n", base_name.c_str(), base_name.c_str());
  fprintf(fp_c, "{\n");
  fprintf(fp_c, "  char file_name[500];\n");
  fprintf(fp_c, "  snprintf(file_name, sizeof(file_name), \"%s%s.b\");\n", folder.c_str(), base_name.c_str());
  fprintf(fp_c, "  FILE * fp = fopen(file_name, \"rb\");\n");
  fprintf(fp_c, "  int32_t row_width = 0, num_rows = 0;\n");
  fprintf(fp_c, "  fread(&row_width, sizeof(row_width), 1, fp);\n");
  fprintf(fp_c, "  fread(&num_rows, sizeof(num_rows), 1, fp);\n");
  fprintf(fp_c, "  if (row_width != sizeof(struct s_%s) /*%d*/) { fprintf(stderr, \"file row_width (%%d) seems invalid\\n\", row_width); fclose(fp); return 0; }\n", base_name.c_str(), row_width);
  fprintf(fp_c, "  struct s_%s * rows = (struct s_%s *)malloc(sizeof(struct s_%s) * num_rows);\n", base_name.c_str(), base_name.c_str(), base_name.c_str());
  fprintf(fp_c, "  if (rows == NULL) { fprintf(stderr, \"couldnt alloc memory for file data\\n\"); fclose(fp); return 0; }\n");
  fprintf(fp_c, "  int32_t ret_num_rows = fread(rows, row_width, num_rows, fp);\n");
  fprintf(fp_c, "  if (ret_num_rows != num_rows) { fprintf(stderr, \"couldnt read all of the file data (%%d vs %%d)\\n\", ret_num_rows, num_rows); fclose(fp); return 0; }\n");
  fprintf(fp_c, "  fclose(fp);\n");
  fprintf(fp_c, "  *ret_count = num_rows;\n");
  fprintf(fp_c, "  return rows;\n");
  fprintf(fp_c, "}\n");
  fprintf(fp_c, "\n");
  #ifdef COMPILE_IN_DATA
  fprintf(fp_c, "#ifdef COMPILE_IN_DATA\n");
  fprintf(fp_c, "int num_%s = %d;\n", base_name.c_str(), num_rows);
  fprintf(fp_c, "struct s_%s %s[%d] = {", base_name.c_str(), data_name.c_str(), num_rows);
  #endif // COMPILE_IN_DATA

  int count_rows = 0;
  while (fgets(line, sizeof(line), fp_in))
  {
    std::vector<std::string> row = get_fields(line);
    #ifdef COMPILE_IN_DATA
    fprintf(fp_c, "%s\n  {", (count_rows==0?"":","));
    #endif // COMPILE_IN_DATA
    for (int i = 0, j = 0 ; i < row.size() ; i++)
    {
      if (field_length[i] == 0) continue;
      if (field_type[i] == 0)
      {
        const char * cell = row[i].c_str();
        #ifdef COMPILE_IN_DATA
        fprintf(fp_c, "%s \"%s\"", (j==0?"":","), cell);
        #endif // COMPILE_IN_DATA
        fwrite(cell, field_length[i], 1, fp_b);
      }
      else if (field_type[i] == 1)
      {
        int32_t cell = atoi(row[i].c_str());
        #ifdef COMPILE_IN_DATA
        fprintf(fp_c, "%s %d", (j==0?"":","), cell);
        #endif // COMPILE_IN_DATA
        fwrite(&cell, field_length[i], 1, fp_b);
      }
      else if (field_type[i] == 2)
      {
        float cell = atof(row[i].c_str());
        #ifdef COMPILE_IN_DATA
        fprintf(fp_c, "%s %f", (j==0?"":","), cell);
        #endif // COMPILE_IN_DATA
        fwrite(&cell, field_length[i], 1, fp_b);
      }

      j++;
    }

    #ifdef COMPILE_IN_DATA
    fprintf(fp_c, " }");
    #endif // COMPILE_IN_DATA

    count_rows++;
    
    if (count_rows >= num_rows) break;
  }
  #ifdef COMPILE_IN_DATA
  fprintf(fp_c, "\n};\n");
  fprintf(fp_c, "#endif // COMPILE_IN_DATA\n");
  #endif // COMPILE_IN_DATA

  fclose(fp_in);
  fclose(fp_h);
  fclose(fp_c);
  fclose(fp_b);

  printf("%s%s.csv: %d fields, %d rows\n", folder.c_str(), base_name.c_str(), num_fields, num_rows);
  printf("------------\n");
  printf("run this to test:\n");
  printf("cc -c -O3 -DCOMPILE_IN_DATA %s%s.c -o %s%s.o\n", folder.c_str(), base_name.c_str(), folder.c_str(), base_name.c_str());

  return num_rows;
}

int main(int argc, char ** argv)
{
  if (argc == 3)
  {
    csv_to_cbh(argv[1], argv[2]);
  }
  else
  {
    printf("Usage Example: %s . routes   ; [dir name] [base file name]\n", argv[0]);
    exit(EXIT_FAILURE);
  }
}

















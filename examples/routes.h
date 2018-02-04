// generated automatically from /Users/kevin/work/csv_to_chb/examples/routes.csv

#ifndef GENERATED_AUTOMATICALLY_FROM_routes_CSV
#define GENERATED_AUTOMATICALLY_FROM_routes_CSV

struct s_routes {
  int   route_id;
  int   agency_id;
  int   route_short_name;
  char  route_long_name[28];
  int   route_type;
  char  route_color[8];
  char  route_text_color[8];
};

#ifdef __cplusplus
extern "C" {
#endif
extern struct s_routes * load_routes(int * ret_count);

extern int num_routes;
extern struct s_routes routes[20];

#ifdef __cplusplus
}
#endif

#endif // GENERATED_AUTOMATICALLY_FROM_routes_CSV

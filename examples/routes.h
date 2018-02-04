// generated automatically from ./routes.csv

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

extern struct s_routes * load_routes(int * ret_count);

#ifdef COMPILE_IN_DATA
extern int num_routes;
extern struct s_routes routes[20];
#endif // COMPILE_IN_DATA

#endif // GENERATED_AUTOMATICALLY_FROM_routes_CSV

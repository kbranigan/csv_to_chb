
#include "routes.h"
#include <stdio.h>

int main()
{
  int num_routes = 0;
  struct s_routes * routes = NULL;
  routes = load_routes(&num_routes);
  printf("num_routes = %d\n", num_routes);

  for (int i = 0 ; i < num_routes ; i++)
  {
    printf("%d: %s\n", i, routes[i].route_long_name);
  }

  printf("done\n");
}

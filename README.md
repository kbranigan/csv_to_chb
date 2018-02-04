

It might not sound too useful but it's just great.

Think of this:

Say you want to start working with GTFS - the transit feed spec.<br />
The stop_times.txt file is 197MB at the moment.<br />
Parsing a file that large can take a good 10 seconds at least. (With converting strings to ints or floats etc)<br />
With all the additional computation you'll be doing with the data, that's just too much time, especially when your developing.<br />

This tool will scan a csv file, reading each field and testing to see if they are int, float, or char - in which case it tests for the largest length.<br />

It creates a struct that can represent each row (memory aligned), writes C code of that content which can be compiled - also it writes the data to a binary file which can be paged into ram.<br />
It's extremely dumb (which means extremely fast), it can not handle quotes, line breaks, comments, different encodings or files without a header.

If you want any of _those_ features, take a look at: https://github.com/kbranigan/kcsv

Copy/Paste command to compile and test code and examples: (compiles generated examples/routes.c and sample code that reads routes.b)
```
make test
or
make test_embedded
```

Example:

<b>routes.csv</b>

```
route_id,agency_id,route_short_name,route_long_name
53209,1,10,VAN HORNE
53210,1,100,FLEMINGDON PARK
53211,1,101,DOWNSVIEW PARK
53212,1,102,MARKHAM RD.
53213,1,104,FAYWOOD
```

<b>After you run:</b>

```
./csv_to_chb . routes
```

You get the following files:

<b>routes.h</b>
<b>routes.c</b>
<b>routes.b</b>

Creating a struct that looks like:
```
struct s_routes {
  int   route_id;
  int   agency_id;
  int   route_short_name;
  char  route_long_name[16];
}
```

A helpful loading function:

```
struct s_routes * load_routes(int * ret_count);
```

Which will load the binary file - or if you compile with -DCOMPILE_WITH_DATA

Then you'll get the globals (when you #include "routes.h"):

```
int num_routes = 20;
struct s_routes routes[20] = {
  { 53209, 1, 10, "VAN HORNE", 3, "804000", "FFFFFF" },
  { 53210, 1, 100, "FLEMINGDON PARK", 3, "FF00FF", "FFFFFF" },
  { 53211, 1, 101, "DOWNSVIEW PARK", 3, "00FF00", "FFFFFF" },
  { 53212, 1, 102, "MARKHAM RD.", 3, "00FF00", "FFFFFF" },
  { 53213, 1, 104, "FAYWOOD", 3, "800080", "FFFFFF" }
};
```

You can compile routes.h using the command:

```
cc examples/routes.c -c -o examples/routes.o
```

then to use it in your code:

```
#include "routes.h"
int num_routes = 0;
struct s_routes * routes = NULL;
routes = load_routes(&num_routes);
```

or with data embedded:  (can be slow for linking)

```
cc -DCOMPILE_IN_DATA examples/routes.c -c -o examples/routes.o
```

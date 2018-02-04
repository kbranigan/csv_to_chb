all: csv_to_cbh csv_to_cbh_with_data_embedded

csv_to_cbh: csv_to_cbh.cpp
	g++ -O3 -std=c++11 $^ -o $@

csv_to_cbh_with_data_embedded: csv_to_cbh.cpp
	g++ -O3 -std=c++11 -DCOMPILE_IN_DATA $^ -o $@

examples/test_routes:
	cc -O3 examples/routes.c -c -o examples/routes.o
	cc -O3 $^ examples/routes.o $@.c -o $@

examples/test_routes_embedded:
	cc -O3 examples/routes.c -DCOMPILE_IN_DATA -c -o examples/routes.o
	cc -O3 -DCOMPILE_IN_DATA $^ examples/routes.o $@.c -o $@

test: csv_to_cbh
	./csv_to_cbh `pwd`/examples/ routes
	make -B examples/test_routes
	./examples/test_routes

test_embedded: csv_to_cbh_with_data_embedded
	./csv_to_cbh_with_data_embedded `pwd`/examples/ routes
	make -B examples/test_routes_embedded
	./examples/test_routes_embedded

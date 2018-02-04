all: csv_to_cbh csv_to_cbh_nodata_in_c

csv_to_cbh: csv_to_cbh.cpp
	g++ -std=c++11 -DCOMPILE_IN_DATA $^ -o $@

csv_to_cbh_nodata_in_c: csv_to_cbh.cpp
	g++ -std=c++11 $^ -o $@

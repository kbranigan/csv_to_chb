all: csv_to_cbh csv_to_cbh_with_data_embedded

csv_to_cbh: csv_to_cbh.cpp
	g++ -std=c++11 $^ -o $@

csv_to_cbh_with_data_embedded: csv_to_cbh.cpp
	g++ -std=c++11 -DCOMPILE_IN_DATA $^ -o $@

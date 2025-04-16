export DYLD_LIBRARY_PATH=./
#./simplelog_cpp_sample --nthread=10 --loopcount=1000000
mv trigger_master.txt trigger_master_.txt
./simple_cpp_example_direct --is_master=1

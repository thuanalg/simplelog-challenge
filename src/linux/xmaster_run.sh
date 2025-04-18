mv trigger_master.txt trigger_master_.txt
export LD_LIBRARY_PATH=./
./simple_cpp_example --is_master=1 --cfg=z.cfg

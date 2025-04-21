#Note, you ONLY this file with process_mode=1
export LD_LIBRARY_PATH=./
./simple_c_fork --is_master=0 --loopcount=1000000 --cfg=z.cfg

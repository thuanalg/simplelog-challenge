#Note, you ONLY this file with process_mode=1
export LD_LIBRARY_PATH=./
./simple_c_fork_deadlock --is_master=0 --loopcount=1 --cfg=z.cfg --nthread=1 --topic_index=0

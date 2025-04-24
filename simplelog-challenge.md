# SimpleLog-Challenge

**High-Performance, Cross-Platform Logging Library for C/C++**



Author: [Thuan Nguyen Thai](mailto:nguyenthaithuanalg@gmail.com)  
Other contributors: None  
Created: April 24, 2025  


  
  

  
### Problem Statement  
Providing a logger which must be SIMPLE-STABLE-POWERFUL for embedded devices to PC and mainframe with flexible configuration.  
Multi-process C/C++ logger.  
  
  
### Requirements
- POSIX thread in UNIX-Like.  
- Win32 APIs in Windows.  

### Background and References  
- No.  

### Motivation
SimpleLog-Challenge is designed to provide:
- Simplicity.
- Multi-thread and multi-process safe logging.
- Portability across Windows, Linux, macOS, ...
- Compatibility from ANSI C89 to C++20.
- Zero external dependencies.
- Extremely high performance (1.1M+ msg/sec) even up to 3M+ msg.sec (may reach limitation of SSD [450MB/500MB](https://github.com/thuanalg/simplelog-challenge/blob/main/src/linux/fork_test.txt) in a few cases) on PC-Linux.
- Stable up to billions of records.
- Message safety.
- Nano sec accuracy.

It is suitable on most of platforms I know, especially with high precision as openBMC.

  
### 🧩 Proposed Design


| Component | Description |
|-----------|-------------|
| `spc_written_thread_routine` | **[Mandatory]** The main thread responsible for writing log data to the file or output stream. |
| `spc_trigger_routine` | **[Optional]** A secondary thread that can be enabled via configuration to trigger log flushing, [trigger=](https://github.com/thuanalg/simplelog-challenge/blob/main/src/simplelog-challenge.cfg). |
  
  
### 🛠️ Exported APIs

SimpleLog-Challenge provides 5 key APIs for initializing, logging, and process control:

| APIs | Description |
|--------------------|-------------|
| `int spc_init_log_ext(SPC_INPUT_ARG *input);` | **[Mandatory]** Initialize the logger with extended input settings. |
| `int spc_finish_log();` | **[Mandatory]** Finalize and clean up the logger. |
| `spclog(log_level, fmt, ...);` | **[Mandatory]** Log a message with the specified level. |
| `spclogtopic(log_level, topic_index, fmt, ...);` | **[Optional]** Log by [topics=](https://github.com/thuanalg/simplelog-challenge/blob/main/src/simplelog-challenge.cfg) from 1 to n. |
| `LLU spc_update_processid();` | **[Optional]** Update process ID, typically used after `fork()` [example](https://github.com/thuanalg/simplelog-challenge/blob/main/tests/fork/main.c). |


### Alternatives Considered
- No. Give one more option.

### Impact
No.  

### Organizational
- Does this proposal require a new repository? No.  
- Which repositories are expected to be modified to execute this design? [github](https://github.com/thuanalg/simplelog-challenge)  

### Testing and Validation
- [Benchmarks on Windows, Linux, macOS vs spdlog](https://github.com/thuanalg/simplelog-challenge/blob/x1/README.md#benchmarking-performance)
- [1B record stress test Windows](https://github.com/thuanalg/simplelog-challenge/blob/main/performance/250113-1billion-multi-processes.txt)
- [1B record stress test Linux](https://github.com/thuanalg/simplelog-challenge/blob/main/performance/250217-CentOS-09-performance-8-Core-1Billion.txt)
- However, you can try with [Benchmarking Linux 8 cores](https://github.com/thuanalg/simplelog-challenge/blob/main/performance/250217-CentOS-09-performance-8-Core.txt) first ( find **How to run**). And do same with Windows/MAC OSX.  

### References
- [UNIX Network Programming, Volume 2: Interprocess Communications, Second Edition](https://www.amazon.com/UNIX-Network-Programming-Interprocess-Communications/dp/0130810819)  
- [Unix Network Programming: The Sockets Networking API](https://www.amazon.com/Unix-Network-Programming-Sockets-Networking/dp/0131411551) 

---


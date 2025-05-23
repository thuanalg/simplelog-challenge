### **SimpleLog-Challenge: A Fast, Lightweight, and Flexible C/C++ Logger (Introduction written by ChatGPT)**


Logging is an essential part of any software system, helping developers gain insights into application behavior, debug issues, and monitor production systems. However, many logging libraries often introduce unnecessary complexity and performance overhead. **SimpleLog-Challenge** was designed to address these issues, offering a **fast, efficient, and easy-to-use logging solution** for both **C** and **C++** applications. 

**SimpleLog-Challenge** is built to support **C89** through to **C++20**, making it a versatile choice for a wide variety of projects. Whether you're working with legacy C code or using modern C++ features, SimpleLog-Challenge integrates seamlessly into your application. This logger is not just about speed—it's about **simplicity, performance**, and **flexibility**, designed with both **embedded systems** and **large-scale applications** in mind.

At its core, SimpleLog-Challenge follows the **Unix Philosophy** of **KISS (Keep It Simple, Stupid)** and aims to be a tribute to the incredible work of **W. Richard Stevens**, whose **Unix Network Programming** books and contributions have profoundly influenced systems programming and network design. This tool is built with his principles of **clarity**, **elegance**, and **efficiency** in mind, ensuring that developers can achieve powerful logging without unnecessary complexity or bloat.

### **Key Features of SimpleLog-Challenge**

1. **Universal Compatibility: From C89 to C++20**  
   SimpleLog-Challenge is compatible with both **C89** (ANSI C) and modern **C++20**, making it an ideal logging solution for a wide range of applications, whether they are legacy C projects or cutting-edge C++ systems.

2. **Cross-Platform Support**  
   SimpleLog-Challenge works across multiple platforms, including **Windows**, **Linux**, **macOS**, **Android**, and various **embedded systems**. This ensures your logging infrastructure is consistent regardless of the underlying operating system or hardware architecture.

3. **Optimized for Both Multithreading and Multi-process**  
	With support for both multithreaded and **multi-process** logging, SimpleLog-Challenge performs exceptionally well in high-concurrency environments. It allows multiple **threads/processes** to log messages concurrently without significant performance degradation, making it ideal for both small-scale and distributed systems.

4. **Memory-Efficient and Configurable**  
   SimpleLog-Challenge is highly configurable, allowing developers to adjust memory usage according to the system’s resources. Whether running on **low-memory embedded devices** or **high-performance servers**, it can be optimized for minimal resource consumption.

5. **Nanosecond Precision for Timestamps**  
   SimpleLog-Challenge provides **nanosecond-level timestamp precision**, essential for applications that require **high-precision timing**, such as **real-time systems**, **network applications**, and **performance profiling**.

6. **Superior Performance**  
   SimpleLog-Challenge is engineered to be extremely fast and efficient. Its performance has been carefully benchmarked to ensure minimal overhead and **high throughput** under demanding conditions.

7. **No dependence**  
   Just use POSIX APIs and Win32 APIs.

8. **Message safety feature**  
   The **message safety feature** in your logger allows users to configure a size limit for each log message (e.g., 10,000 bytes). If a message exceeds this size, it, sometimes, **may be truncated**, but the first 10,000 bytes will still be logged. This ensures that large messages do not cause memory overflow or uncontrolled log writing, while still preserving the beginning of the message for logging. This feature is particularly useful in resource-constrained environments like embedded devices, providing both flexibility and safety in log management. [max_sz_msg](https://github.com/thuanalg/simplelog-challenge/blob/main/src/simplelog-challenge.cfg)
   
### **Performance Highlights: Speed and Efficiency**

One of the core objectives of SimpleLog-Challenge is to provide exceptional **performance** while maintaining **simplicity**. Here’s a detailed look at its performance in different scenarios:

#### **Benchmarking Performance**

1. **Windows10/MSVC 2022 with 10 Threads and 10 Million Records:**

   - **SimpleLog-Challenge:** **15 seconds** [Win10 - 8 cores - 10M records](https://github.com/thuanalg/simplelog-challenge/blob/main/performance/250113-MSVC-2022-performance-multi-processes.txt)
   - **spdlog:** **66 seconds**

   In this test, **SimpleLog-Challenge** logged **10 million records** in just **16 seconds**, compared to **66 seconds** taken by **spdlog**. This demonstrates **4x faster performance** in multithreaded environments, showing that SimpleLog-Challenge efficiently manages logging tasks without unnecessary performance bottlenecks.

2. **Linux Performance Benchmark (10 Threads and 10 Million Records), VM CentOs9:**  
	
	**With 4 CPUs / VM:**	
	- **SimpleLog-Challenge: ~12.5 seconds** [VM-CentOS-09 - 4 cores - 10 threads - 10M records](https://github.com/thuanalg/simplelog-challenge/blob/main/performance/250113-LinuxCentOs09-4CPU-multi-processes.txt)
	- **spdlog: ~15 seconds**  
   In this environment simplelog-topic is faster at **2.5s (~20%)** than spdlog.   
	
	**With 8 CPUs / PC:**	
	- **SimpleLog-Challenge: ~3.358 seconds** [CentOS-09 - 8 cores](https://github.com/thuanalg/simplelog-challenge/blob/main/performance/250217-CentOS-09-performance-8-Core.txt)
	- **spdlog: ~7.051 seconds**  
   In this environment simplelog-topic is faster at **3.72s (~110%, 2x)** than spdlog.
	
	**With 8 CPUs / PC with fork() 20/40 threads (spdlog doesn't support multi-PROCESS with logging to file):**	
	- **20 threads 20M records: ~5.2 seconds** [CentOS-09 - 8 cores - 20 threads - 20M records](https://github.com/thuanalg/simplelog-challenge/blob/main/src/linux/fork_test.txt)
	- **40 threads 40M records: ~10. seconds** [CentOS-09 - 8 cores - 40 threads - 40M records](https://github.com/thuanalg/simplelog-challenge/blob/main/src/linux/fork_test.txt)

3. **Large-Scale Logging Test (1 Billion Records, 10 Threads):**

	**Windows 10/PC CPU - 8cores:** 
   - **Time Taken:** **31 minutes** (**1860 seconds**)
   - **Log Size:** **Size on disk: 102 GB (110,389,039,104 bytes)**: [1 Billion - Windows 10 - 8 cores](https://github.com/thuanalg/simplelog-challenge/blob/main/performance/250113-1billion-multi-processes.txt)


	**CentOS 09/PC CPU - 8cores:** 
   - **Time Taken:** **19 minutes** (**1140 seconds**) 
   - **Log Size:** **Size on disk: 113GB (120,888,922,262 bytes)**: [1 Billion - Linux CentOS 09 - 8 cores](https://github.com/thuanalg/simplelog-challenge/blob/main/performance/250217-CentOS-09-performance-8-Core-1Billion.txt)

   In an extreme scenario, Linux is better Windows. More CPUs more effective.

4. **OSX/Apple clang version 11.0.3 (clang-1103.0.32.29) with 10 Threads and 10 Million Records (Total Number of Cores: 2, Model Name: Mac mini(Late 2012)):**

   - **SimpleLog-challenge: 35 seconds** 
   - **spdlog: 26 seconds** [MAC OSX - 2 cores](https://github.com/thuanalg/simplelog-challenge/blob/main/performance/250205-MacOSX-performance-2-Core.txt) 

   In this environment **spdlog** is faster at **9s (~35%)** than **simplelog-challenge**. However, I shall re-verify with PC with more CPU cores.


#### **Optimized for Low Latency and High Throughput**

- **Minimal Latency:**  
  SimpleLog-Challenge is designed to minimize latency in logging, even when the system is under heavy load. Whether you’re dealing with **real-time systems** or large volumes of logs, SimpleLog-Challenge ensures fast log writing with minimal delay. [simplelog-challenge.cfg](https://github.com/thuanalg/SimpleLog-Challenge/blob/main/src/simplelog-challenge.cfg)

- **Low Memory Usage:**  
  The library is optimized to use **minimal memory**. Memory buffers are dynamically adjusted based on the available system resources, allowing SimpleLog-Challenge to be used in **resource-constrained embedded systems** without consuming unnecessary memory.[simplelog-challenge.cfg](https://github.com/thuanalg/SimpleLog-Challenge/blob/main/src/simplelog-challenge.cfg)

- **Asynchronous Logging Support:**  
  SimpleLog-Challenge offers **asynchronous logging** capabilities, allowing logs to be written in the background without blocking the main application flow. This ensures that logging does not interfere with the critical performance of the application.

- **Log Rotation and Buffering:**  
  With configurable **log rotation** and **buffering** options, SimpleLog-Challenge can handle **large volumes of log data** efficiently without causing performance degradation. You can control when log files are rotated or how much data should be kept in memory at any given time.

### **Why SimpleLog-Challenge?**

- **Performance-Focused:** SimpleLog-Challenge provides **fast, low-latency logging** without sacrificing efficiency, even under heavy load or in high-concurrency applications.
  
- **Flexible and Configurable:** It adapts to various system environments, from **small embedded devices** with **limited memory** to **high-performance mainframes**. You can configure buffer sizes, logging levels, and memory usage according to your system’s needs.

- **Scalable for Large-Scale Systems:** Whether you’re processing millions or billions of log records, SimpleLog-Challenge scales seamlessly without compromising performance.

- **Simple and Easy to Use:** The **minimalistic API** keeps things simple, adhering to the Unix philosophy of **KISS**, making it easy to integrate and maintain.

### **A Tribute to W. Richard Stevens (1951 - 1999)**

**W. Richard Stevens** was a pioneer in the field of **network programming** and **systems design**, best known for his influential work, *Unix Network Programming*. His writing, characterized by clarity, precision, and practical insight, has shaped the way developers approach system-level programming. **SimpleLog-Challenge** is inspired by Stevens' philosophy of simplicity, efficiency, and clarity.

By following the **Unix philosophy** of **KISS**, SimpleLog-Challenge strives to provide developers with a straightforward, efficient logging tool that doesn’t get in the way of your application. It’s designed to be **fast**, **minimal**, and **reliable**—exactly the qualities Stevens emphasized in his work. 

This tool is a **tribute to his legacy**—a small way to express **gratitude** for the profound impact his work has had on the software development community. We honor **Richard Stevens** not only through the design of SimpleLog-Challenge but also by continuing his tradition of writing tools that are **practical**, **elegant**, and **powerful**.

### **Use Cases for SimpleLog-Challenge**

- **Embedded Systems**: With low memory consumption and flexible configuration options, SimpleLog-Challenge is an excellent choice for **embedded devices** with limited resources.
  
- **Multithreaded/Multi-process and Distributed Systems**: SimpleLog-Challenge handles **high-concurrency environments** efficiently, making it ideal for **multithreaded/Multi-process applications** and **distributed systems** where logging overhead needs to be minimized.

- **Real-Time Systems**: For **real-time applications** that require precise timestamps and low-latency logging, SimpleLog-Challenge’s **nanosecond precision timestamps** are crucial for accurate tracking and performance monitoring.

### **Conclusion**

**SimpleLog-Challenge** is more than just a logging library; it is a tribute to **Richard Stevens** and his dedication to clear, efficient systems programming. By following the **Unix Philosophy** of **KISS**, it provides a **fast**, **lightweight**, and **scalable** logging solution that is easy to integrate and maintain. Its **superior performance**, **multithreaded optimizations**, and **minimal resource consumption** make it an ideal choice for both **small embedded devices** and **large-scale applications**.

**SimpleLog-Challenge** isn't just about logging—it's about building a better, more efficient foundation for your application, with a tool that allows you to focus on what's important: your core functionality.

**Open-Source on GitHub**  [SimpleLog-Challenge](https://github.com/thuanalg/SimpleLog-Challenge/)
SimpleLog-Challenge is open-source and available on GitHub. Feel free to contribute, modify, or integrate it into your projects, and be part of a community that values **simplicity**, **performance**, and the principles of **Unix**.

By using **SimpleLog-Challenge**, you’re not just choosing a powerful logging solution—you’re embracing the legacy of **Richard Stevens**, honoring his work in **Unix network programming**, and following his principles of creating clear, efficient, and practical tools.

---


**APIs - Focus on Simplicity and Ease of Use**:  
  - **4 simple APIs** for usage:  
    - **Initialization** (at the start of `main`):  
	    		    - `spc_init_log_ext(SPC_INPUT_ARG *input)`  
    - **Logging** (in any thread):  
	    	    - `spclog(level, fmt, ...)`  
	    	    - **Levels**: `SPC_LOG_DEBUG`, `SPC_LOG_INFO`, `SPC_LOG_WARNING`, `SPC_LOG_ERROR`, `SPC_LOG_FATAL`  
    - **Topic-based Logging**:  
	    	    - `spclogtopic(level, topic_index, fmt, ...)`  
    - **Finalization** (at the end of `main`):  
		   - `spc_finish_log()`  


---
  
**Build and install with cmake**:	  
	- Install **cmake**.     
	- Come to root folder, **mkdir build && cd build**.  
	- For windows-32: **cmake .. -G "Visual Studio 17 2022" -A win32**.  
	- For windows-64: **cmake .. -G "Visual Studio 17 2022"**.  
	- For Linux: **cmake .. -DUNIX_LINUX=1**.  
	- For MAC-OSX/Apple: **cmake .. -DUNIX_LINUX=1 -DMACOSX=1**.  
	- UNIX - build/install: **make; sudo make install**.  
	  
   
  
**Video Install/Demo ( old way):**  
	- For [Windows 10 64bit](https://drive.google.com/file/d/1gccgtO84U7_R9a1WuPcLnT3Iw7hBN7Wr/view?usp=sharing)  
	- For [VM Linux CentOs9 64bit](https://drive.google.com/file/d/1a2OWwSoa1bIbzYvhhoFKDAhS3eTXkPv9/view?usp=sharing)  
	- For MacOSX 64bit, come to **src/mach**, and do similar steps of Linux.  
	- If you want to test with both processes and threads, or fork please see: [fork_test.txt](https://github.com/thuanalg/simplelog-challenge/blob/main/src/linux/fork_test.txt).

---
**Thanks a lot to:**

1. My family, friends.
2. My colleagues: Lê Duy Cường, Bùi Khánh Duy, Nguyễn Công Đức , ....
3. Jan Flik from Intel - https://www.intel.com.  

---

Reference & Dedication:
   - [UNIX Network Programming, Volume 2: Interprocess Communications, Second Edition](https://www.amazon.com/UNIX-Network-Programming-Interprocess-Communications/dp/0130810819)  
   - [Unix Network Programming: The Sockets Networking API](https://www.amazon.com/Unix-Network-Programming-Sockets-Networking/dp/0131411551)  

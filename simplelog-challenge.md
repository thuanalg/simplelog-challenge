**SimpleLog-Challenge: High-Performance, Cross-Platform Logging Library**

### Problem Statement
Providing a logger which must be SIMPLE-STABLE-POWERFUL for embedded devices to PC and mainframe with flexible configuration.

### Motivation
SimpleLog-Challenge is designed to provide:
- Simplicity.
- Multi-thread and multi-process safe logging.
- Portability across Windows, Linux, macOS, ...
- Compatibility from ANSI C89 to C++20.
- Zero external dependencies.
- Extremely high performance (1.1M+ msg/sec) even up to 3M+ msg.sec (may reach limitation of SSD 450MB/500MB in a few cases) on PC-Linux.
- Stable up to billions of records.
- Message safety.
- Nano sec accuracy.

It is suitable on most of platforms I know, especially with high precision as openBMC.

### Design Details
- Core written in ANSI C (optionally usable in C++)
- Uses native APIs: POSIX thread (Unix-Like), `Win32` APIs (Windows)
- Supports topics, log levels, rotation, asynchronous logging
- High configurability (buffer size, CPU-based tuning)
- Performance surpasses **spdlog** by 2x–4x in benchmarked environments.
- Successfully logged 1 billion records (~113GB) in ~30 minutes on 12-year-old hardware

### Alternatives Considered
- No. Give one more option.

### Impact
This does not replace old libraries, because it needs time to verify, but is really worthy for trying new applications.

### Testing and Validation
- Benchmarks on Windows, Linux, macOS vs spdlog
- 1B record stress test
- Verified performance and integrity across threads and processes
- **Benchmarking Performance evidence**: https://github.com/thuanalg/simplelog-challenge/blob/x1/README.md#benchmarking-performance. However, you can try with https://github.com/thuanalg/simplelog-challenge/blob/main/performance/250217-CentOS-09-performance-8-Core.txt first.  

### References
- [GitHub: SimpleLog-Challenge](https://github.com/thuanalg/simplelog-challenge)

---


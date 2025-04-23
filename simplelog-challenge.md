**SimpleLog-Challenge: High-Performance, Cross-Platform Logging Library**

### Problem Statement
Existing logging systems in from embedded devices to PC and mainframe with flexiable configuration and extremely high-throughput multi-thread/multi-process scenarios.

### Motivation
SimpleLog-Challenge is designed to provide:
- Dedicate to W. Richard Stevens (1951 - 1999) by his books and legacy.
- Simple.
- Multi-thread and multi-process safe logging
- Portability across Windows, Linux, macOS, ...
- Compatibility from ANSI C89 to C++20
- Zero external dependencies
- Extremely high performance (1.1M+ msg/sec) even up to 3M+ msg.sec (may reach limitation of SSD 450MB/500MB in a few cases) on PC-Linux.
- Stable up to billions of records.
- Safety by avoiding oversize messages.
- Nano sec accuracy.

This makes it suitable for use in OpenBMC platforms where performance and simplicity are critical, especially in resource-constrained or legacy environments.

### Design Details
- Core written in ANSI C (optionally usable in C++)
- Uses native APIs: POSIX thread (Unix-Like), `Win32` APIs (Windows)
- Supports topics, log levels, rotation, asynchronous logging
- High configurability (buffer size, CPU-based tuning)
- Performance surpasses spdlog by 2x–4x in benchmarked environments
- Successfully logged 1 billion records (~113GB) in ~30 minutes on 12-year-old hardware

### Alternatives Considered
- No. Give one more option.

### Impact
This could serve as a replacement or optional module for logging in low-resource systems, high-throughput apps, or when cross-platform support is needed.

### Testing and Validation
- Benchmarks on Windows, Linux, macOS vs spdlog
- 1B record stress test
- Verified performance and integrity across threads and processes
- Evidence: README.md(https://github.com/thuanalg/simplelog-challenge/blob/main/README.md) -> **Benchmarking Performance**

### References
- [GitHub: SimpleLog-Challenge](https://github.com/thuanalg/simplelog-challenge)

---


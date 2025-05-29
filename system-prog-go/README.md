### Chapter 1

- What is system applications ?
    - Software that is used by other software, not directly by the final user.
    - The software is hardware aware and oriented toward performance.
- APIs are series subroutine definitions, communication protocols, and tools for building software.
- Types of APIs
    - Operating systems
    - Libraries and frameworks
    - Remote APIs
    - Web APIs (REST APIs, SOAP APIs)
- Protection Ring
- Kernal space and user space
    - Applicat at user space (level 3) & OS and kernel task at kernel space - more privilege
- System calls are the way operating systems provide access to the resources for the applications.
- OS system calls category
    - File management
    - Device management
    - Information maintenance
    - Communication
- POSIX standards
    - Portable Operating System Interface
    - POSIX.1 - core services
        - Process creation and control
        - Signals
        - File and directory operations
        - Pipes
        - C library
        - I/O port interface and control
        - Process triggers
    - POSIX.1b and POSIX.1c - realtime and thread extensions
        - Priority scheduling, Real-time signals, Clocks and timers, Semaphores, Message passing, Shared memory, Async / sync I/O, Memory locking interface
        - Thread creation, control and cleanup
        - Thread scheduling
        - Thread synchronization
        - Signal handling
    - POSIX.2 - shell and utilities - command line

### Chapter 2
- 
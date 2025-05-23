## Socket Programming

server flow:
    - socket -> bind -> listen -> accept -> (recv / send) -> close
client flow:
    - socket -> connect -> (recv / send) -> close

- socket
    - TCP, UDP, IPv4, IPv6
- bind
    - bind attaches raw socket to ip and port so that OS knows where to listen for incoming connections
- byte order (network byte order, big endian) vs. host order ()
- select() lets you monitor multiple file descriptors (FDs)
    - lightweight and scalable
    - Handles short bursts of requests efficiently
- epoll is a Linux-only I/O event notification system

| Concept                      | What it does                               |
| ---------------------------- | ------------------------------------------ |
| `epoll_create1()`            | Initializes the epoll instance             |
| `epoll_ctl(ADD)`             | Adds socket to be monitored                |
| `epoll_wait()`               | Blocks until an event (EPOLLIN)            |
| `make_socket_non_blocking()` | Avoids blocking in `accept()` and `recv()` |
| `EPOLLIN`                    | Watch for readability                      |
| `EPOLLOUT`, `EPOLLERR`       | Also available for more advanced use       |

- Doesn't loop over all FDs like poll()/select()
- You only get active FDs in epoll_wait()
- Perfect for 10K+ concurrent clients


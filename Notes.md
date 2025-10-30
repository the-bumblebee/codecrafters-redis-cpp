# Handle Concurrent Clients

This can be done in 2 ways:

1. Threads - Each client socket will be handled by a thread.
2. Event Loop - Using `select()`, `poll()` or `epoll()` to check the readiness of the sockets to prevent blocking.

## Event Loop

- `select()` is inefficient and so this was not used for implementation.
- `poll()` is used for the current implementation. `epoll()` can be used for more efficiency.
- Right now the sockets are blocking. There are rare chances that `accept()`, `recv()`, etc. can cause execution blocking with the current implementation.

**Link:** https://www.ibm.com/docs/en/i/7.1.0?topic=designs-using-poll-instead-select

## Future Considerations

- Use threads for handling sockets instead of the event loop.
- Make the sockets non-blocking.
- Use `epoll()` instead of `poll()`.

# Implement ECHO Command

At this stage, the server should support two commands:
- `PING`
- `ECHO`

**Note:** Commands are case insensitive.

```sh
# Input:
*2\r\n$4\r\nECHO\r\n$3\r\nhey\r\n

#Output:
$3\r\nhey\r\n
```
## Future Considerations

- `RespUtil` class has a single `parse` method which returns the output string based on the input.
- This is to be refactored into 3 classes - `RespParser`, `RespDispatcher`, `RespSerializer`.
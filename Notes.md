# Implementaion Progress
- Initial socket program (PING command), handles one client and just one message. Blocks on `accept()` and `recv()`.
- Enhanced with loop for keeping connection alive and stream messages from 1 client. Blocks on `accept()` and `recv()`.
- Event loop and `poll()` mechanism added to support multiple clients.
- Added support for ECHO command and added a RespUtil class to handle parsing. But implementation is hard to scale.
- Major refactoring, RESP library now has 3 classes - `RespParser`, `RespUtil`, `RespSerializer` to handle parsing.
- Major design overhaul - `RespParser`, `RespDispatcher`, `RespSerializer` classes to handle parsing, processing and serializing. Additional enums and intermediatory structs in `RespUtil`.
  - `RespParser`, `RespDispatcher`, `RespSerializer` are now used through instances (DI) instead of being static.
  - Added `Cache` to store and fetch key-value per connection.

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
- Refactor `main` to segregate socket connection and handling to a network layer/service for improved readability and code separation.

# Design Change - Parser, Dispatcher, Serializer

**Socket (Read) --<Raw Input>--> Parser --<ParseResult>--> Dispatcher --<Response>--> Serializer --<Formatted String>--> Socket (Write)**

- `Cache` stores key-values per connection (map of maps).
- `Cache` has methods that handle per connection actions.
- `main` initializes a single `Cache` instance.
- `RespDispatcher` has a reference to the `Cache` reference for commands like `SET` and `GET`.
- `main` is responsible to initialize and call parser, dispatcher and serializer.
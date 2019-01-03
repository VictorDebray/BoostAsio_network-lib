# MyBoost ASIO - Network Library wrapper

- Asynchronous network communication with [Boost.ASIO](https://www.boost.org/doc/libs/1_69_0/doc/html/boost_asio.html)
- Binary serialization is implemented with [protobuf](https://developers.google.com/protocol-buffers/)

This is a work in progress, more examples will come shortly with documentation.

## Build the project

```
protoc --proto_path=. --cpp_out=. request.proto
cmake .
make && make install

cd bin
./server [PORT]
./client [ADRESS = "127.0.0.1"] [PORT]
```

Stay tuned !
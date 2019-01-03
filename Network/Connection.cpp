//
// Created by Victor Debray on 2018-12-28.
//

#include "Connection.hpp"

namespace myboost {
namespace asio {
Connection::Connection(Sptr<boost::asio::io_service> io_service)
    : ioService_(io_service), socket_(*ioService_) {

}

int Connection::connect(std::string const &ip, const std::string &port) {
  if (isConnected()) {
    close();
  }

  tcp::resolver::query query(ip, port);
  tcp::resolver resolver(*ioService_);
  boost::system::error_code error;
  auto it = resolver.resolve(query, error);

  if (!error) {
    endpoint_ = *it;
    socket_.connect(endpoint_);
    std::cout << "Server IP: " << endpoint_.address()
              << " - Port: " << endpoint_.port() << std::endl;
  } else {
    std::cerr << "Can't resolve IP (" + ip + ") or Port (" + port + ") ("
        + error.message() + ")" << std::endl;
    return -1;
  }
  return 0;
}

void Connection::async_write(const RawBuffer &data,
                             const t_writeHandler &callback) {
  auto f = std::bind(&Connection::onWrite, this, callback,
                     std::placeholders::_1, std::placeholders::_2);
  boost::asio::async_write(socket_, boost::asio::buffer(data), f);
}

void Connection::async_read(RawBuffer &buffer, size_t size,
                            const t_readHandler &callback) {
  auto f = std::bind(&Connection::onRead, this,
                     callback, std::placeholders::_1, std::placeholders::_2);
  boost::asio::async_read(socket_, boost::asio::buffer(buffer, size), f);
}

void Connection::async_read(RawBuffer &buffer, size_t from, size_t size,
                            const t_readHandler &callback) {
  auto f = std::bind(&Connection::onRead, this,
                     callback, std::placeholders::_1, std::placeholders::_2);
  buffer.resize(from + size);
  boost::asio::mutable_buffer buf = boost::asio::buffer(&buffer[from], size);
  boost::asio::async_read(socket_, buf, f);
}

void Connection::setErrorCallback(const t_errorHandler &callback) {
  errorCall_ = callback;
}

bool Connection::isConnected() const {
  return socket_.is_open();
}

void Connection::close() {
  socket_.close();
}

void Connection::onWrite(const t_writeHandler &callback,
                         const boost::system::error_code &ec, size_t size) {
  if (ec) {
    std::cerr << "Connection write" << size << ": " << ec.message() << std::endl;
    errorCall_();
  } else {
    callback(size);
  }
}

void Connection::onRead(const t_readHandler &callback,
                        const boost::system::error_code &ec, size_t size) {
  if (ec) {
    std::cerr << "Connection read " << size << ": " << ec.message() << std::endl;
    errorCall_();
  } else {
    callback(size);
  }
}

boost::asio::ip::tcp::socket &Connection::getSocket() {
  return socket_;
};
}
}

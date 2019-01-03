//
// Created by Victor Debray on 2018-12-28.
//

#include "Accept.hpp"
#include "Connection.hpp"

namespace myboost {
namespace asio {

Accept::Accept()
    : io_service_(std::make_shared<boost::asio::io_service>()), acceptor_(*io_service_) {
}

void Accept::start() {
  io_service_->run();
}

void Accept::stop() {
  io_service_->stop();
}

void Accept::bind(const std::string &addr, uint16_t port) {
  boost::asio::ip::address baddr = boost::asio::ip::address::from_string(addr);
  int one = 1;
  setsockopt(acceptor_.native_handle(), SOL_SOCKET, SO_REUSEADDR, (char *) &one, sizeof(one));
  acceptor_.open(familyFromAddr(baddr));
  acceptor_.bind({baddr, port});
}

void Accept::listen(int backlog) {
  acceptor_.listen(backlog);
}

void Accept::async_accept(const acceptHandler_t &callback) {
  auto sock = std::make_shared<Connection>(io_service_);

  std::function<void(const boost::system::error_code &)> f(
      std::bind(&Accept::onAccept, this, callback, std::placeholders::_1, sock)
  );
  acceptor_.async_accept(sock->getSocket(), f);
}

const Sptr<boost::asio::io_service> &Accept::getIoService() const {
  return io_service_;
}

void Accept::onAccept(const acceptHandler_t &callback,
                      const boost::system::error_code &ec,
                      const std::shared_ptr<Connection> &sock) {
  if (ec)
    std::cerr << "Accept: " << ec.message() << std::endl;
  else
    callback(sock);
  async_accept(callback);
}

boost::asio::ip::tcp Accept::familyFromAddr(const boost::asio::ip::address &addr) const {
  if (addr.is_v4())
    return boost::asio::ip::tcp::v4();
  else if (addr.is_v6())
    return boost::asio::ip::tcp::v6();
  throw std::runtime_error("Address family not recognized");
}
}
}


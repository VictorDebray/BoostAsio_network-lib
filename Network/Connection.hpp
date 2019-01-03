//
// Created by Victor Debray on 2018-12-28.
//

#ifndef _ASIO_SOCKET_HPP_
# define _ASIO_SOCKET_HPP_

# include <iostream>
# include <boost/asio.hpp>
# include "UsingClass.hpp"

namespace myboost {
namespace asio {
class Connection {
 private:
  using tcp = boost::asio::ip::tcp;
  t_errorHandler errorCall_;
//    boost::asio::ip::tcp
//    familyFromAddr(const boost::asio::ip::address &addr) const;

 protected:
  Sptr<boost::asio::io_service> ioService_;
  tcp::socket socket_;
  tcp::endpoint endpoint_;

 public:
  explicit Connection(Sptr<boost::asio::io_service> io_service);
  ~Connection() = default;

  void async_write(const RawBuffer &data,
                   const t_writeHandler &callback);
  void async_read(RawBuffer &buffer, size_t size,
                  const t_readHandler &callback);
  void async_read(RawBuffer &buffer, size_t from, size_t size,
                  const t_readHandler &callback);
  void setErrorCallback(const t_errorHandler &callback);

  tcp::socket &getSocket();

  int connect(std::string const &ip, const std::string &port);
  bool isConnected() const;
  void close();

 protected:
  void onWrite(const t_writeHandler &callback,
               const boost::system::error_code &ec, size_t size);
  void
  onRead(const t_readHandler &callback, const boost::system::error_code &ec,
         size_t size);
};
}
}

#endif /* _ASIO_SOCKET_HPP_ */

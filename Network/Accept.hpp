//
// Created by Victor Debray on 2018-12-28.
//

#ifndef _ASIO_ACCEPT_HPP_
# define _ASIO_ACCEPT_HPP_

# include <memory>

# include <boost/asio.hpp>
# include "Connection.hpp"

namespace myboost {
namespace asio {
class Accept {
  using acceptHandler_t = std::function<void(const Sptr<Connection> &)>;

 protected:
  Sptr<boost::asio::io_service> io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;

 public:
  explicit Accept();

  ~Accept() = default;

 public:
  void start();
  void stop();
  void bind(const std::string &addr, uint16_t port);
  void listen(int backlog);
  void async_accept(const acceptHandler_t &callback);
  const Sptr<boost::asio::io_service> &getIoService() const;

 protected:
  void onAccept(const acceptHandler_t &callback,
                const boost::system::error_code &ec,
                const Sptr<Connection> &sock);
  boost::asio::ip::tcp familyFromAddr(const boost::asio::ip::address &addr) const;
};
}
}


#endif /* _ASIO_ACCEPT_HPP_ */

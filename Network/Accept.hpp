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
/**
 * Class that handles client accept
 */
class Accept {
  using acceptHandler_t = std::function<void(const Sptr<Connection> &)>;

 protected:
  Sptr<boost::asio::io_service> io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;

 public:
  /**
   * Accept constructor
   */
  explicit Accept();
  ~Accept() = default;

 public:
  /**
   * Run io_service
   */
  void start();

  /**
  * Stop io_service
  */
  void stop();

  /**
   * Build endpoint and bind it to socket acceptor
   * @param addr ip address
   * @param port port number
   */
  void bind(const std::string &addr, uint16_t port);

  /**
   * Place acceptor on listening state
   * @param backlog maximum number of clients
   */
  void listen(int backlog);

  /**
   * Async accept
   * @param callback user provided completion callback
   */
  void async_accept(const acceptHandler_t &callback);
  const Sptr<boost::asio::io_service> &getIoService() const;

 protected:
  /**
   * On accept completion callback
   * @param callback
   * @param ec
   * @param sock
   */
  void onAccept(const acceptHandler_t &callback,
                const boost::system::error_code &ec,
                const Sptr<Connection> &sock);

  /**
   * Get protocol family from address (ipv4/ipv6)
   * @param addr
   * @return
   */
  boost::asio::ip::tcp familyFromAddr(const boost::asio::ip::address &addr) const;
};
}
}

#endif /* _ASIO_ACCEPT_HPP_ */

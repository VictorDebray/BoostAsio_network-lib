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
/**
 * Class that wraps asio socket communications
 * Handles and call set error callback
 */
class Connection {
 private:
  using tcp = boost::asio::ip::tcp;
  t_errorHandler errorCall_;

 protected:
  Sptr<boost::asio::io_service> ioService_;
  tcp::socket socket_;
  tcp::endpoint endpoint_;

 public:
  /**
   * Connection constructor
   * @param io_service server io_service
   */
  explicit Connection(Sptr<boost::asio::io_service> io_service);
  ~Connection() = default;

  /**
   * Connects socket to server
   * Note: for client use only, for server connection is established by acceptor
   * @param ip ip address to connect to server
   * @param port port number
   * @return return success value
   */
  int connect(std::string const &ip, const std::string &port);

  /**
   * Async write wrapper
   * @param data RawBuffer to write
   * @param callback user provided completion callback
   */
  void async_write(const RawBuffer &data,
                   const t_writeHandler &callback);

  /**
   * Async read to buffer
   * @param buffer RawBuffer to store read data
   * @param size bytes to read
   * @param callback user provided completion callback
   */
  void async_read(RawBuffer &buffer, size_t size,
                  const t_readHandler &callback);

  /**
   * Async read, from position in buffer
   * resize buffer to length of @param from + @param size
   * @param buffer RawBuffer to store read data
   * @param from start index in buffer
   * @param size bytes to read
   * @param callback user provided completion callback
   */
  void async_read(RawBuffer &buffer, size_t from, size_t size,
                  const t_readHandler &callback);

  void setErrorCallback(const t_errorHandler &callback);
  tcp::socket &getSocket();
  bool isConnected() const;
  void close();

 protected:
  /**
   * On write completion callback
   * @param callback user provided callback
   * @param ec boost error code
   * @param size written byte size
   */
  void onWrite(const t_writeHandler &callback,
               const boost::system::error_code &ec, size_t size);

  /**
   * On read completion callback
   * @param callback user provided callback
   * @param ec boost error code
   * @param size written byte size
   */
  void onRead(const t_readHandler &callback, const boost::system::error_code &ec,
              size_t size);
};
}
}

#endif /* _ASIO_SOCKET_HPP_ */

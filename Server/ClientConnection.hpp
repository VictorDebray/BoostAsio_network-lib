//
// Created by Victor Debray on 2018-12-28.
//

#ifndef CLIENT_CONNECTION_HPP_
# define CLIENT_CONNECTION_HPP_

# include <memory>
# include <string>
# include <vector>

# include "Connection.hpp"
# include "Header.hpp"
# include "Queue.hpp"
# include "UsingClass.hpp"

class ServerNetwork;

class ClientConnection : public std::enable_shared_from_this<ClientConnection> {
 private:
  Sptr<RawBuffer> readBuff_;
  std::shared_ptr<::myboost::asio::Connection> socket_;
  ServerNetwork &network_;
  ReceivedQueue &receivedQueue_;
  CommandQueue &commandQueue_;

 public:
  ClientConnection(Sptr<::myboost::asio::Connection> sock,
                   ServerNetwork &net,
                   ReceivedQueue &queue,
                   CommandQueue &com);

  ClientConnection(const ClientConnection &other) = delete;

  ClientConnection &operator=(const ClientConnection &other) = delete;

 public:
  void connect();

  void startRead();

  void startWrite();

  void handleReadBody(size_t packet_size);
  void handleSaveBody();

  void handleWritePacket();

};

#endif /* CLIENT_CONNECTION_HPP_ */

//
// Created by Victor Debray on 2018-12-27.
//

#ifndef SERVER_CONNECTION_HPP
# define SERVER_CONNECTION_HPP

# include <iostream>
# include <cstddef>
# include <boost/asio.hpp>
# include <system_error>
# include <string>
# include <deque>
# include "Queue.hpp"
# include "Connection.hpp"

class ServerConnection : public std::enable_shared_from_this<ServerConnection> {
 private:
  using tcp = boost::asio::ip::tcp;

  std::string ip_;
  std::string port_;
  Sptr<boost::asio::io_service> ioService_;
  boost::asio::io_service::strand sendingQueueStrand_;
  boost::asio::io_service::strand receivedQueueStrand_;
  SendingQueue& sendingQueue_;
  ReceivedQueue& receivedQueue_;
  myboost::asio::Connection connection_;
  Sptr<RawBuffer> readBuff_;

  int connect();
  void send();
  void sendHandler();
  void startPacketSend();
  void receive();
  void handleReadBody(size_t packetSize);
  void handleSaveBody();

 public:
  ServerConnection(const std::string &ip,
                   const std::string &port,
                   SendingQueue& sendingQueue,
                   ReceivedQueue& receivedQueue);
  ~ServerConnection();

  int start();
  void run();
  bool isLive() const;
};

#endif /* !SERVER_CONNECTION_HPP */
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

/**
 * Class that handles the server connection
 */
class ServerConnection : public std::enable_shared_from_this<ServerConnection> {
 private:
  using tcp = boost::asio::ip::tcp;

  std::string ip_;
  std::string port_;
  Sptr<boost::asio::io_service> ioService_;
  boost::asio::io_service::strand sendingQueueStrand_;
  boost::asio::io_service::strand receivedQueueStrand_;
  SendingQueue &sendingQueue_;
  ReceivedQueue &receivedQueue_;
  myboost::asio::Connection connection_;
  Sptr<RawBuffer> readBuff_;

  /**
   * Connect to server
   * @return success value
   */
  int connect();

  /**
   * Start async read
   * - Reads HEADER_SIZE bytes to get body length
   * - Reads packet size body
   */
  void receive();

  /**
   * Checks if sending queue is not empty and starts write
   */
  void startPacketSend();

  /**
   * Read rest of message
   * @param packetSize size of body to read
   */
  void handleReadBody(size_t packetSize);

  /**
   * Saves read RawBuffer to received queue
   */
  void handleSaveBody();

  /**
   * Async writes message RawBuffer to client
   */
  void handleWritePacket();

 public:
  /**
   * ServerConnection constructor
   * @param ip address to server
   * @param port port number to server
   * @param sendingQueue data to be sent to server
   * @param receivedQueue data received from network
   */
  ServerConnection(const std::string &ip,
                   const std::string &port,
                   SendingQueue &sendingQueue,
                   ReceivedQueue &receivedQueue);
  ~ServerConnection();

  /**
   * Connect to server
   * @return success value
   */
  int start();

  /**
   * Run server communication
   */
  void run();

  bool isLive() const;
};

#endif /* !SERVER_CONNECTION_HPP */
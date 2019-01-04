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

/**
 *  Class that handles a client connection
 */
class ClientConnection : public std::enable_shared_from_this<ClientConnection> {
 private:
  Sptr<RawBuffer> readBuff_;
  std::shared_ptr<::myboost::asio::Connection> socket_;
  ServerNetwork &network_;
  ReceivedQueue &receivedQueue_;
  CommandQueue &commandQueue_;

 public:
  /**
   * Client connection constructor
   * @param sock ASIO socket wrapper
   * @param net network class
   * @param queue received raw buffer data queue
   * @param com commands queue to send
   */
  ClientConnection(Sptr<::myboost::asio::Connection> sock,
                   ServerNetwork &net,
                   ReceivedQueue &queue,
                   CommandQueue &com);

  ClientConnection(const ClientConnection &other) = delete;

  ClientConnection &operator=(const ClientConnection &other) = delete;

 public:

  /**
   * Sets error callback and starts read and write
   */
  void connect();

  /**
   * Start async read
   * - Reads HEADER_SIZE bytes to get body length
   * - Reads packet size body
   */
  void startRead();

  /**
   * Checks if sending queue is not empty and starts write
   */
  void startWrite();

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

};

#endif /* CLIENT_CONNECTION_HPP_ */

//
// Created by Victor Debray on 2018-12-28.
//

#ifndef SERVER_NETWORK_HPP_
# define SERVER_NETWORK_HPP_

# include <map>
# include <string>
# include <memory>
# include <set>

# include "Accept.hpp"
# include "ClientConnection.hpp"

/**
 * Class that handles clients and RawBuffer queues
 */
class ServerNetwork {
 private:
  Sptr<::myboost::asio::Accept> acceptor_;
  std::set<Sptr<ClientConnection>> clients_;
  ReceivedQueue &receivedQueue_;
  CommandQueue &commandQueue_;
  boost::asio::io_service::strand receiveStrand_;
  boost::asio::io_service::strand commandStrand_;

 public:
  /**
   * Server network constructor
   * @param addr server address
   * @param port listening port
   * @param rec received RawBuffer
   * @param com command queues to send
   */
  ServerNetwork(const std::string &addr,
                uint16_t port,
                ReceivedQueue &rec,
                CommandQueue &com);
  ~ServerNetwork() = default;

 public:
  /**
   * Start server
   */
  void run();

  /**
   * Stop accepting
   */
  void stop();

  /**
   * Add client to list
   * @param connection
   */
  void addConnection(const Sptr<ClientConnection> &connection);

  /**
  * Add client to list
  * @param connection
  */
  void removeConnection(const Sptr<ClientConnection> &connection);

  Sptr<boost::asio::io_service> getIoService() const;
  boost::asio::io_service::strand &getReceiveStrand();
  boost::asio::io_service::strand &getCommandStrand();

 private:
  /**
  * Start accepting new clients asynchronously
  */
  void startAccept();
  /**
   * On accept callback, add client to list and connect
   * @param newSock new connection just accepted
   */
  void acceptHandler(Sptr<::myboost::asio::Connection> newSock);
};

#endif /* SERVER_NETWORK_HPP_ */
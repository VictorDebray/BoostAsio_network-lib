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

class ServerNetwork {
 private:
  Sptr<::myboost::asio::Accept> acceptor_;
  std::set<Sptr<ClientConnection>> clients_;
  ReceivedQueue& receivedQueue_;
  CommandQueue& commandQueue_;
  boost::asio::io_service::strand receiveStrand_;
  boost::asio::io_service::strand commandStrand_;

 public:
  ServerNetwork(const std::string &addr,
                  uint16_t port,
                  ReceivedQueue &rec,
                  CommandQueue &com);
  ~ServerNetwork() = default;

 public:
  void run();
  void stop();
  void addConnection(const Sptr<ClientConnection> &spider);
  void removeConnection(const Sptr<ClientConnection> &spider);
  Sptr<boost::asio::io_service> getIoService() const;
  boost::asio::io_service::strand &getReceiveStrand();
  boost::asio::io_service::strand &getCommandStrand();

 private:
  void startAccept();
  void acceptHandler(Sptr<::myboost::asio::Connection> newSock);
};

#endif /* SERVER_NETWORK_HPP_ */
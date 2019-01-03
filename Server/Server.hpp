//
// Created by Victor Debray on 2018-12-28.
//

#ifndef SERVER_HPP_
# define SERVER_HPP_

# include <string>
# include <thread>
# include <atomic>
# include <memory>
# include <cstdint>
# include <unordered_map>

# include "ServerNetwork.hpp"
# include "MessagePacket.hpp"

class ServerNetwork;

class Server {
 private:
  ReceivedQueue receivedQueue_;
  CommandQueue commandQueue_;
  Sptr<ServerNetwork> network_;
  boost::asio::io_service::strand receiveStrand_;
  boost::asio::io_service::strand commandStrand_;
  std::thread deserializerThread_;

 public:
  static const uint16_t defaultPort = 12345;

 public:
  explicit Server(uint16_t port = defaultPort) noexcept;
  ~Server() noexcept;

 public:
  void run();

 private:
  void handleCommand();

 private:
  void deserializeHandler();
};

#endif /* SERVER_HPP_ */
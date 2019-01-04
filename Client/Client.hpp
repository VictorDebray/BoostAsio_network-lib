//
// Created by Victor Debray on 2018-12-27.
//

#ifndef PROJECT_CLIENT_HPP
#define PROJECT_CLIENT_HPP

# include <iostream>
# include <thread>
# include "ServerConnection.hpp"
# include "Queue.hpp"
# include "PacketFactory.hpp"

class Client {
 private:
  std::string ip_;
  std::string port_;
  std::shared_ptr<ServerConnection> currentServer_;

  DataQueue dataQueue_;
  SendingQueue sendingQueue_;
  ReceivedQueue receivedQueue_;

  PacketFactory packetFactory_;

  std::thread threadPacketFactory_;
  std::thread threadPacketLog_;
  std::thread threadConsole_;

  /**
   * Run packet factory
   */
  void startPacketFactory();

  /**
   * Start logging incoming packet (messages)
   */
  void startPacketLog();

  /**
   * Run console
   */
  void startConsole();

 public:
  /**
   * Client constructor
   * @param ip address of server
   * @param port port number
   */
  Client(const std::string &ip, const std::string& port);
  Client() = delete;
  ~Client();
};

#endif //PROJECT_CLIENT_HPP

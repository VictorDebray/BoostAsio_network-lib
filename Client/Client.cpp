//
// Created by Victor Debray on 2018-12-27.
//

#include <thread>
#include "Client.hpp"
#include "MessagePacket.hpp"
#include "Command.hpp"
#include "request.pb.h"

Client::Client(const std::string &ip, const std::string &port) :
    ip_(ip),
    port_(port),
    packetFactory_(dataQueue_, sendingQueue_),
    threadPacketFactory_(&Client::startPacketFactory, this),
    threadPacketLog_(&Client::startPacketLog, this),
    threadConsole_(&Client::startConsole, this) {}

Client::~Client() {
  threadPacketFactory_.join();
  threadPacketLog_.join();
}

void Client::startPacketFactory() {
  packetFactory_.run();
}

void Client::startPacketLog() {
  currentServer_ = std::make_shared<ServerConnection>(ip_, port_, sendingQueue_, receivedQueue_);

  if (currentServer_->start() == -1) {
    std::cerr << "Failed to connect" << std::endl;
    return;
  }

  bool serverOnline = currentServer_->isLive();
  while (true) {
    if (!serverOnline) {
      if (currentServer_->start() == -1)
        continue;
      serverOnline = currentServer_->isLive();
      if (serverOnline) {
      }
    } else {
      std::cout << "Server online." << std::endl;
      currentServer_->run();
    }
  }
}

void Client::startConsole() {
  std::string line;
  while (std::getline(std::cin, line)) {
    if (line == "stop") {
      std::cout << "EXIT" << std::endl;
      exit(0);
    }
    auto command = std::make_shared<Command>(line, line);
    std::lock_guard<std::mutex> guard(dataQueue_.mutex);
    dataQueue_.push(command);
  }
}

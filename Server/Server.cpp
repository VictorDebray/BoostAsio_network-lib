//
// Created by Victor Debray on 2018-12-28.
//

#include <iostream>
#include "Server.hpp"
#include "PacketLogger.hpp"

Server::Server(uint16_t port) noexcept :
    network_(std::make_shared<ServerNetwork>("0.0.0.0", port, receivedQueue_, commandQueue_)),
    receiveStrand_(*network_->getIoService()),
    commandStrand_(*network_->getIoService()),
    deserializerThread_(&Server::deserializeHandler, this) {
  this->run();
}

Server::~Server() noexcept {
  deserializerThread_.join();
}

void Server::run() {
  network_->run();
  std::cout << "Server shutdown." << std::endl;
}

void Server::deserializeHandler() {
  std::cout << "Started deserializer.." << std::endl;
  MessagePacket<Request> messagePacket;
  while (true) {
    while (!receivedQueue_.getData().empty()) {
      std::lock_guard<std::mutex> guard(receivedQueue_.mutex);
      Sptr<RawBuffer> buffer = receivedQueue_.cgetFront();
      std::cout << PacketLogger::show_hex(*buffer) << std::endl;
      if (messagePacket.unpack(*buffer)) {
        std::cout << messagePacket.getMessage()->command() << std::endl;
      } else {
        std::cerr << "Skipped incorrect message or corrupted buffer" << std::endl;
      }
      receivedQueue_.pop();
    }
  }
}

void Server::handleCommand() {
  try {
    std::string line;
    while (std::getline(std::cin, line)) {
      if (line == "stop")
        network_->stop();
    }
    network_->stop();
  }
  catch (std::exception &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }
}

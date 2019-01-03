//
// Created by Victor Debray on 2018-12-27.
//

#include "ServerNetwork.hpp"
#include "Connection.hpp"

ServerNetwork::ServerNetwork(const std::string &addr,
                             uint16_t port,
                             ReceivedQueue &rec,
                             CommandQueue &com) :
    acceptor_(std::make_shared<::myboost::asio::Accept>()),
    receivedQueue_(rec),
    commandQueue_(com),
    receiveStrand_(*acceptor_->getIoService()),
    commandStrand_(*acceptor_->getIoService()) {
  acceptor_->bind(addr, port);
  acceptor_->listen(20);
  startAccept();
}

void ServerNetwork::run() {
  acceptor_->start();
  std::cout << "Server up and running." << std::endl;
}

void ServerNetwork::stop() {
  acceptor_->stop();
}

void ServerNetwork::addConnection(const std::shared_ptr<ClientConnection> &connection) {
  clients_.insert(connection);
}

void ServerNetwork::removeConnection(const std::shared_ptr<ClientConnection> &connection) {
  clients_.erase(connection);
}

Sptr<boost::asio::io_service> ServerNetwork::getIoService() const {
  return acceptor_->getIoService();
}

void ServerNetwork::acceptHandler(const Sptr<::myboost::asio::Connection> newSock) {
  auto client = std::make_shared<ClientConnection>(newSock, *this, receivedQueue_, commandQueue_);
  std::cout << "New client connected, logging its actions !" << std::endl;
  addConnection(client);
  client->connect();
}

boost::asio::io_service::strand &ServerNetwork::getReceiveStrand() {
  return receiveStrand_;
}

boost::asio::io_service::strand &ServerNetwork::getCommandStrand() {
  return commandStrand_;
}

void ServerNetwork::startAccept() {
  auto acc(std::bind(&ServerNetwork::acceptHandler, this, std::placeholders::_1));

  acceptor_->async_accept(acc);
}
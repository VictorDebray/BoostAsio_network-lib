//
// Created by Victor Debray on 2018-12-27.
//

#include "ServerConnection.hpp"
#include "PacketLogger.hpp"

ServerConnection::ServerConnection(const std::string &ip,
                                   const std::string &port,
                                   SendingQueue &sendingQueue,
                                   ReceivedQueue& receivedQueue) :
    ip_(ip),
    port_(port),
    ioService_(std::make_shared<boost::asio::io_service>()),
    sendingQueueStrand_(*ioService_),
    receivedQueueStrand_(*ioService_),
    sendingQueue_(sendingQueue),
    receivedQueue_(receivedQueue),
    connection_(ioService_),
    readBuff_(std::make_shared<RawBuffer>()){
  readBuff_->resize(HEADER_SIZE);
}

ServerConnection::~ServerConnection() {
  connection_.close();
}

int ServerConnection::start() {
  if (connect() == 0)
    return 0;
  connection_.close();
  return -1;
}

int ServerConnection::connect() {
  std::weak_ptr<ServerConnection> self(shared_from_this());
  std::shared_ptr<ServerConnection> nwself(shared_from_this());
  connection_.setErrorCallback(std::bind([this](std::weak_ptr<ServerConnection> lself) {
    if (auto sself = lself.lock()) {
      std::cout << "Disconnected from server" << std::endl;
    }
  }, self));

  try {
    connection_.connect(ip_, port_);
  } catch (boost::system::system_error &e) {
    std::cerr << e.what() << std::endl;
    return -1;
  }
  return 0;
}

void ServerConnection::run() {
  receive();
  startPacketSend();
  ioService_->run();
  connection_.close();
}

bool ServerConnection::isLive() const {
  return connection_.isConnected();
}

void ServerConnection::startPacketSend() {
  ioService_->post(sendingQueueStrand_.wrap([me = shared_from_this()]() {
    if (!me->sendingQueue_.getData().empty()) {
      me->handleWritePacket();
    } else {
      me->startPacketSend();
    }
  }));
}

void ServerConnection::handleWritePacket() {
  if (!connection_.isConnected()) {
    std::cerr << "Send: Can't connect to server socket." << std::endl;
    return;
  }
  Sptr<RawBuffer> buff = sendingQueue_.cgetFront();
  connection_.async_write(*buff, sendingQueueStrand_.wrap(
      [me = shared_from_this()](std::size_t) {
        me->sendingQueue_.pop();
        me->receive();
      }));
}

void ServerConnection::receive() {
  connection_.async_read(*readBuff_, HEADER_SIZE, [me = shared_from_this()](size_t csize) {
    if (csize <= 0) {
      me->handleWritePacket();
      return;
    }
    auto packetSize = Header::decode(*me->readBuff_);
    me->handleReadBody(packetSize);
  });
}

void ServerConnection::handleReadBody(size_t packetSize) {
  connection_.async_read(*readBuff_, packetSize, [me = shared_from_this()](size_t /*csize*/) {
    me->handleSaveBody();
  });
}

void ServerConnection::handleSaveBody() {
  ioService_->post(receivedQueueStrand_.wrap(
      [me = shared_from_this()]() {
        me->receivedQueue_.push(me->readBuff_);
        me->receive();
      })
  );
}
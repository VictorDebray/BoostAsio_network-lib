//
// Created by Victor Debray on 2018-12-28.
//

#include "ClientConnection.hpp"
#include "ServerNetwork.hpp"
#include "Queue.hpp"
#include "Header.hpp"

ClientConnection::ClientConnection(Sptr<::myboost::asio::Connection> sock,
                                   ServerNetwork &net,
                                   ReceivedQueue &queue,
                                   CommandQueue &com) :
    readBuff_(std::make_shared<RawBuffer>()),
    socket_(std::move(sock)),
    network_(net),
    receivedQueue_(queue),
    commandQueue_(com) {
  readBuff_->resize(HEADER_SIZE);
}

void ClientConnection::connect() {
  std::weak_ptr<ClientConnection> self(shared_from_this());
  std::shared_ptr<ClientConnection> nwself(shared_from_this());
  socket_->setErrorCallback(std::bind([this](std::weak_ptr<ClientConnection> lself) {
    if (auto sself = lself.lock()) {
      network_.removeConnection(sself);
    }
  }, self));
  startRead();
}

void ClientConnection::startRead() {
  socket_->async_read(*readBuff_, HEADER_SIZE, [me = shared_from_this()](size_t csize) {
    if (csize <= 0) {
      me->startWrite();
      return;
    }
    auto packetSize = Header::deserialize_header(*me->readBuff_);
    me->handleReadBody(packetSize);
  });
}

void ClientConnection::handleReadBody(size_t packet_size) {
  socket_->async_read(*readBuff_, HEADER_SIZE, packet_size, [me = shared_from_this()](size_t /*csize*/) {
    me->handleSaveBody();
  });
}

void ClientConnection::handleSaveBody() {
  network_.getIoService()->post(
      network_.getReceiveStrand().wrap(
          [me = shared_from_this()]() {
            auto buffer = std::make_shared<RawBuffer>(*me->readBuff_);
            me->receivedQueue_.push(buffer);
            me->startRead();
          })
  );
}

void ClientConnection::startWrite() {
  network_.getIoService()->post(
      network_.getCommandStrand().wrap([me = shared_from_this()]() {
        if (!me->commandQueue_.getData().empty()) {
          me->handleWritePacket();
        } else {
          me->startRead();
        }
      }));
}

void ClientConnection::handleWritePacket() {
  Sptr<RawBuffer> write = commandQueue_.cgetFront();
  socket_->async_write(*write, [me = shared_from_this()](size_t writeSize) {
    if (writeSize <= 0)
      return;
    me->startRead();
  });
}

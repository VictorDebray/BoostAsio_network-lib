//
// Created by Victor Debray on 2018-12-27.
//

#include <fstream>
#include "PacketFactory.hpp"
#include "Header.hpp"

PacketFactory::PacketFactory(DataQueue &dataQueue, SendingQueue &sendingQueue) :
    dataQueue_(dataQueue),
    sendingQueue_(sendingQueue) {}

void PacketFactory::run() {
  while (true) {
    if (!dataQueue_.getData().empty()) {
      try {
        std::lock_guard<std::mutex> guardData(dataQueue_.mutex);
        auto cmd = dataQueue_.cgetFront();

        auto req = std::make_shared<Request>();
        req->set_client_id("Client 1");
        req->set_command(std::move(cmd->command));
        messagePacket_.setMessage(req);
//        messagePacket_.timestampPacket();

        auto buffer = std::make_shared<std::vector<uint8_t>>();
        if (!messagePacket_.pack(*buffer)) {
          std::cerr << "Passing empty packet" << std::endl;
        }

        std::lock_guard<std::mutex> guardSending(sendingQueue_.mutex);
        sendingQueue_.push(buffer);
        dataQueue_.pop();
      } catch (const std::exception &e) {
        throw e;
      }
    }
  }
}


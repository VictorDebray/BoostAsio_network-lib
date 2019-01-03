//
// Created by Victor Debray on 2018-12-27.
//

#ifndef PACKETFACTORY_HPP_
# define PACKETFACTORY_HPP_

# include <string>
# include "Queue.hpp"
# include "Header.hpp"

class PacketFactory {
 private:
  DataQueue &dataQueue_;
  SendingQueue &sendingQueue_;
  MessagePacket<Request> messagePacket_;

 public:
  PacketFactory() = delete;
  ~PacketFactory() = default;
  PacketFactory(DataQueue &dataQueue, SendingQueue &sendingQueue);

  void run();
};

#endif // !PACKETFACTORY_HPP_
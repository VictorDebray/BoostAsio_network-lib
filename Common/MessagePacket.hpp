//
// Created by Victor Debray on 2019-01-01.
//

#ifndef PROJECT_PACKET_HPP
#define PROJECT_PACKET_HPP

#include "Header.hpp"
#include "request.pb.h"

/**
 * Class that handles header and protobuf message RawBuffer
 * @tparam MessageType protobuf message type
 */
template<class MessageType>
class MessagePacket {
 private:
  Sptr<MessageType> packet_;

 public:
  explicit MessagePacket(Sptr<MessageType> packet = std::make_shared<MessageType>()) : packet_(packet) {};

  void setMessage(Sptr<MessageType> packet) {
    packet_ = packet;
  }

  Sptr<MessageType> getMessage() {
    return packet_;
  }

  /**
   * Packs header to RawBuffer with message packet
   * @param buf buffer to pack message packet to
   * @return success
   */
  bool pack(RawBuffer &buf) const {
    if (!packet_)
      return false;

    unsigned msg_size = packet_->ByteSize();
    buf.resize(HEADER_SIZE + msg_size);
    Header::encode(buf, msg_size);
    return packet_->SerializeToArray(&buf[HEADER_SIZE], msg_size);
  }

  /**
   * Parse RawBuffer to message packet
   * @param buf buffer to read from
   * @return success
   */
  bool unpack(const RawBuffer &buf) {
    return packet_->ParseFromArray(&buf[HEADER_SIZE], buf.size() - HEADER_SIZE);
  }

};

#endif //PROJECT_PACKET_HPP

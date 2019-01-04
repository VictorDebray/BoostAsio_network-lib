//
// Created by Victor Debray on 2018-12-29.
//

#ifndef HEADER_HPP_
# define HEADER_HPP_

# include <sstream>
# include <assert.h>
# include "UsingClass.hpp"

namespace Header {

/**
 * Decode header from RawBuffer
 * @param buf buffer to decode header from
 * @return packet size (header)
 */
static size_t decode(const RawBuffer &buf) {
  if (buf.size() < HEADER_SIZE)
    return 0;
  size_t size = 0;
  for (unsigned i = 0; i < HEADER_SIZE; ++i)
    size = size * 256 + (static_cast<unsigned>(buf[i]) & 0xFF);
  return size;
}

/**
 * Encode header in buffer from buffer
 * @param buf buffer to encode buffer to
 * @param size size of packet (message)
 */
static void encode(RawBuffer &buf, unsigned size) {
  assert(buf.size() >= HEADER_SIZE);
  buf[0] = static_cast<uint8_t>((size >> 24) & 0xFF);
  buf[1] = static_cast<uint8_t>((size >> 16) & 0xFF);
  buf[2] = static_cast<uint8_t>((size >> 8) & 0xFF);
  buf[3] = static_cast<uint8_t>(size & 0xFF);
}
};

#endif /* HEADER_HPP_ */
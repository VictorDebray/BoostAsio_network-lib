//
// Created by Victor Debray on 2018-12-27.
//

#ifndef PACKET_LOGGER_HPP
# define PACKET_LOGGER_HPP

#include "UsingClass.hpp"

namespace PacketLogger {
template<class CharContainer>
std::string show_hex(const CharContainer &c) {
  std::string hex;
  char buf[16];
  typename CharContainer::const_iterator i;
  for (i = c.begin(); i != c.end(); ++i) {
    std::sprintf(buf, "%02X ", static_cast<unsigned>(*i) & 0xFF);
    hex += buf;
  }
  return hex;
}
}

#endif /* PACKET_LOGGER_HPP */
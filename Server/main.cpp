//
// Created by Victor Debray on 2018-12-27.
//

#include <iostream>
#include "Server.hpp"

int main(int ac, char **av) {
  std::cout << "Hello, World!" << std::endl;
  if (ac != 2) {
    std::cout << "Please specify port." << std::endl;
    return 1;
  }
  int port = std::strtol(av[1], nullptr, 10);
  if (port == 0) {
    std::cerr << strerror(errno) << std::endl;
  }
  Server server(port);
  return 0;
}
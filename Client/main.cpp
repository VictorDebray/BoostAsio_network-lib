//
// Created by Victor Debray on 2018-12-27.
//

#include <iostream>
#include "Client.hpp"

int main(int ac, char **av) {
  std::cout << "Hello, World!" << std::endl;
  if (ac != 3) {
    std::cout << "Please specify port." << std::endl;
    return 1;
  }
  std::string ip(av[1]);
  std::string port(av[2]);

  Client client(ip, port);
  return 0;
}
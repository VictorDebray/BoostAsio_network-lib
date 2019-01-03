//
// Created by Victor Debray on 2019-01-02.
//

#ifndef PROJECT_COMMAND_HPP
# define PROJECT_COMMAND_HPP

# include <string>

class Command {
 public:
  std::string command;
  std::string params;

 public:
  Command(std::string const &command, std::string const &params) :
      command(command), params(params) {};
};

#endif //PROJECT_COMMAND_HPP

//
// Created by Victor Debray on 2018-12-27.
//

#ifndef PROJECT_QUEUE_HPP
#define PROJECT_QUEUE_HPP

#include <queue>
#include "UsingClass.hpp"
#include "MessagePacket.hpp"
#include "Command.hpp"
#include "request.pb.h"

/*
** Base queue class
*/
template<class T>
struct AQueue {
 private:
  std::queue<Sptr<T>> _data;
 public:
  std::mutex mutex;

  std::queue<Sptr<T>> const &getData() const {
    return _data;
  }

  void push(const Sptr<T> elem) {
    _data.push(elem);
  }

  Sptr<T> const cgetFront() const {
    return _data.front();
  }

  Sptr<T> getFront() {
    return _data.front();
  }

  void pop() {
    _data.pop();
  }
};

class DataQueue : public AQueue<Command> {
};

struct CommandQueue : public AQueue<RawBuffer> {
};

class ReceivedQueue : public AQueue<RawBuffer> {
};

struct SendingQueue : public AQueue<RawBuffer> {
};

#endif //PROJECT_QUEUE_HPP

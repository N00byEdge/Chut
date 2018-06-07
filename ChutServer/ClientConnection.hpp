#pragma once

#include "Chut.hpp"

#include "SFML/Network.hpp"

#include <functional>
#include <thread>
#include <mutex>
#include <queue>

struct ClientConnection {
  ClientConnection(sf::TcpListener &listener, std::function<void(Chut::Message &)> callback) {
    listener.accept(socket);
    socket.setBlocking(true);
    thread = std::thread{ [&]() { this->listen(callback); } };
  }

  ~ClientConnection() {
    if (thread.joinable())
      thread.join();
  }

  void sendMessage(Chut::Message &m) {
    try {
      m.serialize(socket);
    }
    catch(...) { }
  }

  bool isActive() const {
    return active;
  }

private:
  void listen(std::function<void(Chut::Message &)> callback) {
    struct ActiveReseter {
      ActiveReseter(bool &b) : b{ b } { }
      ~ActiveReseter() { b = false; }
    private:
      bool &b;
    } reseter{active};
    
    while(true) {
      try {
        auto message = Chut::Message::deserialize(socket);
        callback(message);
      }
      catch (...) {
        return;
      }
    }
  }

  bool active = true;
  std::thread thread;
  sf::TcpSocket socket;
};

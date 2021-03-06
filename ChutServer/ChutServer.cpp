#include "Chut.hpp"

#include "ClientConnection.hpp"

#include <list>
#include <iostream>

int main() {
  auto const port = Chut::ask<unsigned short>(std::cin, std::cout, "Please enter a listening port: ");
  sf::TcpListener listener;
  listener.setBlocking(true);
  listener.listen(port);

  std::list<ClientConnection> clients;
  std::mutex sendMutex;
  std::mutex outMutex;

  auto onMessage = [&](Chut::Message &message) {
    {
      std::lock_guard<std::mutex> outLock{ outMutex };
      std::cout << "[" << message.source << "]: " << message.message << "\n";
    }

    {
      std::lock_guard<std::mutex> sendLock{ sendMutex };
      for (auto it = clients.begin(); it != clients.end();) {
        if (!it->isActive()) {
          it = clients.erase(it);
        }
        else {
          it++->sendMessage(message);
        }
      }
    }
  };

  while (true) {
    clients.emplace_back(listener, onMessage);
  }
}


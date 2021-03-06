#include "Chut.hpp"

#include <iostream>
#include <thread>

int main() {
  auto const hostname = Chut::ask<std::string>(std::cin, std::cout, "Enter hostname: ");
  auto const port = Chut::ask<unsigned short>(std::cin, std::cout, "Enter port: ");

  sf::TcpSocket socket;
  socket.setBlocking(true);
  socket.connect(hostname, port);

  auto listen = [&]() {
    while (true) {
      try {
        auto message = Chut::Message::deserialize(socket);
        std::cout << "[" << message.source << "]: " << message.message << "\n";
      }
      catch(...) {
        std::cout << "Invalid message recieved.\n";
      }
    }
  };

  if(socket.getRemoteAddress() == sf::IpAddress::None) {
    std::cout << "Connection failed.\n";
    return 1;
  }

  std::cout << "Connected to " << socket.getRemoteAddress() << ":" << socket.getRemotePort() << "\n";
  std::thread t{ listen };

  auto const username = Chut::ask<std::string>(std::cin, std::cout, "Enter a username: ");
  std::string message;
  getline(std::cin, message);
  while(getline(std::cin, message)) {
    try {
      if (!message.empty())
        Chut::Message{ username, message }.serialize(socket);
    }
    catch(...) {
      std::cout << "Error sending message.\n";
      return 2;
    }
  }
}


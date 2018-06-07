#pragma once

#include <string>

#include <SFML/Network.hpp>

namespace Chut {
  struct Message {
    std::string source;
    std::string message;

    static Message deserialize(sf::TcpSocket &sock) {
      size_t sourceLen = 0;
      size_t recieved;
      sock.receive(&sourceLen, 1, recieved);
      if (recieved != 1)
        throw "Could not recieve message";
      size_t messageLen = 0;
      sock.receive(&messageLen, 2, recieved);
      if (recieved != 2)
        throw "Could not recieve message";

      char buf[0xffff];

      sock.receive(buf, sourceLen, recieved);
      if (recieved != sourceLen)
        throw "Could not recieve message";
      buf[recieved] = '\0';
      std::string source{ buf, buf + sourceLen };

      sock.receive(buf, messageLen, recieved);
      if (recieved != messageLen)
        throw "Could not recieve message";
      buf[recieved] = '\0';
      std::string message{ buf, buf + messageLen };


      return { std::move(source), std::move(message) };
    }

    void serialize(sf::TcpSocket &sock) const {
      auto s = source.size() & 0xff;
      if(sock.send(&s, 1) != sf::TcpSocket::Done) throw "Could not send message";
      s = message.size() & 0xffff;
      if (sock.send(&s, 2) != sf::TcpSocket::Done) throw "Could not send message";
      sock.send(source.c_str(), source.size() & 0xff);
      sock.send(message.c_str(), message.size() & 0xffff);
    }
  };
}

#include "Window.h"
#include "Server.h"
#include <cmath>

using namespace std;

void handler(sf::IpAddress &ip, const PortNumber &port, const PacketID &packetID, sf::Packet &packet, Server *server)
{
   ClientID id = server->getClientID(ip, port);

   if (id != static_cast<ClientID>(Network::NullID))
   {
      if (static_cast<PacketType>(packetID) == PacketType::Disconnect)
      {
         sf::Packet newPacket;
         StampPacket(PacketType::Disconnect, newPacket);
         server->serverLogic.signToRemovePlayer(id);
         server->removeClient(id);
         server->broadcast(newPacket);
      }
      else if (static_cast<PacketType>(packetID) == PacketType::Message)
      {
         std::string receiveMsg;
         packet >> receiveMsg;

         std::string msg = ip.toString() + ": " + std::to_string(port) + " :" + receiveMsg;
         DEBUG_COUT(msg);
         sf::Packet newPacket;
         StampPacket(PacketType::Message, newPacket);
         newPacket << msg;

         server->broadcast(newPacket, id);
      }
      else if (static_cast<PacketType>(packetID) == PacketType::PlayerMove)
      {
         int dir;
         ClientID clientID;
         packet >> clientID >> dir;
         sf::Packet newPacket;
         StampPacketPlayerMove(newPacket, clientID, static_cast<MoveDirection>(dir));
         DEBUG_COUT("ship " << clientID << "is moving " << dir);
         sf::Lock lock(server->getMutex());
         server->serverLogic.movePlayer(clientID, static_cast<MoveDirection>(dir));
      }
      else if (static_cast<PacketType>(packetID) == PacketType::PlayerCreate)
      {
         //TODO this x and y should get from map
         float x = 400, y = 400;
         ClientID clientID;
         packet >> clientID;
         sf::Packet newPacket;
         StampPacket(PacketType::PlayerCreate, newPacket);
         newPacket << clientID << x << y;
         DEBUG_COUT("Creating ship");

         
         server->serverLogic.addPlayer(clientID, x, y);
         server->broadcast(newPacket);
      }
   }
   else if (static_cast<PacketType>(packetID) == PacketType::Connect)
   {
      ClientID id = server->addClient(ip, port);
      sf::Packet newPacket;
      StampPacket(PacketType::Connect, newPacket);
      newPacket << id;
      std::cout << server->send(id, newPacket) << "   " <<  ip << "  " << port <<  std::endl;
   }

}

void commandHandler(Server *server)
{
   while (server->isRunning())
   {
      std::string str;
      std::getline(std::cin, str);
      if (str == "quit")
      {
         server->stop(); 
         break;
      }
      else if (str == "dc")
      {
         server->disconnectAll();
      }

   }
}


int main()
{

   Server server(handler);
	if (server.start() == true)
	{
      sf::Thread c(&commandHandler, &server);
      c.launch();
      std::function<void(void)> reqHandler = std::bind(&Server::requestHandling, &server);
      sf::Thread reqHandlerThread(reqHandler);
      reqHandlerThread.launch();
      server.serverLogic.initPsyhicsWorld();
      
      Window win("Server", sf::Vector2u(900, 800));
      server.serverLogic.initDebugDraw(&win);
      
      sf::Clock clock;
      sf::Time elapsedTime;
      sf::Time timeSinceLastUpdate = sf::Time::Zero;
      sf::Time timeSinceLastUpdateSnapshot = sf::Time::Zero;
      const sf::Time timePerFrame = sf::seconds(1.f / 60.f);
      const sf::Time timePerSnapshot = sf::seconds(1.f / 30.f);
      sf::Packet p;
      while (win.isDone() == false)
      {
         elapsedTime = clock.restart();
         timeSinceLastUpdate += elapsedTime;
         timeSinceLastUpdateSnapshot += elapsedTime;

         while (timeSinceLastUpdateSnapshot > timePerSnapshot)
         {
            std::cout << timeSinceLastUpdateSnapshot.asSeconds() << std::endl;
            {
               sf::Lock lock(server.getMutex());
               p = server.serverLogic.getPlayersSnapshot();
            }
            server.broadcast(p);
            p.clear();
            timeSinceLastUpdateSnapshot -= timePerFrame;
         }

         while (timeSinceLastUpdate > timePerFrame)
         {
            timeSinceLastUpdate -= timePerFrame;
            win.update();
            server.serverLogic.updatePsyhicsWorld();
         }
         win.beginDraw();
         server.serverLogic.debugDraw();
         win.endDraw();
         
         server.serverLogic.clearBodies();
      }

      
	}
   
}
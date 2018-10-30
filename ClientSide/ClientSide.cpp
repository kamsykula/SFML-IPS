#include "Client.h"
#include <Game.h>

void clientHandler(const PacketID &id, sf::Packet &packet, Client *client)
{
	if (static_cast<PacketType>(id) == PacketType::Message)
	{
		std::string message;
		packet >> message;
		std::cout << message << std::endl;
	}
	else if (static_cast<PacketType>(id) == PacketType::Disconnect)
	{
		DEBUG_COUT("Client disconnect");
		client->disconnect();
	}
}

void commandProcess(Client* client)
{
	while (client->isConnected())
	{
		std::string str;
		std::getline(std::cin, str);
		if (str != "")
		{
			if (str == "!quit")
			{
				client->disconnect();
				break;
			}
			sf::Packet p;
			StampPacket(PacketType::Message, p);
			p << str;
			client->sendPacket(p);
		}
	}
}
int main(int argc, char** argv)
{
	/**//*
   sf::IpAddress ip;
	PortNumber port;
	if (argc == 1)
	{
		std::cout << "Enter Server IP: ";
		std::cin >> ip;
		std::cout << "Enter Server Port: ";
		std::cin >> port;
	}
	else if (argc == 3)
	{
		ip = argv[1];
		port = atoi(argv[2]);
	}
	else
	{
		return 1;
	}
	Client client;
	client.setServer(ip, port);
	client.setup(&clientHandler);
	sf::Thread c(&commandProcess, &client);
	if (client.connect())
	{
		c.launch();
		sf::Clock clock;
		clock.restart();
		while (client.isConnected())
		{
			client.update(clock.restart());
		}
	}
	else
	{
		std::cout << "Failed to connect." << std::endl;
	}
	std::cout << "Quitting..." << std::endl;
	sf::sleep(sf::seconds(1.f));*/
   
   Game game;
   while (game.getWindow()->isDone() == false)
   {
      game.update();
      game.render();
      game.lateUpdate();
   }
   
	return 0;
}
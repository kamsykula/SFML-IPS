#include "Client.h"
#include "Game.h"



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
   Game game("Game");
   game.run();
   
   //sf::Thread c(&commandProcess, &c
	std::cout << "Quitting..." << std::endl;
	sf::sleep(sf::seconds(1.f));
    
   
	return 0;
}
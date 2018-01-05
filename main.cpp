#include <iostream>
#include <SFML/Network.hpp>


#define SERVER_IP "192.168.88.138"
#define SERVER_PORT 5000

using namespace std;

sf::TcpSocket socket;

int
iIdJugador,
iIdRaza;

std::string
sUserNick,
sUserPass,
sUserPassRepeat,
sUserRaces,
sCharacterName,
sPlayerPosition,
sPlayerSelection,
sCharacterSelected;

std::vector<string> sNameRaces;

bool
bVerified = false,
bPlayerCreated = false,
bRepeatPassword = false,
bRaceCreated = false,
bCharacterCreated = false;

int main()
{


    sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT, sf::seconds(15.f));
    return 0;
    }



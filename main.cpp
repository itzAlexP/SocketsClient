#include <iostream>
#include <SFML/Network.hpp>
#include <unistd.h>

#define SERVER_IP "192.168.88.138"
#define SERVER_PORT 5000

using namespace std;

sf::TcpSocket
socket;

char
cBufferSocket[100];


std::string
sUserNick,
sUserPass,
sInfo;

size_t
received;

bool
bVerified = false,
bVerifiedPassword = false;


int main()
{


    sf::Socket::Status status = socket.connect(SERVER_IP, SERVER_PORT, sf::seconds(15.f));


    //El cliente se intentara conectar todo el rato cada 15 segundos hasta conseguirlo.
    if(status != sf::Socket::Done)
    {

        std::cout << "Error de conexión con el servidor." << std::endl;
        return 0;

    }

    //Repètiremos login hasta que sea valido
    while(!bVerified)
    {

        //Enviamos nombre de usuario al servidor
        cout << "Inserte nombre de usuario." << std::endl;
        cin >> sInfo;
        status = socket.send(sInfo.c_str(), sizeof(sInfo) + 1);

        //Esperamos respuesta del servidor
        status = socket.receive(cBufferSocket, sizeof(cBufferSocket), received);

        //Existe usuario
        if(cBufferSocket[0] == '0')
        {

            //Pedimos la contraseña y la enviamos al servidor
            system("clear");
            cout << "Inserte password." << std::endl;
            cin >> sInfo;
            status = socket.send(sInfo.c_str(), sizeof(sInfo) + 1);

            //Esperamos respuesta del servidor
            status = socket.receive(cBufferSocket, sizeof(cBufferSocket), received);

            //Contraseña correcta
            if(cBufferSocket[0] == '0')
            {

                bVerified = true;


            }//Contraseña incorrecta
            else
            {

                //Preguntaremos la contraseña hasta que la introduzca correctamente
                while(!bVerifiedPassword)
                {

                    cout << "Contraseña incorrecta, introduzcala de nuevo." << std::endl;
                    cin >> sInfo;

                    status = socket.send(sInfo.c_str(), sizeof(sInfo) + 1);

                    //Esperamos respuesta del servidor
                    status = socket.receive(cBufferSocket, sizeof(cBufferSocket), received);

                    if(cBufferSocket[0] == '0')
                    {

                        bVerified = true;
                        bVerifiedPassword = true;

                    }
                }

            }
        }
    }

    return 0;
}



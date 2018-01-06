#include <iostream>
#include <SFML/Network.hpp>
#include <unistd.h>
#include <vector>

#define SERVER_IP "192.168.88.138"
#define SERVER_PORT 5000

using namespace std;

std::vector<string>
vRazas;

sf::TcpSocket
socket;

char
cBufferSocket[100],
              cBufferSocketLong[2000];


int
iCounter = 0;

std::string
sUserNick,
sUserPass,
sInfo,
sRazas;

size_t
received;

bool
bVerified = false,
bVerifiedPassword = false,
bVerifiedNewUser = false,
bVerifiedRace = false,
bVerifiedCharacterName = false;


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
        else if (cBufferSocket[0] == '2') //Registro de usuario
        {
            //Pedimos la contraseña y la enviamos al servidor
            system("clear");
            std::cout << "\nNo existe dicho usuario. Se va a proceder a crear un nuevo usuario.\n" << "\nInserte nombre de usuario" <<std::endl;
            cin >> sInfo;
            status = socket.send(sInfo.c_str(), sizeof(sInfo) + 1);

            //Esperamos respuesta del servidor
            status = socket.receive(cBufferSocket, sizeof(cBufferSocket), received);

            //Usuario nuevo existe
            if(cBufferSocket[0] == '1')
            {
                while(!bVerifiedNewUser)
                {
                    std::cout << "Usuario ya en uso, inserte otro nombre de usuario." << std::endl;
                    cin >> sInfo;

                    status = socket.send(sInfo.c_str(), sizeof(sInfo) + 1);

                    //Esperamos respuesta del servidor
                    status = socket.receive(cBufferSocket, sizeof(cBufferSocket), received);

                    if(cBufferSocket[0] == '0')
                    {
                        bVerifiedNewUser = true;
                    }
                }
            }

            system("clear");
            std::cout << "\nUsuario disponible, inserte contraseña.\n";

            while(!bVerifiedPassword)//Repetimos hasta tener una contraseña valida
            {

                //Pedimos al usuario que escriba la contraseña
                std::cin >> sInfo;
                status = socket.send(sInfo.c_str(), sizeof(sInfo) + 1);

                std::cout << "\nRepita contraseña.\n";
                std::cin >> sInfo;
                status = socket.send(sInfo.c_str(), sizeof(sInfo) + 1);

                //Esperamos respuesta del servidor
                status = socket.receive(cBufferSocket, sizeof(cBufferSocket), received);

                if(cBufferSocket[0] == '0')
                {
                    bVerifiedPassword = true;
                }
                else if(cBufferSocket[0] == '1')//Las contraseñas no coinciden
                {
                    std::cout << "\nLas contraseñas no coinciden, introduzca de nuevo la contraseña.\n";
                }
            }

            system("clear");

            //Seleccionamos raza para el personaje
            status = socket.receive(cBufferSocketLong, sizeof(cBufferSocketLong), received);


            std::cout << "Selecciona raza\n" << std::endl;

            for(int i = 0; i < sizeof(cBufferSocketLong); i ++)
            {

                if(cBufferSocketLong[i] != '-')
                {

                    sRazas =  sRazas + cBufferSocketLong[i];

                }

                if(cBufferSocketLong[i] == '-')
                {


                    vRazas.push_back(sRazas);
                    sRazas.clear();

                }

            }

            for(int i = 0; i < vRazas.size() / 2; i++, iCounter =  iCounter + 2)
            {

                std::cout << vRazas[iCounter] << "    |    " << vRazas[iCounter + 1] << std::endl;

            }

            while(!bVerifiedRace)
            {

                cin >> sInfo;

                for(int i = 0; i < vRazas.size(); i = i+2)
                {

                    if(vRazas[i] == sInfo)
                    {
                        status = socket.send(sInfo.c_str(), sizeof(sInfo) + 1);
                        bVerifiedRace = true;

                    }
                }

                if(!bVerifiedRace)
                {

                    std::cout << "\n Raza no valida, seleccione una de las listadas." << std::endl;

                }

            }


            std::cout << "\nInserte nombre del personaje" << std::endl;



            //Enviamos el nombre del personaje
            cin >> sInfo;
            status = socket.send(sInfo.c_str(), sizeof(sInfo) + 1);

            //Esperamos respuesta del servidor
            status = socket.receive(cBufferSocket, sizeof(cBufferSocket), received);

            //Nombre Ocupado
            if(cBufferSocket[0] == '1')
            {
                //Mientras no pongga un nombre valido.
                while(!bVerifiedCharacterName)
                {

                    //Enviamos nombre al servidor
                    std::cout << "\nNombre no disponible, inserte otro nombre." << std::endl;
                    cin >> sInfo;
                    status = socket.send(sInfo.c_str(), sizeof(sInfo) + 1);

                    //Esperamos respuesta del servidor
                    status = socket.receive(cBufferSocket, sizeof(cBufferSocket), received);

                    //Si el nombre esta libre
                    if(cBufferSocket[0] == '0')
                    {

                        bVerifiedCharacterName = true;
                    }
                }

            }












        }
    }

    return 0;
}

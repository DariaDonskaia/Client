

#include "Client.h"


using namespace std;



const std::string currentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);

    return buf;
}
class Client {
private:

    int numPort;
    int sock;
    int secondConnect;
    struct sockaddr_in addr;
public:
    string nameClient;
    Client(string name, int port, int timeConnect) : nameClient(name), numPort(port), secondConnect(timeConnect) {};


    bool openConn() {

        sock = socket(AF_INET, SOCK_STREAM, 0);
        if (sock < 0)
        {
            perror("socket");
            return false;
            //exit(1);
        }

        addr.sin_family = AF_INET;
        addr.sin_port = htons(numPort);
        addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);

        if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) < 0)
        {
            perror("connect");
            return false;
            //exit(2);
        }
        return true;
    }
    void sendMessage(string mess) {
        
        if (*(mess.end() - 1) != '\n') {
            mess += '\n';
        }
        
        char* buf = new char[mess.size()];
        send(sock, mess.c_str(), mess.size(), 0);
        recv(sock, buf, mess.size(), 0);
        delete[] buf;
    }

    void closeConn() {
        close(sock);
        sleep(secondConnect);
    }
};
int main(int argc, char* argv[])
{

    int port = atoi(argv[2]);
    int time = atoi(argv[3]);
    Client c = { argv[1], port, time };

    while (c.openConn()) {
        c.sendMessage('[' + currentDateTime() + ']' + c.nameClient);
        c.closeConn();
    }

    return 0;
}
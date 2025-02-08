#ifndef CONNECTION_H
#define CONNECTION_H

#include <string>

namespace client {
    // TLS sertifikası, asenkron I/O, idle timeout, yeniden gönderme mekanizması entegre edilmiş bağlantı fonksiyonları.
    void connectToServer(const std::string &serverIP, int port);
    bool isConnected();
    void sendDataToServer(const std::string &data);
    std::string receiveDataFromServer();
}

#endif // CONNECTION_H

#ifndef TCP_IP_H
#define TCP_IP_H

namespace network {
    // Gerçek asenkron I/O, TLS desteği, paket yeniden gönderme ve kötü niyetli istemci tespiti içeren TCP/IP sunucusunu başlatır.
    void startTCPServer(int port);
}

#endif // TCP_IP_H

#ifndef WEBSOCKET_H
#define WEBSOCKET_H

namespace network {
    // Gerçek asenkron I/O, TLS desteği, yeniden gönderme ve hata yönetimi eklenmiş WebSocket sunucusunu başlatır.
    void startWebSocketServer(int port);
}

#endif // WEBSOCKET_H

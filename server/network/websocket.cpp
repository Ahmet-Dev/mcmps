#include "websocket.h"
#include <websocketpp/config/asio_no_tls.hpp> // Üretimde TLS destekli config kullanılmalı (örneğin, asio_tls)
#include <websocketpp/server.hpp>
#include <iostream>
#include <functional>
#include <thread>
#include <chrono>
#include <mutex>

namespace network {

typedef websocketpp::server<websocketpp::config::asio> ws_server;

class WebSocketServer {
public:
    WebSocketServer() {
        server.init_asio();
        // Gerçek üretimde TLS entegrasyonu için TLS callback’leri entegre edilir.
        server.clear_access_channels(websocketpp::log::alevel::all);
        server.clear_error_channels(websocketpp::log::elevel::all);
        server.set_open_handler(std::bind(&WebSocketServer::on_open, this, std::placeholders::_1));
        server.set_close_handler(std::bind(&WebSocketServer::on_close, this, std::placeholders::_1));
        server.set_message_handler(std::bind(&WebSocketServer::on_message, this, std::placeholders::_1, std::placeholders::_2));
    }

    void run(int port) {
        try {
            server.set_reuse_addr(true);
            server.listen(port);
            server.start_accept();
            std::cout << "[WEBSOCKET] TLS destekli WebSocket sunucusu port " << port << " üzerinde başlatıldı." << std::endl;
            server.run();
        } catch (const std::exception &e) {
            std::cerr << "[WEBSOCKET] Hata: " << e.what() << std::endl;
        }
    }

private:
    void on_open(websocketpp::connection_hdl hdl) {
        std::cout << "[WEBSOCKET] Yeni bağlantı açıldı." << std::endl;
        // Üretimde: Bağlantı açıldığında idle timeout ve kötü niyetli istemci kontrolü yapılabilir.
    }

    void on_close(websocketpp::connection_hdl hdl) {
        std::cout << "[WEBSOCKET] Bağlantı kapandı." << std::endl;
    }

    void on_message(websocketpp::connection_hdl hdl, ws_server::message_ptr msg) {
        std::cout << "[WEBSOCKET] Mesaj alındı: " << msg->get_payload() << std::endl;
        try {
            // Üretimde: Mesaj içerikleri, yeniden gönderme ve ACK mekanizmaları ile işlenir.
            server.send(hdl, msg->get_payload(), msg->get_opcode());
        } catch (const websocketpp::exception &e) {
            std::cerr << "[WEBSOCKET] Mesaj gönderme hatası: " << e.what() << std::endl;
        }
    }

    ws_server server;
};

void startWebSocketServer(int port) {
    WebSocketServer wsServer;
    wsServer.run(port);
}

} // namespace network

#include "connection.h"
#include "error_handler.h"
#include <boost/asio.hpp>
#include <boost/asio/ssl.hpp>
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

using boost::asio::ip::tcp;
namespace asio = boost::asio;

namespace client {

class ConnectionManager {
public:
    ConnectionManager(const std::string& serverIP, int port)
        : io_context(), ssl_context(asio::ssl::context::sslv23_client),
          socket(io_context, ssl_context), deadline_timer(io_context),
          serverIP(serverIP), port(port), connected(false)
    {
        // TLS sertifika doğrulaması için OpenSSL ayarları
        ssl_context.set_default_verify_paths();
        // Gerçek sertifika dosyalarını yükleyin:
        try {
            ssl_context.use_certificate_file("client.crt", asio::ssl::context::pem);
            ssl_context.use_private_key_file("client.key", asio::ssl::context::pem);
        } catch (std::exception& e) {
            logError(std::string("[CONNECTION] Sertifika yükleme hatası: ") + e.what());
        }
        socket.set_verify_mode(asio::ssl::verify_peer);
    }

    void connect() {
        try {
            tcp::resolver resolver(io_context);
            auto endpoints = resolver.resolve(serverIP, std::to_string(port));
            asio::async_connect(socket.lowest_layer(), endpoints,
                [this](boost::system::error_code ec, tcp::endpoint) {
                    if (!ec) {
                        socket.async_handshake(asio::ssl::stream_base::client,
                            [this](boost::system::error_code ec) {
                                std::lock_guard<std::mutex> lock(connMutex);
                                if (!ec) {
                                    connected = true;
                                    std::cout << "[CONNECTION] TLS handshake başarılı." << std::endl;
                                } else {
                                    logError("[CONNECTION] TLS handshake hatası: " + ec.message());
                                }
                                connCv.notify_all();
                            });
                    } else {
                        logError("[CONNECTION] Bağlantı hatası: " + ec.message());
                        connCv.notify_all();
                    }
                });
            // Idle timeout: 5 dakika
            deadline_timer.expires_after(std::chrono::seconds(300));
            deadline_timer.async_wait([this](boost::system::error_code ec) {
                if (!ec) {
                    std::lock_guard<std::mutex> lock(connMutex);
                    if (!connected) {
                        logError("[CONNECTION] Bağlantı zaman aşımına uğradı.");
                    }
                }
            });
            io_thread = std::thread([this]() { io_context.run(); });
        } catch (std::exception &e) {
            logError(std::string("[CONNECTION] Exception: ") + e.what());
        }
    }

    bool isConnected() {
        std::unique_lock<std::mutex> lock(connMutex);
        return connected;
    }

    void sendData(const std::string &data) {
        try {
            asio::async_write(socket, asio::buffer(data),
                [this, data](boost::system::error_code ec, std::size_t /*length*/) {
                    if (ec) {
                        logError("[CONNECTION] Veri gönderme hatası: " + ec.message());
                        retrySendData(data, 3);
                    }
                });
        } catch (std::exception &e) {
            logError(std::string("[CONNECTION] Send Exception: ") + e.what());
        }
    }

    void retrySendData(const std::string &data, int retries) {
        if (retries <= 0) {
            logError("[CONNECTION] Yeniden gönderme sınırına ulaşıldı.");
            return;
        }
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::cout << "[CONNECTION] Veri yeniden gönderiliyor, kalan deneme: " << retries << std::endl;
        sendData(data);
    }

    std::string receiveData() {
        try {
            char buf[1024];
            size_t len = socket.read_some(asio::buffer(buf));
            return std::string(buf, len);
        } catch (std::exception &e) {
            logError(std::string("[CONNECTION] Veri alma hatası: ") + e.what());
            return "";
        }
    }

    ~ConnectionManager() {
        try {
            socket.lowest_layer().close();
            io_context.stop();
            if (io_thread.joinable())
                io_thread.join();
        } catch (...) {}
    }
private:
    asio::io_context io_context;
    asio::ssl::context ssl_context;
    asio::ssl::stream<tcp::socket> socket;
    asio::steady_timer deadline_timer;
    std::string serverIP;
    int port;
    std::atomic<bool> connected;
    std::mutex connMutex;
    std::condition_variable connCv;
    std::thread io_thread;
};

static ConnectionManager* connManager = nullptr;

void connectToServer(const std::string &serverIP, int port) {
    if (!connManager) {
        connManager = new ConnectionManager(serverIP, port);
        connManager->connect();
    }
}

bool isConnected() {
    return connManager ? connManager->isConnected() : false;
}

void sendDataToServer(const std::string &data) {
    if (connManager && connManager->isConnected()) {
        connManager->sendData(data);
    }
}

std::string receiveDataFromServer() {
    if (connManager && connManager->isConnected()) {
        return connManager->receiveData();
    }
    return "";
}

} // namespace client

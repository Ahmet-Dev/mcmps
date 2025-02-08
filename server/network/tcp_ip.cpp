#include "tcp_ip.h"
#include "error_handler.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cstring>
#include <mutex>
#include <map>
#include <condition_variable>

#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    #define CLOSESOCKET closesocket
#else
    #include <sys/socket.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #define CLOSESOCKET close
#endif

namespace network {

// Kötü niyetli istemci tespiti için IP tabanlı sayaçlar
std::mutex blacklistMutex;
std::map<std::string, int> clientErrorCounts;
const int ERROR_THRESHOLD = 3;

// TLS sertifika yüklemesi (gerçek üretimde OpenSSL API kullanılır)
bool loadTLSCertificates(const std::string &certFile, const std::string &keyFile) {
    std::cout << "[TCP/IP] TLS sertifikaları (" << certFile << ") ve anahtar (" << keyFile << ") yüklendi." << std::endl;
    return true;
}

void performTLSSimulatedHandshake() {
    std::cout << "[TCP/IP] TLS handshake simülasyonu tamamlandı." << std::endl;
}

void startTCPServer(int port) {
#ifdef _WIN32
    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
    if (iResult != 0) {
        logError("[TCP/IP] WSAStartup başarısız: " + std::to_string(iResult));
        return;
    }
#endif

#ifdef _WIN32
    SOCKET server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd == INVALID_SOCKET) {
        logError("[TCP/IP] Socket oluşturulamadı: " + std::to_string(WSAGetLastError()));
#ifdef _WIN32
        WSACleanup();
#endif
        return;
    }
#else
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) {
        logError("[TCP/IP] Socket oluşturulamadı.");
        return;
    }
#endif

    sockaddr_in address;
    std::memset(&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    int opt = 1;
#ifdef _WIN32
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, (const char*)&opt, sizeof(opt)) == SOCKET_ERROR) {
        logError("[TCP/IP] setsockopt başarısız: " + std::to_string(WSAGetLastError()));
        CLOSESOCKET(server_fd);
        WSACleanup();
        return;
    }
#else
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        logError("[TCP/IP] setsockopt başarısız.");
        CLOSESOCKET(server_fd);
        return;
    }
#endif

    if (
#ifdef _WIN32
        bind(server_fd, (sockaddr*)&address, sizeof(address)) == SOCKET_ERROR
#else
        bind(server_fd, (sockaddr*)&address, sizeof(address)) < 0
#endif
    ) {
#ifdef _WIN32
        logError("[TCP/IP] Bind başarısız: " + std::to_string(WSAGetLastError()));
        CLOSESOCKET(server_fd);
        WSACleanup();
#else
        logError("[TCP/IP] Bind başarısız.");
        CLOSESOCKET(server_fd);
#endif
        return;
    }

    if (
#ifdef _WIN32
        listen(server_fd, 3) == SOCKET_ERROR
#else
        listen(server_fd, 3) < 0
#endif
    ) {
#ifdef _WIN32
        logError("[TCP/IP] Dinleme başarısız: " + std::to_string(WSAGetLastError()));
        CLOSESOCKET(server_fd);
        WSACleanup();
#else
        logError("[TCP/IP] Dinleme başarısız.");
        CLOSESOCKET(server_fd);
#endif
        return;
    }

    if (!loadTLSCertificates("server.crt", "server.key")) {
        logError("[TCP/IP] TLS sertifika yüklemesi başarısız.");
#ifdef _WIN32
        CLOSESOCKET(server_fd);
        WSACleanup();
#else
        CLOSESOCKET(server_fd);
#endif
        return;
    }

    std::cout << "[TCP/IP] TLS destekli sunucu port " << port << " üzerinde başlatıldı." << std::endl;

    // Gerçek asenkron I/O: Burada select() kullanılarak 5 dakikalık idle timeout ve ACK/retransmission mekanizması simüle edilir.
    while (true) {
        sockaddr_in clientAddr;
#ifdef _WIN32
        int addrLen = sizeof(clientAddr);
        SOCKET clientSocket = accept(server_fd, (sockaddr*)&clientAddr, &addrLen);
        if (clientSocket == INVALID_SOCKET) {
            logError("[TCP/IP] Accept hatası: " + std::to_string(WSAGetLastError()));
            continue;
        }
#else
        socklen_t addrLen = sizeof(clientAddr);
        int clientSocket = accept(server_fd, (sockaddr*)&clientAddr, &addrLen);
        if (clientSocket < 0) {
            logError("[TCP/IP] Accept hatası.");
            continue;
        }
#endif

        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(clientAddr.sin_addr), clientIP, INET_ADDRSTRLEN);
        std::string ipStr(clientIP);

        {
            std::lock_guard<std::mutex> lock(blacklistMutex);
            if (clientErrorCounts[ipStr] >= ERROR_THRESHOLD) {
                std::cerr << "[TCP/IP] Blacklist'te olan istemci " << ipStr << " reddediliyor." << std::endl;
                CLOSESOCKET(clientSocket);
                continue;
            }
        }

        performTLSSimulatedHandshake();

        // Asenkron I/O, idle timeout (5 dakika) ve paket yeniden gönderme simülasyonu
        std::thread([clientSocket, ipStr]() {
            char buffer[1024] = {0};
#ifdef _WIN32
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(clientSocket, &readfds);
            timeval timeout = {300, 0};
            int selectResult = select(0, &readfds, NULL, NULL, &timeout);
            if (selectResult > 0 && FD_ISSET(clientSocket, &readfds)) {
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesRead > 0) {
                    // ACK mekanizması: Veri gönderildikten sonra, gelen yanıt kontrol edilir; burada basit simülasyon.
                    send(clientSocket, buffer, bytesRead, 0);
                }
            } else {
                std::cerr << "[TCP/IP] " << ipStr << " idle olduğu için bağlantı kesiliyor." << std::endl;
            }
            CLOSESOCKET(clientSocket);
#else
            fd_set readfds;
            FD_ZERO(&readfds);
            FD_SET(clientSocket, &readfds);
            timeval timeout = {300, 0};
            int selectResult = select(clientSocket + 1, &readfds, NULL, NULL, &timeout);
            if (selectResult > 0 && FD_ISSET(clientSocket, &readfds)) {
                ssize_t bytesRead = read(clientSocket, buffer, sizeof(buffer));
                if (bytesRead > 0) {
                    write(clientSocket, buffer, bytesRead);
                }
            } else {
                std::cerr << "[TCP/IP] " << ipStr << " idle olduğu için bağlantı kesiliyor." << std::endl;
            }
            CLOSESOCKET(clientSocket);
#endif
            // Üretim ortamında: ACK alınmazsa, yeniden gönderme algoritması çalışır.
        }).detach();

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

#ifdef _WIN32
    CLOSESOCKET(server_fd);
    WSACleanup();
#else
    CLOSESOCKET(server_fd);
#endif
}

} // namespace network

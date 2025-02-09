#include <iostream>
#include <cassert>
#include <thread>
#include "connection.h"
#include "auth.h"
#include "error_handler.h"

int main()
{
    std::cout << "[TEST CLIENT] Başlatılıyor..." << std::endl;
    client::connectToServer("127.0.0.1", 1881);
    while (!client::isConnected())
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    std::cout << "[TEST CLIENT] Bağlantı testi başarılı." << std::endl;
    std::string testData = "Test veri\n";
    client::sendDataToServer(testData);
    std::string received = client::receiveDataFromServer();
    std::cout << "[TEST CLIENT] Gönderilen: " << testData << ", Alınan: " << received << std::endl;
    bool authResult = clientAuthenticate("dummy_token");
    assert(!authResult);
    std::cout << "[TEST CLIENT] Tüm testler başarılı." << std::endl;
    return 0;
}

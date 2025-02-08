// client/worker.cpp
#include "worker.h"
#include "connection.h"
#include "auth.h"
#include "execution.h"
#include "gpu_support.h"
#include "error_handler.h"
#include <nlohmann/json.hpp>
#include <fstream>
#include <iostream>
#include <thread>
#include <chrono>

using json = nlohmann::json;

int main() {
    std::ifstream configFile("client/client_config.json");
    if (!configFile.is_open()) {
        std::cerr << "[WORKER] client_config.json açılamadı!" << std::endl;
        return 1;
    }
    json config;
    configFile >> config;
    std::string serverIP = config["server_ip"];
    int port = config["port"];
    std::string jwtToken = config["jwt_token"];
    
    client::connectToServer(serverIP, port);
    while (!client::isConnected()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    
    if (!clientAuthenticate(jwtToken)) {
        std::cerr << "[WORKER] Kimlik doğrulaması başarısız. Çıkılıyor." << std::endl;
        return 1;
    }
    
    std::string receivedMachineCode = client::receiveDataFromServer();
    if (receivedMachineCode.empty()) {
        std::cerr << "[WORKER] Sunucudan makine kodu alınamadı." << std::endl;
    } else {
        if (isCUDAAvailable() || isOpenCLAvailable() || isDirectXAvailable() || isVulkanAvailable()) {
            executeMachineCode(receivedMachineCode);
        } else {
            executeMachineCode(receivedMachineCode);
        }
        std::string executionResult = "execution_result\n";
        client::sendDataToServer(executionResult);
    }
    
    std::cout << "[WORKER] İşlem tamamlandı." << std::endl;
    return 0;
}

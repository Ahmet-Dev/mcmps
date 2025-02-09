# Machine Code Multi Processing System

This project presents the basic framework of a system that sends programs from the server to clients as machine code and performs distributed computations using the CPU and GPU power of the clients.

## Features
- Work distribution with Round Robin, Work Stealing, Performance-Based Allocation algorithms.
- Virtual CPU, RAM (min. 8GB) and GPU resource management.
- Secure communication with TLS encryption over TCP/IP and WebSockets.
- Use of JWT for client authentication.
- Performance monitoring and error logging mechanisms.

## Technologies Used
- **Languages:** C, C++, Assembly
- **Networking:** TCP/IP, WebSockets
- **Encryption:** AES-256, SHA-256, TLS 1.3
- **Parallel Processing:** OpenMP, SIMD, TBB
- **GPU Acceleration:** CUDA, OpenCL, Vulkan

# Machine Code Multi Processing System Architecture

The system consists of the following five layers:

1. **Client Layer:** Clients run machine codes coming from the server on the CPU/GPU.
2. **Communication Layer:** Secure data transfer is provided over TCP/IP and WebSockets.
3. **Workload Management Layer:** Round Robin, Work Stealing and Performance-Based Allocation algorithms are used.
4. **Virtualized Resource Layer:** Virtual CPU, RAM (min. 8GB) and GPU drivers are managed.

5. **Security and Monitoring Layer:** TLS, AES-256, JWT verification, error logging and performance monitoring are performed.

# Installation Guide

## Requirements
- A C++ compiler with C++17 support
- CMake 3.10 or later
- OpenSSL libraries

## Build Steps
1. Create a `build` folder in the project directory:
`mkdir build && cd build`
2. Configure with CMake:
`cmake ..`
3. Build the project:
`make`

## Operation
- To start the server: `./server`
- To start the client: `./client`

# API Descriptions

## Authentication
- `bool verifyJWT(const std::string &token);`
Verifies the JWT token on the server side.

- `bool clientAuthenticate(const std::string &jwtToken);`
Authenticates the client with the JWT token.

## Workload Distribution
- `void distributeTasksRoundRobin();`

- `void distributeTasksWorkStealing();`

- `void distributeTasksPerformanceBased();`

## Virtual Resources
- `void initializeVirtualCPU();`

- `void initializeVirtualRAM();`

- `void initializeVirtualGPU();`

# Security Policies

## Data Encryption
- AES-256 is used for data encryption.
- SHA-256 hash algorithm is applied for data integrity.

## Communication Security
- Secure data transfer is provided with TLS 1.3.
- TCP/IP and WebSocket connections are encrypted over TLS.

## Authentication
- Clients are verified with JWT tokens.
- Client connection is disconnected if there is no action for 5 minutes.

# Türkçe

Bu proje, sunucudaki programları makine kodu olarak istemcilere gönderip, istemcilerin CPU ve GPU gücünü kullanarak dağıtık hesaplamalar yapan bir sistemin temel iskeletini sunar.

## Özellikler
- Round Robin, Work Stealing, Performance-Based Allocation algoritmaları ile iş dağıtımı.
- Sanal CPU, RAM (min. 8GB) ve GPU kaynak yönetimi.
- TCP/IP ve WebSockets üzerinden TLS şifrelemeli güvenli iletişim.
- İstemci kimlik doğrulaması için JWT kullanımı.
- Performans izleme ve hata loglama mekanizmaları.

## Kullanılan Teknolojiler
- **Diller:** C, C++, Assembly
- **Ağ İletişimi:** TCP/IP, WebSockets
- **Şifreleme:** AES-256, SHA-256, TLS 1.3
- **Paralel İşlem:** OpenMP, SIMD, TBB
- **GPU Hızlandırma:** CUDA, OpenCL, Vulkan

# Machine Code Multi Processing System Mimarisi

Sistem, aşağıdaki beş katmandan oluşmaktadır:

1. **İstemci Katmanı:** İstemciler, sunucudan gelen makine kodlarını CPU/GPU üzerinde çalıştırır.
2. **İletişim Katmanı:** TCP/IP ve WebSockets üzerinden güvenli veri transferi sağlanır.
3. **İş Yükü Yönetim Katmanı:** Round Robin, Work Stealing ve Performance-Based Allocation algoritmaları kullanılır.
4. **Sanallaştırılmış Kaynak Katmanı:** Sanal CPU, RAM (min. 8GB) ve GPU sürücüleri yönetilir.
5. **Güvenlik ve İzleme Katmanı:** TLS, AES-256, JWT doğrulama, hata loglama ve performans izleme işlemleri yapılır.

# Kurulum Rehberi

## Gereksinimler
- C++17 desteğine sahip bir C++ derleyicisi
- CMake 3.10 veya üstü
- OpenSSL kütüphaneleri

## Derleme Adımları
1. Proje dizininde bir `build` klasörü oluşturun:  
   `mkdir build && cd build`
2. CMake ile yapılandırın:  
   `cmake ..`
3. Projeyi derleyin:  
   `make`

## Çalıştırma
- Sunucuyu başlatmak için: `./server`
- İstemciyi başlatmak için: `./client`

# API Açıklamaları

## Authentication
- `bool verifyJWT(const std::string &token);`  
  Sunucu tarafında JWT token doğrulaması yapar.

- `bool clientAuthenticate(const std::string &jwtToken);`  
  İstemcide JWT token ile kimlik doğrulaması yapar.

## İş Yükü Dağıtımı
- `void distributeTasksRoundRobin();`  
- `void distributeTasksWorkStealing();`  
- `void distributeTasksPerformanceBased();`  

## Sanal Kaynaklar
- `void initializeVirtualCPU();`  
- `void initializeVirtualRAM();`  
- `void initializeVirtualGPU();`  

# Güvenlik Politikaları

## Veri Şifreleme
- Verilerin şifrelenmesi için AES-256 kullanılır.
- Veri bütünlüğü için SHA-256 hash algoritması uygulanır.

## İletişim Güvenliği
- TLS 1.3 ile güvenli veri aktarımı sağlanır.
- TCP/IP ve WebSocket bağlantıları TLS üzerinden şifrelenir.

## Kimlik Doğrulama
- İstemciler JWT token ile doğrulanır.
- 5 dakika boyunca işlem yapılmazsa istemci bağlantısı kesilir.

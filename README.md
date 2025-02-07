# Machine Code Multi Processing System

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

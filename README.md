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

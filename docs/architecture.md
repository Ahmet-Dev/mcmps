# Machine Code Multi Processing System Mimarisi

Sistem, aşağıdaki beş katmandan oluşmaktadır:

1. **İstemci Katmanı:** İstemciler, sunucudan gelen makine kodlarını CPU/GPU üzerinde çalıştırır.
2. **İletişim Katmanı:** TCP/IP ve WebSockets üzerinden güvenli veri transferi sağlanır.
3. **İş Yükü Yönetim Katmanı:** Round Robin, Work Stealing ve Performance-Based Allocation algoritmaları kullanılır.
4. **Sanallaştırılmış Kaynak Katmanı:** Sanal CPU, RAM (min. 8GB) ve GPU sürücüleri yönetilir.
5. **Güvenlik ve İzleme Katmanı:** TLS, AES-256, JWT doğrulama, hata loglama ve performans izleme işlemleri yapılır.

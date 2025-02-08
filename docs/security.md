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

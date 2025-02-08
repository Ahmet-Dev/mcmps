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

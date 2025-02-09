#include "CodeManager.hpp"
#include "virtual_ram.hpp"
#include <sstream>
#include <iostream>

CodeManager::CodeManager(core::VirtualRAM *ram)
    : virtualRam(ram), currentIndex(0)
{
    loadCodeSegments();
}

void CodeManager::loadCodeSegments()
{
    // Simülasyon: VirtualRAM'de saklanan veriden sabit bir string kullanılıyor.
    std::string simulatedData = "MOV EAX, 1\nMOV EBX, 2\nMOV ECX, 3\n";
    std::istringstream iss(simulatedData);
    std::string line;
    segments.clear();
    while (std::getline(iss, line))
    {
        if (!line.empty())
            segments.push_back(line);
    }
}

std::string CodeManager::getNextCodeSegment()
{
    if (segments.empty())
        return "";
    std::string segment = segments[currentIndex];
    currentIndex = (currentIndex + 1) % segments.size();
    return segment;
}

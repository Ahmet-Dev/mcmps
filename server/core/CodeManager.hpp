#ifndef CODE_MANAGER_HPP
#define CODE_MANAGER_HPP

#include <string>
#include <vector>

namespace core
{
    class VirtualRAM;
}

class CodeManager
{
public:
    CodeManager(core::VirtualRAM *ram);
    void loadCodeSegments();
    std::string getNextCodeSegment();

private:
    core::VirtualRAM *virtualRam;
    std::vector<std::string> segments;
    size_t currentIndex;
};

#endif // CODE_MANAGER_HPP

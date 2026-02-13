#include "rbtrace/backtrace.hpp"
#include "rbtrace/elf.hpp"
#include <iostream>
#include <iomanip>

void dummyRecursion(int depth)
{
    if(depth == 0)
    {
        auto trace = rbtrace::capture_trace();

        std::cout << "Captured " << trace.size << " frames.\n";

        for(size_t i{0}; i < trace.size; ++i)
        {
            uintptr_t addr = trace.frames[i].address;
            rbtrace::BinInfo info;

            std::cout << "[" << i << "] 0x" << std::hex << addr << std::dec;

            if(rbtrace::ELFLoader::getInfo(addr, info))
            {
                uintptr_t offset = addr - info.baseAddr;
                std::cout << "-> " << info.path
                          << " (+0x" << std::hex << offset << ")\n";
            }
            else
            {
                std::cout << "-> ???\n";
            }
            std::cout << '\n';
        }

        return;
    }

    dummyRecursion(depth - 1);
}

void caller()
{
    dummyRecursion(5);
}

int main()
{
    caller();
    return 0;
}

/*
Structure to represent a loaded module (executable/library)
*/

#pragma once
#include <string>
#include <cstdint>
#include <vector>

namespace rbtrace
{
    //Represents a loaded elf bin (executable or shared obj)
    struct BinInfo
    {
        std::string path; // path to the bin
        uintptr_t baseAddr; // Load address in memory

        //More fields tba later
    };

    // Interface to find which bin owns the address
    class ELFLoader
    {
        public:
            
            /*
            Given an absolute address from the stack trace
            find the bin containing it
            and return true if found, populate info
            */

            static bool getInfo(uintptr_t addr, BinInfo &info);
    };
}
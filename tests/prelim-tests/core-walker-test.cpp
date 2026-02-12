#include "rbtrace/backtrace.hpp"
#include <iostream>
#include <iomanip>

void dummyRecursion(int depth)
{
    if(depth == 0)
    {
        auto trace = rbtrace::capture_trace();

        std::cout << "Captured: " << trace.size << " frames.\n";

        for(const auto &frame : trace)
        {
            std::cout << "0x" << std::hex << frame.address << std::dec << '\n';
        }   

        return;
    }

    dummyRecursion(depth - 1);
}

void caller()
{
    dummyRecursion(3);
}

int main()
{
    std::cout << "\nRaw frame walker: \n";
    caller(); 
}
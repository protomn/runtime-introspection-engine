/*
Core header
*/

#pragma once
#include <cstdint>

namespace rbtrace
{
    // Defining a single stack frame
    struct Frame
    {
        uintptr_t address; //Instruction pointer that returns the address

        //Default constructor 
        Frame() : address(0) { }

        explicit Frame(uintptr_t a) : address(a) { }
    };  
}
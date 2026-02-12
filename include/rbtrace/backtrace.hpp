/*
A fixed size buffer for storage
Does not use malloc or new due to future hot path implementation
*/

#pragma once
#include "frame.hpp"
#include <array>
#include <cstddef>

namespace rbtrace
{
    /*
    Defining the maximum capture depth
    64 typically sufficient, 128 for potential deep recursion
    */
    constexpr size_t MAX_DEPTH = 128;

    struct Trace
    {
        std::array<Frame, MAX_DEPTH> frames;
        size_t size{0};

        void push_back(uintptr_t addr)
        {
            if(size < MAX_DEPTH)
            {
                frames[size++] = Frame(addr);
            }
        }

        //Iteration helpers
        auto begin() const { return frames.begin(); }
        auto end() const { return frames.end(); }
    };

    //Core function
    Trace capture_trace();
}
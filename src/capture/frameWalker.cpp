/*
FrameWalker class to traverse the RBP chain
*/

#include "rbtrace/backtrace.hpp"
#include <cstdint>

namespace rbtrace
{
    __attribute__((noinline))
    Trace capture_trace()
    {
        Trace trace;

        //Get current base pointer
        //__builtin_frame_address(0) returns the frame pointer of the current function
        void **rbp = static_cast<void **>(__builtin_frame_address(0));

        //Walk the chain
        /*
        Loop is limited to prevent inf loops in case of corrupted stacks
        */
        for(size_t i{0}; i < MAX_DEPTH; ++i)
        {
            /*
            If RBP is null or unaligned, stop
            Stack grows downwards so nextRBP > currRBP
            Can be violated by coroutines/stack switching
            */
            if(!rbp)
            {
                break;
            }

            /*
            Return addr stored in RBP + 8 bytes
            rbp[0] = prevRBP
            rbp[1] = return addr
            */
            void *return_addr = rbp[1];

            if(!return_addr)
            {
                break;
            }

            trace.push_back(reinterpret_cast<uintptr_t>(return_addr));

            //Move to previous frame
            void **nxtRBP = static_cast<void **>(rbp[0]);

            /*
            Since stack grows from top to bottom, 
            the caller (prev frame) should have a higher address
            if nxtRBp <= rbp -> maybe in a loop or at the top of the stack
            */

            if(nxtRBP <= rbp)
            {
                break;
            }

            rbp = nxtRBP;
        }

        return trace;
    }
}

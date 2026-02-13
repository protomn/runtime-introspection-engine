/*
Code responsible for interfacing with the OS loader
Function dl_iterate_phdr allows for iteration over every loaded shared object.
*/

#include "rbtrace/elf.hpp"
#include <link.h>
#include <dlfcn.h>
#include <cstring>
#include <iostream>

namespace rbtrace
{
    struct SearchContext
    {
        uintptr_t targ_addr;
        BinInfo *result;
        bool found;
    };

    //Callback function for every loaded obj
    static int iterateCallback(struct dl_phdr_info *info, size_t size, void *data)
    {
        (void)size;
        SearchContext *ctx = static_cast<SearchContext*>(data);
        
        //Calculate the addr range for this obj
        //base addr is info -> dlpi_addr
        uintptr_t base = info -> dlpi_addr;

        /*
        Need to check total size of loaded segments to check bounds
        if the dlpi_info api can identify it, we can trust it
        */
       for(int i{0}; i < info -> dlpi_phnum; ++i)
       {
            const ElfW(Phdr) *phdr = &info -> dlpi_phdr[i];

            //Only care about the load segments which are executable
            if(phdr -> p_type == PT_LOAD && (phdr -> p_flags & PF_X))
            {
                uintptr_t start = base + phdr -> p_vaddr;
                uintptr_t end = start + phdr -> p_memsz;

                if(ctx -> targ_addr >= start && ctx -> targ_addr < end)
                {
                    //Found
                    ctx -> result -> baseAddr = base;

                    //dlpi_name is empty for the main executable
                    ctx -> result -> path = (info -> dlpi_name && info -> dlpi_name[0])
                                            ? info -> dlpi_name : "/proc/self/exe";
                    
                    ctx -> found = true;
                    return 1; //Stop iterations
                }
            }
       }

       return 0; //Continue iter
    }

    bool ELFLoader::getInfo(uintptr_t addr, BinInfo &info)
    {
        SearchContext ctx;
        ctx.targ_addr = addr;
        ctx.result = &info;
        ctx.found = false;

        dl_iterate_phdr(iterateCallback, &ctx);

        return ctx.found;
    }
}
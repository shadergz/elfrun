#include <cstring>
#include <iostream>
#include <stdexcept>
#include <print>

#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>

#include <linkable_format.h>
namespace elfrun {
    void LinkableFormat::load(const i32 fd) {
        fstat(fd, &status);

        const auto mapped{static_cast<char*>(mmap(nullptr, status.st_size, PROT_READ, MAP_PRIVATE, fd, 0))};
        if (mapped == MAP_FAILED)
            throw std::runtime_error("mmap failed");

        mappedElf = {reinterpret_cast<u8*>(mapped), static_cast<long unsigned int>(status.st_size)};
        std::memcpy(&header, mapped, sizeof(header));

        programHeaders = {reinterpret_cast<Elf64_Phdr*>(mapped + header.e_phoff), header.e_phnum};
        sectionsHeader = {reinterpret_cast<Elf64_Shdr*>(mapped + header.e_shoff), header.e_shnum};

        const auto& strSection{sectionsHeader[header.e_shstrndx]};
        stringTable = {mapped + strSection.sh_offset, strSection.sh_size};
    }

    void LinkableFormat::link([[maybe_unused]] i32 argc, char **argv) {
        const std::string interpreter{getInterpreter()};
        if (!interpreter.empty()) {
            throw std::runtime_error{"Cannot deal dynamic linked executable"};
        }

        for (const auto& header : programHeaders) {
            if (header.p_type == PT_LOAD) {
                mapExecutable(header);
            }
        }

        if (!entry)
            throw std::runtime_error("The entry point of the binary was not successfully loaded");

        std::print("Binary entry point located at: {}", entry);

        argv[0] = argv[1];
    }

    void LinkableFormat::jump(i32 argc, char** argv) const {
        // Let's use our program's stack
        auto rsp{reinterpret_cast<u64>(&argv[0]) - 0x8};
        u64 pc{(entry)};
        __asm(
            "mov %0, %%rsp;"
            "xor rax, rax;"
            "xor rbx, rbx"
            "mov %1, %%rpc"

            :: "r"(rsp), "r" (pc)
        );

        std::print("Return passed back to the loader");
    }

    LinkableFormat::~LinkableFormat() {
        munmap(mappedElf.data(), status.st_size);

        mappedElf = {};
        programHeaders = {};
        sectionsHeader = {};
        stringTable = {};
    }

    std::string_view LinkableFormat::getInterpreter() const {
        for (const auto& progHeaders : programHeaders) {
            if (progHeaders.p_type == PT_INTERP)
                return {reinterpret_cast<char*>(&mappedElf[0] + progHeaders.p_offset), progHeaders.p_filesz};
        }
        return {};
    }

    constexpr u64 pageRoundDown(const u64 address) {
        return address & ~(4096 - 1);
    }
    constexpr u64 pageRoundUp(const u64 address) {
        return (address + 4096 - 1) & ~(4096 - 1);
    }

    void LinkableFormat::mapExecutable(const Elf64_Phdr& header) {
        u64 minAddr{std::min(header.p_vaddr, static_cast<decltype(minAddr)>(-1))};
        u64 maxAddr{std::max(header.p_vaddr + header.p_memsz, static_cast<decltype(maxAddr)>(-1))};

        const u64 mapStart{pageRoundDown(minAddr)};
        const u64 mapSz{pageRoundUp(maxAddr - mapStart)};
        const auto program{mmap(reinterpret_cast<void*>(mapStart), mapSz,
            PROT_READ | PROT_WRITE | PROT_EXEC,
            MAP_SHARED | MAP_ANONYMOUS, -1, 0)};
        if (program == MAP_FAILED) {
            throw std::runtime_error("mmap failed");
        }
        std::print(
            ".rodata, .data, .text, and .bss sections mapped "
            "at memory address: {}", mapStart);

        // header.p_vaddr should now be mapped correctly in memory
        std::memcpy(reinterpret_cast<void*>(header.p_vaddr), &mappedElf[0] + header.p_offset, header.p_memsz);

        if (header.p_memsz > header.p_filesz) {
            // Clearing the .bss section of the binary
            std::memset(reinterpret_cast<void*>(header.p_vaddr + header.p_filesz), 0, header.p_filesz);
        }

        entry = this->header.e_entry;
    }
}

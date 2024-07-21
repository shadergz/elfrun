#pragma once

#include <cstdint>
#include <span>
#include <ehdr.h>
#include <string_view>

namespace elfrun {
    using Status = struct stat;
    using u64 = std::uint64_t;
    using i32 = std::int32_t;
    using u8 = std::uint8_t;

    class LinkableFormat {
    public:
        LinkableFormat() = default;
        void load(i32 fd);
        void link(i32 argc, char** argv);
        void jump(i32 argc, char** argv) const;
        ~LinkableFormat();

    private:
        [[nodiscard]] std::string_view getInterpreter() const;
        void mapExecutable(const Elf64_Phdr& header);

        Status status{};
        std::span<u8> mappedElf;
        Elf64_Ehdr header{};

        std::span<Elf64_Phdr> programHeaders{};
        std::span<Elf64_Shdr> sectionsHeader{};

        std::span<char> stringTable{};
        Elf64_Addr entry{};
    };
}

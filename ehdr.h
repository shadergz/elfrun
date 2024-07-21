#pragma once

namespace elfrun {

    using Elf64_Addr = unsigned long;
    using Elf64_Off = Elf64_Addr;
    using Elf64_Xword = Elf64_Addr;
    using Elf64_Word = unsigned int;
    using Elf64_Half = unsigned short;

    constexpr auto PT_INTERP{3};
    constexpr auto PT_LOAD{1};

#pragma pack(push, 1)
    typedef struct {
        unsigned char e_ident[16]; /* ELF identification */
        Elf64_Half e_type; /* Object file type */
        Elf64_Half e_machine; /* Machine type */
        Elf64_Word e_version; /* Object file version */
        Elf64_Addr e_entry; /* Entry point address */
        Elf64_Off e_phoff; /* Program header offset */
        Elf64_Off e_shoff; /* Section header offset */
        Elf64_Word e_flags; /* Processor-specific flags */
        Elf64_Half e_ehsize; /* ELF header size */
        Elf64_Half e_phentsize; /* Size of program header entry */
        Elf64_Half e_phnum; /* Number of program header entries */
        Elf64_Half e_shentsize; /* Size of section header entry */
        Elf64_Half e_shnum; /* Number of section header entries */
        Elf64_Half e_shstrndx; /* Section name string table index */
    } Elf64_Ehdr;

    typedef struct {
        Elf64_Word p_type; /* Type of segment */
        Elf64_Word p_flags; /* Segment attributes */
        Elf64_Off p_offset; /* Offset in file */
        Elf64_Addr p_vaddr; /* Virtual address in memory */
        Elf64_Addr p_paddr; /* Reserved */
        Elf64_Xword p_filesz; /* Size of segment in file */
        Elf64_Xword p_memsz; /* Size of segment in memory */
        Elf64_Xword p_align; /* Alignment of segment */
    } Elf64_Phdr;

    typedef struct {
        Elf64_Word sh_name; /* Section name */
        Elf64_Word sh_type; /* Section type */
        Elf64_Xword sh_flags; /* Section attributes */
        Elf64_Addr sh_addr; /* Virtual address in memory */
        Elf64_Off sh_offset; /* Offset in file */
        Elf64_Xword sh_size; /* Size of section */
        Elf64_Word sh_link; /* Link to other section */
        Elf64_Word sh_info; /* Miscellaneous information */
        Elf64_Xword sh_addralign; /* Address alignment boundary */
        Elf64_Xword sh_entsize; /* Size of entries, if section has table */
    } Elf64_Shdr;
#pragma pack(pop)
}


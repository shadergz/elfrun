#include <iostream>
#include <filesystem>
#include <fcntl.h>
#include <unistd.h>

#include <linkable_format.h>
int main(int argc, char** argv) {
    if (argc < 2)
        exit(argc);

    const std::string_view pathName{argv[1]};
    {
        // ReSharper disable once CppTooWideScopeInitStatement
        const std::filesystem::path path{pathName};
        if (!exists(path)) {
            exit(2);
        }
    }
    const elfrun::i32 fd{open(pathName.data(), O_RDONLY)};
    if (fd < 3)
        exit(1);

    elfrun::LinkableFormat elf;
    elf.load(fd);
    elf.link(argc, argv);
    elf.jump(argc, argv);

    close(fd);

    return 0;
}

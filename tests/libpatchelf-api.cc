#include "libpatchelf.h"

#include <cstdlib>
#include <filesystem>
#include <stdexcept>
#include <string>
#include <unistd.h>
#include <vector>

static void require(bool condition, const char * message)
{
    if (!condition)
        throw std::runtime_error(message);
}

int main(int argc, char * argv[])
{
    require(argc == 2 || argc == 3, "expected ELF fixture and optional SONAME");

    char name[] = "/tmp/libpatchelf-api-XXXXXX";
    int fd = mkstemp(name);
    require(fd >= 0, "mkstemp failed");
    close(fd);
    const std::filesystem::path copy(name);
    std::filesystem::copy_file(argv[1], copy,
        std::filesystem::copy_options::overwrite_existing);

    const std::string first = "/tmp/libpatchelf-first";
    const std::string second = "/tmp/libpatchelf-second";

    patchelf::ElfFile elf(copy);
    const auto needed = elf.getNeeded();
    const auto soname = elf.getSoname();
    const auto initialRPath = elf.getRPath();
    require(!elf.isChanged(), "fresh ELF reported changed");
    require(!needed.empty(), "fixture has no dynamic dependencies");
    if (argc == 3)
        require(soname == argv[2], "unexpected SONAME");

    elf.addRPath(first);
    elf.addRPath(second);
    const auto expectedRPath = initialRPath.empty()
        ? first + ":" + second
        : initialRPath + ":" + first + ":" + second;
    require(elf.isChanged(), "pending RPATH did not mark ELF changed");
    require(elf.getRPath() == expectedRPath, "pending RPATH readback failed");
    elf.save();
    require(!elf.isChanged(), "saved ELF reported changed");

    patchelf::ElfFile reopened(copy);
    require(reopened.getRPath() == expectedRPath, "saved RPATH readback failed");
    require(reopened.getNeeded() == needed, "dependencies changed");
    require(reopened.getSoname() == soname, "SONAME changed");
    reopened.save();
    std::filesystem::remove(copy);
}

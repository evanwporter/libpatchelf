#pragma once

#include <filesystem>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace patchelf {

    class ElfFile {
    public:
        explicit ElfFile(const std::filesystem::path& path);
        ~ElfFile();

        ElfFile(ElfFile&&) noexcept;
        ElfFile& operator=(ElfFile&&) noexcept;

        ElfFile(const ElfFile&) = delete;
        ElfFile& operator=(const ElfFile&) = delete;

        [[nodiscard]]
        std::vector<std::string> getNeeded() const;

        [[nodiscard]]
        std::optional<std::string> getSoname() const;

        [[nodiscard]]
        std::string getRPath() const;

        void addRPath(const std::string& directory);

        [[nodiscard]]
        bool isChanged() const noexcept;

        void save();

    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };

}

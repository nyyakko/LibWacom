#include "Command.hpp"

#include <liberror/Try.hpp>
#include <fmt/format.h>

using namespace liberror;
using namespace libwacom;

Result<std::string> xsetwacom::execute(std::string_view command)
{
    std::string output {};
    auto fd = popen(command.data(), "r");
    if (fd == nullptr)
        return make_error("File descriptor for xsetwacom command returned as nullptr");

    while (true)
    {
        std::array<char, 512> buffer {0};
        if (fgets(buffer.data(), buffer.size(), fd) == nullptr) break;
        output.append(buffer.data());
    }

    fclose(fd);

    return output;
}


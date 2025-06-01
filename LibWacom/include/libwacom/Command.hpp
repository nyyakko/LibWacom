#pragma once

#include <liberror/Result.hpp>

#include <string>

namespace libwacom::xsetwacom {

liberror::Result<std::string> execute(std::string_view command);

}

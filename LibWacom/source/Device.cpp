#include "Device.hpp"

#include "Command.hpp"

#include <iostream>
#include <liberror/Try.hpp>

#include <algorithm>
#include <functional>
#include <regex>
#include <sstream>

using namespace liberror;
using namespace libwacom;

Result<std::vector<Device>> libwacom::get_available_devices()
{
    std::vector<Device> devices {};

    auto fnTrim = [] (auto const& value) {
        auto result = value;
        result.erase(result.begin(), std::ranges::find_if(result, std::not_fn(isspace)));
        result.erase(std::find_if(result.rbegin(), result.rend(), std::not_fn(isspace)).base(), result.end());
        return result;
    };

    auto output = TRY(xsetwacom::execute("xsetwacom --list devices"));
    std::regex pattern(R"((.+)\s+id: (\d+)\s+type: (\w+))");
    std::sregex_iterator iterator(output.begin(), output.end(), pattern);
    for (; iterator != std::sregex_iterator{}; iterator = std::next(iterator))
    {
        devices.push_back({
            .name = fnTrim(iterator->str(1)),
            .id = std::atoi(fnTrim(iterator->str(2)).data()),
            .kind = Device::Kind::from_string(fnTrim(iterator->str(3)))
        });
    }

    return devices;
}

Result<Pressure> libwacom::get_stylus_pressure_curve(int stylus)
{
    Pressure pressure {};
    auto command = fmt::format("xsetwacom --get {} PressureCurve", stylus);
    auto output = TRY(xsetwacom::execute(command));
    std::stringstream sstream(output);
    sstream >> pressure.minX >> pressure.minY;
    pressure.minX /= 100.f;
    pressure.minY /= 100.f;
    sstream >> pressure.maxX >> pressure.maxY;
    pressure.maxX /= 100.f;
    pressure.maxY /= 100.f;
    return pressure;
}

Result<void> libwacom::set_stylus_pressure_curve(int stylus, Pressure pressure)
{
    auto command = fmt::format("xsetwacom --set {} PressureCurve {} {} {} {}",
        stylus,
        static_cast<int>(std::round(pressure.minX * 100.f)),
        static_cast<int>(std::round(pressure.minY * 100.f)),
        static_cast<int>(std::round(pressure.maxX * 100.f)),
        static_cast<int>(std::round(pressure.maxY * 100.f))
    );
    TRY(xsetwacom::execute(command));
    return {};
}

Result<int> libwacom::get_stylus_threshold(int stylus)
{
    auto threshold = 0;
    auto command = fmt::format("xsetwacom --get {} Threshold", stylus);
    auto output = TRY(xsetwacom::execute(command));
    std::stringstream sstream(output);
    sstream >> threshold;
    return threshold;
}

Result<void> libwacom::set_stylus_threshold(int stylus, int threshold)
{
    auto command = fmt::format("xsetwacom --set {} Threshold {}", stylus, threshold);
    auto output = TRY(xsetwacom::execute(command));
    return {};
}

Result<int> libwacom::get_stylus_cursor_proximity(int stylus)
{
    auto proximity = 0;
    auto command = fmt::format("xsetwacom --get {} CursorProximity", stylus);
    auto output = TRY(xsetwacom::execute(command));
    std::stringstream sstream(output);
    sstream >> proximity;
    return proximity;
}

Result<void> libwacom::set_stylus_cursor_proximity(int stylus, int proximity)
{
    auto command = fmt::format("xsetwacom --set {} CursorProximity {}", stylus, proximity);
    auto output = TRY(xsetwacom::execute(command));
    return {};
}

Result<Area> libwacom::get_stylus_default_area(int stylus)
{
    auto previousArea = TRY(get_stylus_area(stylus));
    TRY(reset_stylus_area(stylus));
    auto defaultArea = TRY(get_stylus_area(stylus));
    TRY(set_stylus_area(stylus, previousArea));
    return defaultArea;
}

Result<Area> libwacom::get_stylus_area(int stylus)
{
    Area area {};
    auto command = fmt::format("xsetwacom --get {} Area", stylus);
    auto output = TRY(xsetwacom::execute(command));
    std::stringstream sstream(output);
    sstream >> area.offsetX >> area.offsetY;
    sstream >> area.width >> area.height;
    return area;
}

Result<void> libwacom::set_stylus_area(int stylus, Area area)
{
    auto command = fmt::format("xsetwacom --set {} Area {} {} {} {}", stylus, area.offsetX, area.offsetY, area.width, area.height);
    TRY(xsetwacom::execute(command));
    return {};
}

Result<void> libwacom::reset_stylus_area(int stylus)
{
    auto command = fmt::format("xsetwacom --set {} ResetArea", stylus);
    TRY(xsetwacom::execute(command));
    return {};
}

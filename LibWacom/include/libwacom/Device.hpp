#pragma once

#include <liberror/Result.hpp>
#include <libenum/Enum.hpp>

#include <string_view>

namespace libwacom {

struct Device
{
    ENUM_CLASS(Kind, STYLUS, PAD, ERASER, TOUCH) // cppcheck-suppress unknownMacro

    std::string name;
    int id;
    Kind kind;
};

liberror::Result<std::vector<Device>> get_available_devices();

struct Pressure
{
    float minX, minY;
    float maxX, maxY;
};

struct Area
{
    float offsetX, offsetY;
    float width, height;
};

liberror::Result<Pressure> get_stylus_pressure_curve(int stylus);
liberror::Result<void> set_stylus_pressure_curve(int stylus, Pressure pressure);
liberror::Result<int> get_stylus_threshold(int stylus);
liberror::Result<void> set_stylus_threshold(int stylus, int threshold);
liberror::Result<int> get_stylus_cursor_proximity(int stylus);
liberror::Result<void> set_stylus_cursor_proximity(int stylus, int proximity);
liberror::Result<Area> get_stylus_default_area(int stylus);
liberror::Result<Area> get_stylus_area(int stylus);
liberror::Result<void> set_stylus_area(int stylus, Area area);
liberror::Result<void> reset_stylus_area(int stylus);
liberror::Result<void> set_stylus_output_from_display_name(int stylus, std::string_view displayName);
liberror::Result<void> set_stylus_output_from_display_area(int stylus, Area area);

}

#pragma once

#include <liberror/Result.hpp>
#include <libenum/Enum.hpp>

namespace libwacom {

class Device
{
public:
    ENUM_CLASS(Kind, STYLUS, PAD, ERASER, TOUCH)

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
    int offsetX, offsetY;
    int width;
    int height;
};

liberror::Result<Pressure> get_stylus_pressure_curve(int stylus);
liberror::Result<void> set_stylus_pressure_curve(int stylus, Pressure pressure);
liberror::Result<int> get_stylus_threshold(int stylus);
liberror::Result<void> set_stylus_threshold(int stylus, int threshold);
liberror::Result<int> get_stylus_cursor_proximity(int stylus);
liberror::Result<void> set_stylus_cursor_proximity(int stylus, int proximity);
liberror::Result<Area> get_stylus_area(int stylus);
liberror::Result<void> set_stylus_area(int stylus, Area area);
liberror::Result<void> reset_stylus_area(int stylus);

}

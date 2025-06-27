#include "Device.hpp"

#include <fmt/format.h>
#include <liberror/Try.hpp>
#include <range/v3/view.hpp>

#include <sys/wait.h>

#include <algorithm>
#include <functional>
#include <regex>
#include <sstream>

using namespace liberror;
using namespace libwacom;

namespace xsetwacom {

namespace {

std::string read_pipe_output(int pipe)
{
    std::string output {};

    while (true)
    {
        std::array<char, 256> buffer {};
        if (read(pipe, buffer.data(), buffer.size()) <= 0) break;
        output.append(buffer.data());
    }

    return output;
}

liberror::Result<std::pair<std::string, std::string>> execute(std::string command, std::vector<std::string> arguments)
{
    int stdoutPipe[2];
    pipe(stdoutPipe);

    int stderrPipe[2];
    pipe(stderrPipe);

    pid_t forkID = fork();
    if (forkID != 0)
    {
        dup2(stdoutPipe[1], STDOUT_FILENO);
        dup2(stderrPipe[1], STDERR_FILENO);

        std::vector<char*> argumentsData { command.data() };
        // cppcheck-suppress constParameterReference
        std::transform(arguments.begin(), arguments.end(), std::next(argumentsData.begin()), [] (std::string& str) {
            return str.data();
        });

        execvp(command.data(), argumentsData.data());
        return liberror::make_error(strerror(errno));
    }

    close(stdoutPipe[1]);
    close(stderrPipe[1]);

    auto out = read_pipe_output(stdoutPipe[0]);
    auto err = read_pipe_output(stderrPipe[0]);

    close(stdoutPipe[0]);
    close(stderrPipe[0]);
    waitpid(forkID, nullptr, 0);

    return std::make_pair(out, err);
}

}

liberror::Result<std::string> execute(std::string command)
{
    auto [out, err] = TRY(execute("xsetwacom", command | ranges::views::split(' ') | ranges::to<std::vector<std::string>>));
    if (!err.empty()) return liberror::make_error(err);
    return out;
}

}

Result<std::vector<Device>> libwacom::get_available_devices()
{
    std::vector<Device> devices {};

    auto fnTrim = [] (auto const& value) {
        auto result = value;
        result.erase(result.begin(), std::ranges::find_if(result, std::not_fn(isspace)));
        result.erase(std::find_if(result.rbegin(), result.rend(), std::not_fn(isspace)).base(), result.end());
        return result;
    };

    auto output = TRY(xsetwacom::execute("--list devices"));
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
    auto command = fmt::format("--get {} PressureCurve", stylus);
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
    auto command = fmt::format("--set {} PressureCurve {} {} {} {}",
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
    auto command = fmt::format("--get {} Threshold", stylus);
    auto output = TRY(xsetwacom::execute(command));
    std::stringstream sstream(output);
    sstream >> threshold;
    return threshold;
}

Result<void> libwacom::set_stylus_threshold(int stylus, int threshold)
{
    auto command = fmt::format("--set {} Threshold {}", stylus, threshold);
    TRY(xsetwacom::execute(command));
    return {};
}

Result<int> libwacom::get_stylus_cursor_proximity(int stylus)
{
    auto proximity = 0;
    auto command = fmt::format("--get {} CursorProximity", stylus);
    auto output = TRY(xsetwacom::execute(command));
    std::stringstream sstream(output);
    sstream >> proximity;
    return proximity;
}

Result<void> libwacom::set_stylus_cursor_proximity(int stylus, int proximity)
{
    auto command = fmt::format("--set {} CursorProximity {}", stylus, proximity);
    TRY(xsetwacom::execute(command));
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
    auto command = fmt::format("--get {} Area", stylus);
    auto output = TRY(xsetwacom::execute(command));
    std::stringstream sstream(output);
    sstream >> area.offsetX >> area.offsetY;
    sstream >> area.width >> area.height;
    return area;
}

Result<void> libwacom::set_stylus_area(int stylus, Area area)
{
    auto command = fmt::format("--set {} Area {} {} {} {}",
        stylus,
        std::round(area.offsetX),
        std::round(area.offsetY),
        std::round(area.width),
        std::round(area.height)
    );
    TRY(xsetwacom::execute(command));
    return {};
}

Result<void> libwacom::reset_stylus_area(int stylus)
{
    auto command = fmt::format("--set {} ResetArea", stylus);
    TRY(xsetwacom::execute(command));
    return {};
}

Result<void> libwacom::set_stylus_output_from_display_name(int stylus, std::string_view displayName)
{
    auto command = fmt::format("--set {} MapToOutput {}", stylus, displayName);
    TRY(xsetwacom::execute(command));
    return {};
}

Result<void> libwacom::set_stylus_output_from_display_area(int stylus, Area area)
{
    auto command = fmt::format("--set {} MapToOutput {}x{}+{}+{}",
        stylus,
        static_cast<int>(std::round(area.width)),
        static_cast<int>(std::round(area.height)),
        static_cast<int>(std::round(area.offsetX)),
        static_cast<int>(std::round(area.offsetY))
    );
    TRY(xsetwacom::execute(command));
    return {};
}

liberror::Result<void> libwacom::set_stylus_handedness(int stylus, Handedness handedness)
{
    std::string command {};
    switch (handedness)
    {
    case Handedness::LEFT: {
        command = fmt::format("--set {} Rotate 3", stylus);
        break;
    }
    case Handedness::RIGHT: {
        command = fmt::format("--set {} Rotate 0", stylus);
        break;
    }
    }

    TRY(xsetwacom::execute(command));

    return {};
}

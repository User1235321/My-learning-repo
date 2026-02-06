#include <iostream>
#include <string>
#include <array>
#include <cstdlib> // Для system()

int main() {
    std::array<std::string, 1> commands = {
        "xdotool key Caps_Lock",  // Включает Caps Lock
    };

    for (const auto& command : commands) {
        int result = std::system(command.c_str());
        if (result == -1) {
            std::cerr << "Ошибка выполнения команды: " << command << std::endl;
            return 1;
        }
        std::cout << "Команда '" << command << "' выполнена." << std::endl;
    }

    return 0;
}

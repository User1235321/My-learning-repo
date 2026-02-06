#include <iostream>
#include <string>

int main()
{
    std::string input;
    std::cout << "Write name of input file:\n";
    std::cin >> input;

    std::string output;
    std::cout << "Write name of output file:\n";
    std::cin >> output;

    short low = 0, bottom = 0, medium = 0, high = 0, up = 0;
    std::cout << "Write change to low(200 Gz), bottom(400 Gz), medium(1000 Gz), high(4 KGz) and up(13 KGz) frequencies (from -900 to 900):\n";
    std::cin >> low >> bottom >> medium >> high >> up;

    std::string result = "ffmpeg -i " + input + " -af \"equalizer=f=200:width_type=o:width=200:g=" + std::to_string(low) + ",";
    result += "equalizer=f=400:width_type=o:width=200:g=" + std::to_string(bottom) + ",";
    result += "equalizer=f=1000:width_type=o:width=200:g=" + std::to_string(medium) + ",";
    result += "equalizer=f=4000:width_type=o:width=200:g=" + std::to_string(high) + ",";
    result += "equalizer=f=13000:width_type=o:width=200:g=" + std::to_string(up) + "\" " + output;

    char * finalResult = new char(result.size());
    std::cout << result << '\n';
    for (size_t i = 0; i < result.size(); ++i) finalResult[i] = result[i];
    system(finalResult);
    return 0;
}

#pragma once

#include <SFML/Graphics.hpp>

#include <filesystem>
#include <string>
#include <vector>

struct DataSet
{
    enum class Kind
    {
      Solution,
      Error,
      Time
    };
    Kind kind;
    std::vector<double> x;
    std::vector<double> first;
    std::vector<double> second;
    std::vector<double> error;
};

DataSet readData(const std::filesystem::path & path);
std::filesystem::path findFont();
void renderPlot(const DataSet & data, const std::filesystem::path & output, const std::filesystem::path & fontPath);
void showPlot(const std::filesystem::path & imagePath);

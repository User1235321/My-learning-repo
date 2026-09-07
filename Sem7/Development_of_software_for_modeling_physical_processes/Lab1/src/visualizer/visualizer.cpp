#include "visualizer.hpp"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace
{

std::string trim(std::string value)
{
  const auto first = value.find_first_not_of(" \t\r\n");
  if (first == std::string::npos)
    return {};
  const auto last = value.find_last_not_of(" \t\r\n");
  return value.substr(first, last - first + 1);
}

double number(const std::string & value, std::size_t line, const std::string & name)
{
  try
  {
    std::size_t used = 0;
    const double result = std::stod(trim(value), &used);
    if (used != trim(value).size() || !std::isfinite(result))
      throw std::invalid_argument("number");
    return result;
  }
  catch (...)
  {
    throw std::runtime_error("Строка " + std::to_string(line) + ": неверное число в столбце " + name);
  }
}

std::vector<std::string> fields(const std::string & line)
{
  std::vector<std::string> result;
  std::stringstream stream(line);
  std::string field;
  while (std::getline(stream, field, ','))
    result.push_back(trim(field));
  if (!line.empty() && line.back() == ',')
    result.emplace_back();
  return result;
}

std::string format(double value)
{
  std::ostringstream stream;
  stream << std::setprecision(4) << std::scientific << value;
  return stream.str();
}

void text(sf::RenderTarget & target, sf::Font & font, const std::string & value, unsigned size, sf::Vector2f position,
          sf::Color color = sf::Color(35, 35, 35))
{
  sf::Text label(sf::String::fromUtf8(value.begin(), value.end()), font, size);
  label.setFillColor(color);
  label.setPosition(position);
  target.draw(label);
}

struct Bounds
{
    double xmin, xmax, ymin, ymax;
};

Bounds bounds(const DataSet & data)
{
  Bounds result{std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest(),
                std::numeric_limits<double>::max(), std::numeric_limits<double>::lowest()};
  auto add = [&](double x, double y)
  {
    result.xmin = std::min(result.xmin, x);
    result.xmax = std::max(result.xmax, x);
    result.ymin = std::min(result.ymin, y);
    result.ymax = std::max(result.ymax, y);
  };
  for (std::size_t i = 0; i < data.x.size(); ++i)
  {
    if (data.kind == DataSet::Kind::Solution)
    {
      add(data.x[i], data.first[i]);
      add(data.x[i], data.second[i]);
    }
    else
      add(data.x[i], data.first[i]);
  }
  if (result.xmin == result.xmax)
  {
    result.xmin -= 1.0;
    result.xmax += 1.0;
  }
  if (result.ymin == result.ymax)
  {
    const double padding = std::max(1.0, std::abs(result.ymin) * 0.1);
    result.ymin -= padding;
    result.ymax += padding;
  }
  const double xp = (result.xmax - result.xmin) * 0.05;
  const double yp = (result.ymax - result.ymin) * 0.08;
  return {result.xmin - xp, result.xmax + xp, result.ymin - yp, result.ymax + yp};
}

void line(sf::RenderTarget & target, sf::Vector2f a, sf::Vector2f b, sf::Color color, float width = 2.f)
{
  sf::RectangleShape shape;
  const sf::Vector2f delta = b - a;
  shape.setSize({std::sqrt(delta.x * delta.x + delta.y * delta.y), width});
  shape.setPosition(a);
  shape.setRotation(std::atan2(delta.y, delta.x) * 180.f / 3.14159265f);
  shape.setFillColor(color);
  target.draw(shape);
}

}

DataSet readData(const std::filesystem::path & path)
{
  std::ifstream input(path);
  if (!input)
    throw std::runtime_error("Не удалось открыть CSV-файл: " + path.string());
  std::string header;
  if (!std::getline(input, header))
    throw std::runtime_error("CSV-файл пуст");
  const auto columns = fields(header);
  DataSet data;
  if (columns == std::vector<std::string>{"coordinate", "numerical", "exact", "absolute_error"})
    data.kind = DataSet::Kind::Solution;
  else if (columns == std::vector<std::string>{"intervals", "step", "error"})
    data.kind = DataSet::Kind::Error;
  else if (columns == std::vector<std::string>{"intervals", "time_us"})
    data.kind = DataSet::Kind::Time;
  else
    throw std::runtime_error("Неподдерживаемый заголовок CSV");
  std::string row;
  std::size_t lineNumber = 1;
  while (std::getline(input, row))
  {
    ++lineNumber;
    if (trim(row).empty())
      continue;
    const auto values = fields(row);
    if (values.size() != columns.size())
      throw std::runtime_error("Строка " + std::to_string(lineNumber) + ": неверное число столбцов");
    if (data.kind == DataSet::Kind::Error)
      data.x.push_back(number(values[1], lineNumber, columns[1]));
    else
      data.x.push_back(number(values[0], lineNumber, columns[0]));
    if (data.kind == DataSet::Kind::Solution)
    {
      data.first.push_back(number(values[1], lineNumber, columns[1]));
      data.second.push_back(number(values[2], lineNumber, columns[2]));
      data.error.push_back(number(values[3], lineNumber, columns[3]));
    }
    else if (data.kind == DataSet::Kind::Error)
      data.first.push_back(number(values[2], lineNumber, columns[2]));
    else
      data.first.push_back(number(values[1], lineNumber, columns[1]));
  }
  if (data.x.empty())
    throw std::runtime_error("CSV-файл не содержит данных");
  return data;
}

std::filesystem::path findFont()
{
  const std::vector<std::filesystem::path> roots = {"/usr/share/fonts", "/usr/local/share/fonts"};
  for (const auto & root : roots)
    if (std::filesystem::exists(root))
      for (const auto & entry : std::filesystem::recursive_directory_iterator(root))
        if (entry.is_regular_file() && entry.path().filename() == "DejaVuSans.ttf")
          return entry.path();
  throw std::runtime_error("Не найден системный шрифт DejaVu Sans");
}

void renderPlot(const DataSet & data, const std::filesystem::path & output, const std::filesystem::path & fontPath)
{
  sf::RenderTexture canvas;
  if (!canvas.create(1400, 900))
    throw std::runtime_error("Не удалось создать графический контекст SFML");
  sf::Font font;
  if (!font.loadFromFile(fontPath.string()))
    throw std::runtime_error("Не удалось загрузить шрифт: " + fontPath.string());
  canvas.clear(sf::Color::White);
  const sf::Vector2f origin(135.f, 750.f), size(1170.f, 610.f);
  const Bounds box = bounds(data);
  auto point = [&](double x, double y)
  {
    return sf::Vector2f(origin.x + static_cast<float>((x - box.xmin) / (box.xmax - box.xmin) * size.x),
                        origin.y - static_cast<float>((y - box.ymin) / (box.ymax - box.ymin) * size.y));
  };
  for (int i = 0; i <= 10; ++i)
  {
    const float px = origin.x + size.x * i / 10.f, py = origin.y - size.y * i / 10.f;
    line(canvas, {px, origin.y}, {px, origin.y - size.y}, sf::Color(225, 225, 225), 1.f);
    line(canvas, {origin.x, py}, {origin.x + size.x, py}, sf::Color(225, 225, 225), 1.f);
    text(canvas, font, format(box.xmin + (box.xmax - box.xmin) * i / 10), 16, {px - 25.f, origin.y + 12.f});
    text(canvas, font, format(box.ymin + (box.ymax - box.ymin) * i / 10), 16, {origin.x - 120.f, py - 9.f});
  }
  line(canvas, origin, {origin.x + size.x, origin.y}, sf::Color::Black, 2.f);
  line(canvas, origin, {origin.x, origin.y - size.y}, sf::Color::Black, 2.f);
  auto drawSeries = [&](const std::vector<double> & values, sf::Color color)
  {
    for (std::size_t i = 1; i < data.x.size(); ++i)
      line(canvas, point(data.x[i - 1], values[i - 1]), point(data.x[i], values[i]), color, 3.f);
  };
  if (data.kind == DataSet::Kind::Solution)
  {
    drawSeries(data.first, sf::Color(40, 95, 190));
    drawSeries(data.second, sf::Color(210, 65, 55));
    text(canvas, font, "Численное решение", 20, {980.f, 120.f}, sf::Color(40, 95, 190));
    text(canvas, font, "Точное решение", 20, {980.f, 150.f}, sf::Color(210, 65, 55));
    text(canvas, font, "Решение и ошибка", 30, {520.f, 35.f});
    text(canvas, font, "Координата", 22, {620.f, 820.f});
    text(canvas, font, "Значение", 22, {35.f, 390.f});
  }
  else if (data.kind == DataSet::Kind::Error)
  {
    drawSeries(data.first, sf::Color(40, 95, 190));
    text(canvas, font, "Ошибка", 20, {980.f, 120.f}, sf::Color(40, 95, 190));
    text(canvas, font, "Ошибка от шага", 30, {560.f, 35.f});
    text(canvas, font, "Шаг", 22, {650.f, 820.f});
    text(canvas, font, "Ошибка", 22, {45.f, 390.f});
  }
  else
  {
    drawSeries(data.first, sf::Color(40, 95, 190));
    text(canvas, font, "Время расчёта", 30, {560.f, 35.f});
    text(canvas, font, "Число разбиений", 22, {590.f, 820.f});
    text(canvas, font, "Время, мкс", 22, {35.f, 390.f});
  }
  canvas.display();
  if (!output.parent_path().empty())
    std::filesystem::create_directories(output.parent_path());
  if (!canvas.getTexture().copyToImage().saveToFile(output.string()))
    throw std::runtime_error("Не удалось сохранить PNG: " + output.string());
}

void showPlot(const std::filesystem::path & imagePath)
{
  sf::Texture texture;
  if (!texture.loadFromFile(imagePath.string()))
    throw std::runtime_error("Не удалось загрузить PNG для показа: " + imagePath.string());
  sf::Sprite sprite(texture);
  sf::RenderWindow window(sf::VideoMode(texture.getSize().x, texture.getSize().y), "Визуализатор");
  while (window.isOpen())
  {
    sf::Event event{};
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        window.close();
    }
    window.clear(sf::Color::White);
    window.draw(sprite);
    window.display();
  }
}

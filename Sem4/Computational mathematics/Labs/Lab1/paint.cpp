#include "paint.hpp"

#include <SFML/Graphics.hpp>

void paint(const vec & orig, const vec & Lagrange, const vec & spline)
{
  sf::RenderWindow window(sf::VideoMode(800, 800), "UwU");

  sf::Texture t;
  t.loadFromFile("chart.png");
  sf::Sprite pic(t);

  while (window.isOpen())
  {
    sf::Event event;
    while (window.pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
      {
        window.close();
      }
    }
    /*window.clear(sf::Color::White);
    // Рисуем точки
    for (const auto& point : orig)
    {
      sf::CircleShape circle(5);
      circle.setPosition(orig.x - 5, orig.y - 5);
      circle.setFillColor(sf::Color::Red);
      window.draw(circle);
    }
    // Рисуем линии, соединяющие точки
    for (size_t i = 0; i < points.size() - 1; ++i)
    {
      sf::Vertex line[] = {
        sf::Vertex(orig[i], sf::Color::Blue),
        sf::Vertex(orig[i + 1], sf::Color::Blue)
      };
      window.draw(line, 2, sf::Lines);
    }*/
  window.display();
  }
}

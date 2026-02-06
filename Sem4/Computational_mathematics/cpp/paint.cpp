#include "paint.hpp"

#include <SFML/Graphics.hpp>

void paint(const vec & x, const vec & orig, const vec & Lagrange, const vec & spline)
{
  const double scale = 100;
  const double w = 1920;
  const double h = 1080;

  sf::RenderWindow window(sf::VideoMode(w, h), "UwU");

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
    window.clear(sf::Color::White);

    for (size_t i = 0; i < x.size(); ++i)
    {
      sf::CircleShape circle(5);
      circle.setPosition((w / 20) - orig[i] * scale, (h / 3) + x[i] * scale);
      circle.setFillColor(sf::Color(255, 0, 0, 150));
      window.draw(circle);

      circle.setPosition((w / 20) - Lagrange[i] * scale, (h / 3) + x[i] * scale);
      circle.setFillColor(sf::Color(0, 255, 0, 150));
      window.draw(circle);

      circle.setPosition((w / 20) - spline[i] * scale, (h / 3) + x[i] * scale);
      circle.setFillColor(sf::Color(0, 0, 255, 150));
      window.draw(circle);
    }
    for (size_t i = 0; i < orig.size() - 1; ++i)
    {
      sf::Vertex line[] = {
        sf::Vertex(sf::Vector2f((w / 20) - orig[i] * scale, (h / 3) + x[i] * scale), sf::Color(255, 0, 0, 150)),
        sf::Vertex(sf::Vector2f((w / 20) - orig[i + 1] * scale, (h / 3) + x[i + 1] * scale), sf::Color(255, 0, 0, 150))
      };
      window.draw(line, 5, sf::Lines);

      line[0] = sf::Vertex(sf::Vector2f((w / 20) - Lagrange[i] * scale, (h / 3) + x[i] * scale), sf::Color(0, 255, 0, 150));
      line[1] = sf::Vertex(sf::Vector2f((w / 20) - Lagrange[i + 1] * scale, (h / 3) + x[i + 1] * scale), sf::Color(0, 255, 0, 150));
      window.draw(line, 5, sf::Lines);

      line[0] = sf::Vertex(sf::Vector2f((w / 20) - spline[i] * scale, (h / 3) + x[i] * scale), sf::Color(0, 0, 255, 150));
      line[1] = sf::Vertex(sf::Vector2f((w / 20) - spline[i + 1] * scale, (h / 3) + x[i + 1] * scale), sf::Color(0, 0, 255, 150));
      window.draw(line, 5, sf::Lines);
    }
    window.display();
  }
}

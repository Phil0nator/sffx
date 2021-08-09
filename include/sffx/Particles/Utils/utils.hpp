#pragma once
#include <chrono>
#include <SFML/Graphics.hpp>


namespace sffx
{

    std::chrono::milliseconds now() noexcept;
    double randomDouble();

    template<typename T>
    T randomize(const T& x, const T& factor);


    template<>
    sf::Vector2f randomize(const sf::Vector2f & x, const sf::Vector2f & factor);

    template<typename T>
    T randomize(const T& x, const T& factor)
    {
        return x + (( randomDouble()-0.5 ) * factor);
    }

}
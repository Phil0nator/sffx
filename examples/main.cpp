#include <SFML/Graphics.hpp>
#include <stdlib.h>
#include "sffx/Effects.hpp"

int main(int argc, char const *argv[])
{
    auto window = new sf::RenderWindow( sf::VideoMode(600, 600), "demo" );
    
    sf::RectangleShape rect{{50,50}};
    rect.setOutlineThickness(3);
    rect.setOutlineColor(sf::Color(255,255,255));
    rect.setFillColor(sf::Color::Black);
    rect.setPosition(290,290);
    
    sf::RenderTexture buffer;
    buffer.create( 600,600 );
    buffer.setSmooth(true);
    
    sffx::Shake shake( 600,600 );
    shake.setSpeed( 0.4 );
    shake.setIntensity( 0.015 );
    sffx::Blur blur{600,600};
    blur.setIntensity(5);
    sffx::EffectPipeline pipeline { 600, 600, {&shake, &blur} };

    sffx::MotionBlur mb{600,600};

    std::optional< sf::Clock > doShake;

    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::MouseMoved)
            {
                rect.setPosition( event.mouseMove.x-25, event.mouseMove.y-25 );
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                doShake = sf::Clock();
            }
        }

        window->clear(sf::Color::Black);
        buffer.clear(sf::Color(255,255,255));
        buffer.draw(rect);
        buffer.display();
        if (doShake)
        {
            window->draw( mb( *pipeline( buffer.getTexture() ).getTexture() ) );
            if (doShake->getElapsedTime() >= sf::milliseconds(500))
            {
                doShake = std::nullopt;
            }
        }
        else
        {
            window->draw( mb(buffer.getTexture()) );
        }
        window->display();
    }


    return EXIT_SUCCESS;
}
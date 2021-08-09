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
    
    sffx::Blur blur{ 600,600 };
    blur.setAxis( {0,0} );


    while (window->isOpen())
    {
        sf::Event event;
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::MouseMoved)
            {
                rect.setPosition( event.mouseMove.x, event.mouseMove.y );
            }
        }

        window->clear(sf::Color::Black);
        buffer.clear(sf::Color(255,255,255));
        buffer.draw(rect);
        buffer.display();
        window->draw( blur( buffer.getTexture() ) );
        window->display();
    }


    return EXIT_SUCCESS;
}
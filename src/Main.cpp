#include <iostream>
#include <SFML/Graphics.hpp>

int main(int argc, char const *argv[])
{

    sf::RenderWindow window(sf::VideoMode(800,600), "jujube");
    window.setVerticalSyncEnabled(true);
    
    bool fullscreen = false;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case sf::Event::KeyPressed:
                switch (event.key.code)
                {
                case sf::Keyboard::Enter:
                    if (event.key.alt)
                    {
                        window.close();
                        if (fullscreen)
                        {
                            window.create(sf::VideoMode(800,600), "jujube");
                        } else {
                            auto width = sf::VideoMode::getDesktopMode().width;
                            auto height = sf::VideoMode::getDesktopMode().height;
                            window.create(sf::VideoMode(width,height), "jujube", sf::Style::Fullscreen);
                        }
                        fullscreen = not fullscreen;
                    } 
                    break;
                default:
                    break;
                }
                break;
            case sf::Event::Closed:
                window.close();
                break;
            default:
                break;
            }
        }
        window.clear(sf::Color::Black);
        window.display();
    }
    return 0;
}

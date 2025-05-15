#ifndef DEFAULT_BUTTON_FACTORY 
#define DEFAULT_BUTTON_FACTORY

#include <Graphics/InterfaceElements/ButtonFactory/Button_factory.h>

#include <SFML/Graphics.hpp>

class DefaultButtonFactory : public ButtonFactory {
public:
    explicit DefaultButtonFactory(const sf::Font& font)
        :_font(font) 
    {}

    std::unique_ptr<Button> createButton
    (
        const std::string& text,
        const sf::Vector2f& position,
        const sf::Vector2f& size = { 200, 50 }
    ) 
    const override
    {
        ButtonConfig config;
        config.buttonSize = size;
        config.buttonPosition = position;
        config.title = sf::Text(text, _font, 20);
        config.normalColor = sf::Color(100, 100, 200);
        config.hoverColor = sf::Color(150, 150, 255);
        config.outlineThickness = 2;
        config.outlineColor = sf::Color::White;

        return std::make_unique<Button>(config);
    }

private:
    const sf::Font& _font;
};

#endif //DEFAULT_BUTTON_FACTORY
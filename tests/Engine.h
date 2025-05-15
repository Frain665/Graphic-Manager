#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <random>
#include <optional>
#include <memory>
#include <string>
#include <vector>

#include <Graphics/InterfaceElements/Button.h>
#include <Graphics/InterfaceElements/TextField.h>
#include <Exceptions.h>
#include <AnchoredElement.h>

class Engine
{
private:
	void init();
	void initVariables();
	void uploadResources();
	void initWindow();

	Engine() = default;
	Engine operator=(const Engine&) = delete;
	Engine(const Engine&) = delete;

	std::unique_ptr<sf::RenderWindow> _window;
	sf::Event _event{};
	sf::VideoMode _videoMode;
	std::string _windowTitle;

	int _passwordLength;
	std::string _allowedChars;
	std::optional<AnchoredElement> _anchor;
	const std::string _fontPath = RESOURCES_DIR "fonts/defaultFont.otf";
	sf::Font _font;

	std::vector<std::unique_ptr<Button>> _buttons;
	std::shared_ptr <TextField> _textField;

public:
	static Engine& getInstance();

	~Engine()
	{
		if (_window && _window->isOpen())
		{
			_window->close();
		}
	}

	void handleInput();
	void run();
	void render();
	void updatingButtons();
	std::string generateRandomPassword();
	void update();
};

#endif //ENGINE_HPP
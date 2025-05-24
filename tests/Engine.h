#ifndef ENGINE_HPP
#define ENGINE_HPP

#include <iostream>
#include <random>
#include <optional>
#include <memory>
#include <string>
#include <vector>

#include <GraphicsManager.h>

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

	std::unique_ptr<ProgressBar> _volumeBar;

	std::vector<std::optional<AnchoredElement>> _buttonAnchors;
	std::vector<std::optional<AnchoredElement>> _checkboxAnchors;
	std::vector<std::optional<AnchoredElement>> _textFieldsAnchors;

	const std::string _fontPath = RESOURCES_DIR "Fonts/defaultFont.otf";
	sf::Font _font;

	std::vector<std::unique_ptr<Button>> _buttons;
	std::vector<std::unique_ptr<CheckBox>> _checkboxes;
	std::vector<std::unique_ptr<TextField>> _textFields;


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
	void updateButtons();
	void update();
};

#endif //ENGINE_HPP
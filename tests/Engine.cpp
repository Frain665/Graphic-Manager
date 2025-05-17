#include "Engine.h"

void Engine::initVariables()
{
	_windowTitle = "Test";
	_window = nullptr;
	_event = {};
}

void Engine::uploadResources()
{
	if (!_font.loadFromFile(_fontPath))
	{
		throw FontException(_fontPath);
	}

	size_t buttonIndex = _buttons.size() - 1;

	DefaultButtonFactory defaultButtonFactory(_font);

	auto generateButton = defaultButtonFactory.createButton("Generate", { 200, 50 });

	_buttons.push_back(std::move(generateButton));

	auto checkbox = std::make_unique<CheckBox>("Enable Feature", sf::Vector2f{ 100, 100 });
	checkbox->setCallback([=](bool checked) {
		if (checked)
		{
			
		}
		else
		{
			
		}
		});

	_checkboxes.emplace_back(std::move(checkbox));

	_textField = std::make_shared <TextField>();
	_textField->setSize(sf::Vector2f(100.f, 50.f));
	_textField->setPosition(sf::Vector2f(400, 300.f));

	_anchor.emplace(AnchorHorizontal::CENTER,
		AnchorVertical::BOTTOM,
		sf::Vector2f(-70, -20),
		sf::Vector2f(200, 50),
		[this, buttonIndex](const sf::Vector2f& offset, const sf::Vector2f& size)
		{
			if (buttonIndex < _buttons.size() && _buttons[buttonIndex])
			{
				_buttons[buttonIndex]->setPosition(offset);
				_buttons[buttonIndex]->getShape().setSize(size);
			}
		});
}

void Engine::initWindow()
{
	_window = std::make_unique<sf::RenderWindow>(sf::VideoMode(800, 600),
		_windowTitle, sf::Style::Close | sf::Style::Titlebar);
	_window->setFramerateLimit(60);

	if (!_window)
	{
		throw WindowNotInitializedException("Game::initWindow() -> ");
	}
}

void Engine::init()
{
	initVariables();
	uploadResources();

	try
	{
		initWindow();
	}
	catch (const WindowNotInitializedException& exception)
	{
		std::cerr << "CRITICAL: " << exception.what() << std::endl;
		exit(EXIT_FAILURE);
	}
	catch (const FileLoadException& exception)
	{
		std::cerr << "RESOURCE ERROR: " << exception.what() << std::endl;
	}
	catch (const std::exception& exception)
	{
		std::cerr << "UNKNOWN ERROR: " << exception.what() << std::endl;
	}
}

void Engine::update()
{
	this->handleInput();

	_textField->handleEvent(*_window, _event);

	for (auto const& box : _checkboxes)
	{
		box->handleEvent(*_window, _event);
	}

	updatingButtons();

	_anchor->update(sf::Vector2u(800, 600));

	for (auto const& button : _buttons)
	{
		button->handleEvent(*_window, _event);
	}
}

void Engine::render()
{
	_window->clear();

	for (auto const& button : _buttons)
	{
		button->draw(*_window);
	}

	for (const auto& box : _checkboxes)
	{
		box->draw(*_window);
	}

	_textField->draw(*_window);

	_window->display();
}

void Engine::updatingButtons()
{

}

void Engine::handleInput()
{

	while (_window->pollEvent(_event))
	{
		switch (_event.type)
		{
		case sf::Event::Closed:
			_window->close();
			break;
		case sf::Event::KeyPressed:
			if (_event.key.code == sf::Keyboard::Escape)
			{
				_window->close();
			}
			break;
		case sf::Event::Resized:
			_anchor->update(_window->getSize());
			break;
		default:
			break;
		}
	}
}

void Engine::run()
{
	init();

	while (_window->isOpen())
	{
		update();
		render();
	}
}

Engine& Engine::getInstance()
{
	static Engine instance;
	return instance;
}
#include "Engine.h"
#include <AnchoredElement.h>
#include <Graphics/InterfaceElements/ButtonFactory/Default_button_factory.h>

void Engine::initVariables()
{
	_windowTitle = "Password Generator";
	_window = nullptr;
	_event = {};
	_passwordLength = 12;
	_allowedChars = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!@#$%^&*()";
}

void Engine::uploadResources()
{
	if (!_font.loadFromFile(_fontPath))
	{
		throw FontException(_fontPath);
	}

	DefaultButtonFactory defaultButtonFactory(_font);

	auto generateButton = defaultButtonFactory.createButton("Generate", { 200, 50 });

	_buttons.push_back(std::move(generateButton));
	size_t buttonIndex = _buttons.size() - 1;

	_textField = std::make_shared <TextField>();
	_textField->setSize(sf::Vector2f(400.f, 100.f));
	_textField->setPosition(300.f, 300.f);

	_anchor.emplace(AnchorHorizontal::CENTER,
		AnchorVertical::BOTTOM,
		sf::Vector2f(-70, -20),
		sf::Vector2f(200, 50),
		[this, buttonIndex](float xPos, float yPos, const sf::Vector2f& size)
		{
			if (buttonIndex < _buttons.size() && _buttons[buttonIndex])
			{
				_buttons[buttonIndex]->setPosition(xPos, yPos);
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

	_textField->draw(*_window);

	_window->display();
}

void Engine::updatingButtons()
{
	if (_buttons.empty() || !_textField)
	{
		return;
	}

	try
	{
		const auto& generateButton = _buttons.at(0);

		if (generateButton->isClicked(_event))
		{
			std::string newPassword = generateRandomPassword();
			_textField->setText(newPassword);

			std::cout << "Generated new password: " << newPassword << std::endl;
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Error in updatingButtons: " << e.what() << std::endl;
	}

}

std::string Engine::generateRandomPassword()
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<> distr(0, _allowedChars.size() - 1);

	std::string password;
	password.reserve(_passwordLength);

	for (int i = 0; i < _passwordLength; ++i) {
		password += _allowedChars[distr(generator)];
	}

	return password;
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
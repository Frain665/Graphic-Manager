#ifndef CHECKBOX_FACTORY
#define CHECKBOX_FACTORY

#include <memory>

#include <Graphics/InterfaceElements/CheckBox.h>

class CheckBoxFactory
{
public:
	virtual std::unique_ptr<CheckBox> createCheckBox(const sf::Font& font,
		const std::string& text,
		const sf::Vector2f& pos,
		unsigned int characterSize = 16) const = 0;

	~CheckBoxFactory() = default;
};

#endif //CHECKBOX_FACTORY
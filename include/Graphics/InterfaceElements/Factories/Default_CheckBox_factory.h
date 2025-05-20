#include <Graphics/InterfaceElements/Factories/CheckBox_factory.h>
#include <Graphics/InterfaceElements/CheckBox.h>

class DefaultCheckBoxFactory : public CheckBoxFactory
{
public:
	std::unique_ptr<CheckBox> createCheckBox(const sf::Font& font,
		const std::string& text,
		const sf::Vector2f& pos,
		unsigned int characterSize = 16) const override
	{
		auto checkbox = std::make_unique<CheckBox>(font, text, pos, characterSize);

		checkbox->setSize({ 20.f, 20.f });

		return checkbox;
	}
};
#ifndef ANCHORED_ELEMENT_HPP
#define ANCHORED_ELEMENT_HPP

#include <functional>

#include <SFML/System/Vector2.hpp>

enum class AnchorHorizontal { LEFT, CENTER, RIGHT, STRETCH };
enum class AnchorVertical { TOP, CENTER, BOTTOM, STRETCH };

class AnchoredElement
{
public:
	using UpdateCallback = std::function<void(const sf::Vector2f&, const sf::Vector2f&)>;

	AnchoredElement(AnchorHorizontal horizAnchor,
		AnchorVertical vertAnchor,
		const sf::Vector2f& offset,
		const sf::Vector2f& size,
		UpdateCallback const& callback);

	void update(const sf::Vector2u& windowSize);

private:
	AnchorHorizontal _horizAnchor;
	AnchorVertical _vertAnchor;
	sf::Vector2f _offset;
	sf::Vector2f _size;
	UpdateCallback _callback;

};

#endif //ANCHORED_ELEMENT_HPP
#include "AnchoredElement.h"

AnchoredElement::AnchoredElement(AnchorHorizontal horizAnchor,
    AnchorVertical vertAnchor,
    const sf::Vector2f& offset,
    const sf::Vector2f& size,
    UpdateCallback const& callback)
    : _horizAnchor(horizAnchor),
    _vertAnchor(vertAnchor),
    _offset(offset),
    _size(size),
    _callback(callback) {
}

void AnchoredElement::update(const sf::Vector2u& windowSize)
{
    sf::Vector2f newPosition;
    sf::Vector2f newSize;

    switch (_horizAnchor) {
    case AnchorHorizontal::LEFT:
        newPosition.x = _offset.x;
        newSize.x = _size.x;
        break;
    case AnchorHorizontal::CENTER:
        newPosition.x = windowSize.x / 2.0f + _offset.x;
        newSize.x = _size.x;
        break;
    case AnchorHorizontal::RIGHT:
        newPosition.x = windowSize.x - _size.x + _offset.x;
        newSize.x = _size.x;
        break;
    case AnchorHorizontal::STRETCH:
        newPosition.x = _offset.x;
        newSize.x = windowSize.x - _offset.x * 2;
        break;
    }

    switch (_vertAnchor) {
    case AnchorVertical::TOP:
        newPosition.y = _offset.y;
        newSize.y = _size.y;
        break;
    case AnchorVertical::CENTER:
        newPosition.y = windowSize.y / 2.0f + _offset.y;
        newSize.y = _size.y;
        break;
    case AnchorVertical::BOTTOM:
        newPosition.y = windowSize.y - _size.y + _offset.y;
        newSize.y = _size.y;
        break;
    case AnchorVertical::STRETCH:
        newPosition.y = _offset.y;
        newSize.y = windowSize.y - _offset.y * 2;
        break;
    }

    _callback(newPosition, newSize);
}

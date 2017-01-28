#include "Anim_Directional.hpp"
#include "SpriteSheet.hpp"

void Anim_Directional::cropSprite() {
	sf::Vector2f padding = spriteSheet->getSheetPadding();
	sf::Vector2f spacing = spriteSheet->getSpriteSpacing();
	sf::IntRect rect(
		spriteSheet->getSpriteSize().x * frameCurrent + padding.x + (spacing.x * frameCurrent),
		(spriteSheet->getSpriteSize().y * (frameRow + (short)spriteSheet->getDirection())) + padding.y + ((frameRow + (short)spriteSheet->getDirection()) * spacing.y),
		spriteSheet->getSpriteSize().x,
		spriteSheet->getSpriteSize().y);
	spriteSheet->cropSprite(rect);
}

void Anim_Directional::frameStep() {
	if (frameStart < frameEnd)
		++frameCurrent;
	else
		--frameCurrent;

	if ((frameStart < frameEnd && frameCurrent > frameEnd) || (frameStart > frameEnd && frameCurrent < frameEnd)) {
		if (loop) {
			frameCurrent = frameStart;
			return;
		}
		frameCurrent = frameEnd;
		pause();
	}
}

void Anim_Directional::readIn(std::stringstream& stream) {
	stream >> frameStart >> frameEnd >> frameRow >> frameTime >> frameActionStart >> frameActionEnd;
}
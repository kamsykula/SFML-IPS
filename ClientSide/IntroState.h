#pragma once
#include "BaseState.h"
#include "StateManager.h"
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/Color.hpp>


class IntroState : public BaseState
{
public:
   IntroState(StateManager *stateManager);
   ~IntroState();

   void onCreate() override;
   void onDestroy() override;

   void draw() override;
   void update(const sf::Time &time) override;

   void activate() override;
   void deactivate() override;

   void Continue(EventDetails*);

private:
   sf::Font font;
   sf::Text introText;
   sf::Sprite introSprite;
   sf::Texture introTexture;

   sf::Clock clock;
   sf::Time time;
   float timePassed;
   unsigned int transparentNum;
};


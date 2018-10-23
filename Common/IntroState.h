#pragma once
#include "BaseState.h"
#include "StateManager.h"
#include <SFML/Graphics/Text.hpp>


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

private:
   sf::Font font;
   sf::Text introText;
   float timePassed;
};

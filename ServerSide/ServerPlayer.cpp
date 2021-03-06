#include "ServerPlayer.h"
#include "Global.h"
#include <iostream>
#include <cmath>

ServerPlayer::ServerPlayer(b2Body *body, ShipType shipType)
{
   this->body = body;
   this->maxSpeed = 2;
   this->maxAngularSpeed = 0.5;
   this->verticalSpeed = 0;
   this->angularSpeed = 0;
   this->shootTimeout = 0;
   this->body->SetUserData(this);
   this->shipType = shipType;
}


ServerPlayer::~ServerPlayer()
{
}

sf::Vector2f ServerPlayer::getPosition()
{
   b2Vec2 pos = this->body->GetPosition();
   return sf::Vector2f(pos.x, pos.y);
}

float ServerPlayer::getAngle()
{
   return this->body->GetAngle();
}

short ServerPlayer::getHealt()
{
	return this->health;
}

void ServerPlayer::setHealth(short new_heath)
{
	this->health += new_heath;
}

bool ServerPlayer::isDead() 
{
	if (!this->health)
	{
		return true;
	}
	return false;
}

PlayerState ServerPlayer::getPlayerState()
{
   return PlayerState(getPosition(), getAngle(), this->health);
}

void ServerPlayer::update(const sf::Time &time)
{
   this->verticalSpeed *= .78;
   this->angularSpeed *= .50;
   float x = std::cos(this->body->GetAngle());
   float y = std::sin(this->body->GetAngle());
   this->body->SetLinearVelocity(b2Vec2(x * verticalSpeed, this->verticalSpeed * y));
   this->body->SetAngularVelocity(this->angularSpeed);
   this->shootTimeout += time.asSeconds();
}

void ServerPlayer::move(MoveDirection & direction, const sf::Int32 & _time)
{
   b2Vec2 speedVec = this->body->GetLinearVelocity();
   float angular = this->body->GetAngularVelocity();
   b2Vec2 angularVec = this->body->GetLinearVelocity();

   float time = _time / 1000.f;
   if (direction & MoveDirection::FORWARD)
   {
      if (std::fabs(this->verticalSpeed) <= this->maxSpeed)
      {
         this->verticalSpeed += 0.2f;
      }
   }
   else if (direction & MoveDirection::BACKWARD)
   {
      if (std::fabs(this->verticalSpeed) <= this->maxSpeed)
      {
         //this->verticalSpeed -= 0.2f;
      }
   }

   if (direction & MoveDirection::LEFT)
   {
      if (std::fabs(this->angularSpeed) <= this->maxAngularSpeed)
      {
         this->angularSpeed += 0.4f + 0.4f * (time * time * 0.5f);
      }
   }
   else if (direction & MoveDirection::RIGHT)
   {
      if (std::fabs(this->angularSpeed) <= this->maxAngularSpeed)
      {
         this->angularSpeed -= 0.4f + 0.4f * (time * time * 0.5f);
      }
   }
}

bool ServerPlayer::canShoot()
{
   return this->shootTimeout > 2.f;
}
ShipType ServerPlayer::getShipType()
{
   return this->shipType;
}
void ServerPlayer::shoot()
{
   this->shootTimeout = 0;
}

b2Body * ServerPlayer::getBody()
{
   return this->body;
}

#pragma once
#include "util.h"

// Describes a Collidable Object (Player, Enemy and Bullet inherit this class)
class Collidable {
public:
	virtual Disk getCollisionHull() const = 0;
};

// Decribes a GameObject (Player, Enemy and Bullet inherit this class)
class GameObject
{
public:
	GameObject(const class Game& mygame);
	virtual void update() = 0;
	virtual void draw() = 0;
	virtual void init() = 0;

	virtual float getPosX() const = 0;
	virtual float getPosY() const = 0;

	virtual bool isActive() const = 0;
protected:
	const class Game& game;
};
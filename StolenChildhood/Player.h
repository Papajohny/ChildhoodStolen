#pragma once
#include "gameobject.h"
#include "config.h"
#include <algorithm>

class Player : public GameObject, public Collidable
{
public:
	Player(const class Game& mygame);
	~Player();

	void init() override;
	void update() override;
	void draw() override;

	float getPosX() const override { return pos_x; }
	float getPosY() const override { return pos_y; }
	Disk getCollisionHull() const override;
	float getRemainingLife() const { return life; }
	bool isActive() const override { return active; }

	void setLife(float inplife) { life = inplife; }
	void drainLife(float amount) { life = std::max<float>(0.0f, life - amount); }

private:
	float pos_x = CANVAS_WIDTH / 2;
	float pos_y = CANVAS_HEIGHT - 75;
	float speed = 7.0f;
	float life;
	bool active = true;
};
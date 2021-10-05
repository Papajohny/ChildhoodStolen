#pragma once
#include "gameobject.h"
#include "graphics.h"

class Enemy : public GameObject, public Collidable
{
public:
	Enemy(const class Game& mygame, float positionX);
	~Enemy();

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
	float pos_x;
	float pos_y;
	float speed;
	float life;
	bool active = true;
	float size;
	graphics::Brush brush;

};
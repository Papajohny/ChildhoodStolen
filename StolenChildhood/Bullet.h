#pragma once
#include "gameobject.h"
#include "graphics.h"

class Bullet : public GameObject, public Collidable
{
public:
	Bullet(const class Game& mygame, float shooterPosX, float shooterPosY, bool isPB);
	~Bullet();

	void init() override;
	void update() override;
	void draw() override;

	float getPosX() const override { return pos_x; }
	float getPosY() const override { return pos_y; }
	Disk getCollisionHull() const override;
	bool isActive() const override { return active; }

	void setHitTarget(bool hT) { hitTarget = hT; }

private:
	float pos_x;
	float pos_y;
	float speed;
	bool active = true;
	float size;
	graphics::Brush brush;

	bool hitTarget = false;

	bool isPlayersBullet;
};
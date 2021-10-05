#include "Enemy.h"
#include "config.h"
#include "util.h"
#include "game.h"

#include <iostream>

Enemy::Enemy(const Game& mygame, float positionX)
	: GameObject(mygame)
{
	pos_x = positionX;
	init();
}

Enemy::~Enemy()
{
}

void Enemy::init()
{
	speed = 0.03f;
	pos_y = 0 - 1.1f * size;
	size = 100;
	brush.outline_opacity = 0.0f;
	pos_x += size;
}

void Enemy::update()
{
	// Enemy stops at y = 150
	if (pos_y >= 150)
	{
		pos_y = 150;
	}
	else
	{
		graphics::playSound(std::string(ASSET_PATH) + "fx_bootsteps.mp3", 0.15f, false);
		pos_y += speed * graphics::getDeltaTime();
	}

	// If Enemy has life = 0.0f his state changes to inactive.
	if (life <= 0.0f) {
		active = false;
	}
}

void Enemy::draw()
{
	// Draws Enemy's shadow.
	brush.texture = std::string(ASSET_PATH) + "shadow.png";
	graphics::drawRect(pos_x - 38, pos_y + 55, size, size, brush);

	// Draws the Enemy.
	brush.texture = std::string(ASSET_PATH) + "enemy.png";
	graphics::drawRect(pos_x, pos_y, size, size + 40, brush);

	// For debugging purposes the Enemy's Collision hull is drawn.
	if (game.getDebugMode())
	{
		graphics::Brush br;
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.3f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;
		Disk collisionHull = getCollisionHull();
		graphics::drawDisk(collisionHull.cx, collisionHull.cy, collisionHull.radius, br);
	}
}

Disk Enemy::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x + 10;
	disk.cy = pos_y + 20;
	disk.radius = size / 2;
	return disk;
}

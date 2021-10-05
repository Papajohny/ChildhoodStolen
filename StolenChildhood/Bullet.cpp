#include "Bullet.h"
#include "config.h"
#include "util.h"
#include "game.h"

#include <iostream>

Bullet::Bullet(const Game& mygame, float shooterPosX, float shooterPosY, bool isPB)
	: GameObject(mygame)
{
	graphics::playSound(std::string(ASSET_PATH) + "fx_gunshot.mp3", 0.35f, false);
	pos_x = shooterPosX;
	pos_y = shooterPosY;
	isPlayersBullet = isPB;
	init();
}

Bullet::~Bullet()
{
}

void Bullet::init()
{
	speed = 0.55f;
	size = 50;
	brush.outline_opacity = 0.0f;
}

void Bullet::update()
{
	// If it is a Player's Bullet, it goes up.
	if (isPlayersBullet)
	{
		pos_y -= speed * graphics::getDeltaTime();

		// Until it leaves the Screen.
		if (pos_y < 0 - size)
		{
			active = false;
		}
	}
	// else if is it an Enemy's Bullet it goes down.
	else
	{
		pos_y += speed * graphics::getDeltaTime();

		if (pos_y > CANVAS_HEIGHT + size)
		{
			active = false;
		}
	}

	// For both cases if the Bullet hits the target, its state changes to inactive.
	if (hitTarget)
	{
		active = false;
	}
}

void Bullet::draw()
{
	// If it an Enemy's Bullet the texture is rotated.
	if (!isPlayersBullet)
	{
		graphics::setOrientation(180.0f);
	}

	brush.texture = std::string(ASSET_PATH) + "bullet.png";
	graphics::drawRect(pos_x, pos_y, 50, 50, brush);

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

	graphics::resetPose();
}

// The Collision Hull needs to change if it is a Player's Bullet or an Enemy's Bullet.
Disk Bullet::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x;
	if (isPlayersBullet)
	{
		disk.cy = pos_y - 10;
	}
	else
	{
		disk.cy = pos_y + 10;
	}
	disk.radius = size / 2 - 10;
	return disk;
}
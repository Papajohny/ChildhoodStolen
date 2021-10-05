#include "Player.h"
#include "graphics.h"
#include "game.h"
#include <iostream>


Player::Player(const Game& mygame)
	: GameObject(mygame)
{
}

Player::~Player()
{
}

void Player::init()
{
}

void Player::update()
{
	// Checks if Player presses any of the WASD keys.
	if (graphics::getKeyState(graphics::SCANCODE_W))
	{
		pos_y -= speed * graphics::getDeltaTime() / 10.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_A))
	{
		pos_x -= speed * graphics::getDeltaTime() / 10.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_S))
	{
		pos_y += speed * graphics::getDeltaTime() / 10.0f;
	}
	if (graphics::getKeyState(graphics::SCANCODE_D))
	{
		pos_x += speed * graphics::getDeltaTime() / 10.0f;
	}

	// Borders for the Player's movements.
	if (pos_x < 55)
	{
		pos_x = 55;
	}
	if (pos_x > 1153)
	{
		pos_x = 1153;
	}
	if (pos_y < 460)
	{
		pos_y = 460;
	}
	if (pos_y > CANVAS_HEIGHT - 75)
	{
		pos_y = CANVAS_HEIGHT - 75;
	}

	// If Player is dead it is inactive (duh).
	if (life <= 0.0f) {
		active = false;
	}
}

void Player::draw()
{
	graphics::Brush br;
	br.outline_opacity = 0.0f;

	// Draws Player's shadow.
	br.texture = std::string(ASSET_PATH) + "shadow.png";
	graphics::drawRect(pos_x - 50, pos_y + 55 * 1.2f, 75, 75, br);

	// Draws the Player.
	br.texture = std::string(ASSET_PATH) + "player.png";
	graphics::drawRect(pos_x, pos_y, 100, 150, br);

	// For debugging purposes the collisionHull of the Player is shown.
	if (game.getDebugMode())
	{
		br.outline_opacity = 1.0f;
		br.texture = "";
		br.fill_color[0] = 0.3f;
		br.fill_color[1] = 1.0f;
		br.fill_color[2] = 0.3f;
		br.fill_opacity = 0.3f;
		br.gradient = false;
		Disk collisionHull = getCollisionHull();
		graphics::drawDisk(collisionHull.cx, collisionHull.cy, collisionHull.radius, br);
	}
}

Disk Player::getCollisionHull() const
{
	Disk disk;
	disk.cx = pos_x - 12;
	disk.cy = pos_y - 20;
	disk.radius = 50.0f;
	return disk;
}

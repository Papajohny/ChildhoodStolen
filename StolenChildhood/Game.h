#pragma once
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include <set>
#include "graphics.h"

// The game's state
class Game {
public:
	void init(); // Here the game initializes some stuff needed.
	void update(); // Responsible for the Game logic
	void draw(); // Responsible for everything that is drawn on Screen

	// Constructor, Destructor
	Game();
	~Game();

	// Useful for debugging
	void setDebugMode(bool d) { debug_mode = d; }
	bool getDebugMode() const { return debug_mode; }

private:
	// 
	typedef enum { STATUS_START, STATUS_PLAYING, STATUS_CREDITS } status_t; // Useful for changing between the 3 main screens
	status_t status = STATUS_START; // Initialing status to the Start Screen
	bool debug_mode = false; // Useful for debugging

	// All related to the Start Screen
	int menuOption = 0;
	bool pressedControls = false;
	bool justPressedButtonMainMenu = false;
	bool justPressedEnterForSubMenu = false;
	bool isOnStartMenu = true;
	bool subMenuChooseDifficulty = false;
	int subMenuOption;
	float timePassedOnStartScreen = 0.0f;
	float timePassedOnSubMenu = 0.0f;
	float timeToDistinguishEntersBetweenMainAndSubMenus = 0.0f;
	void updateStartScreen();
	void drawStartScreen();

	// Alternating between those two when playing in the Level Screen
	void updateLevelScreen();
	void drawLevelScreen();

	// All related to the Player.
	Player* player = nullptr;
	bool player_initialized = false;
	int timePassedSincePlayerShooted = 0;
	void spawnPlayer();
	void updatePlayer();

	// All related to Player's Bullets.
	std::set<class Bullet*> playersBullets;
	void playerShootsBullets();
	void updatePlayersBullets();
	void checkPlayersBullets();

	// All related to Enemies.
	int numberOfEnemies = 5;
	int currentWave = 0;
	int enemiesAlive = 0;
	bool changeBetweenTwoEnemies = false;
	float twoOrLessEnemiesTimePassedSinceShoot = 0.0f;
	float timePassedToAllowMiddleEnemyToShoot = 0.0f;
	std::set<class Enemy*> enemiesOnScreen;
	void spawnEnemies();
	float ithEnemysPositionX(int iOfCurrentEnemy, int maxEnemies); // Computes the X position of the ith Enemy
	void updateEnemies();

	// All related to Enemies' Bullets.
	std::set<class Bullet*> enemiesBullets;
	int lastEnemyThatShooted = 0;
	void enemyShootBullet();
	void updateEnemiesBullets();
	void checkEnemiesBullets();

	// Checks	1) if any of Player's Bullets collide with any of the Enemies and
	//			2) if any of the Enemies' Bullets collide with Player.
	void checkCollisions();

	// All related to Credit Screen
	float timePassedOnCreditScreen = 0.0f;
	int whatToShowOnCreditScreen = 0;
	bool wipeFirstCreditScreen = false;
	void updateCreditsScreen();
	void drawCreditsScreen();
	void clearAll();
};
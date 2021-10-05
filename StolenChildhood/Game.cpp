#include "game.h"
#include "config.h"

#include <cstdlib>
#include <time.h>

void Game::init()
{
	status = STATUS_START;
	graphics::setFont(std::string(ASSET_PATH) + "font_warwound.otf");
	graphics::playMusic(std::string(ASSET_PATH) + "bgm_what_a_wonderful_world.mp3", 0.5f, true, 4000);
	menuOption = 1;
}

void Game::update()
{
	if (status == STATUS_START)
	{
		updateStartScreen();
	}
	else if (status == STATUS_PLAYING)
	{
		updateLevelScreen();
	}
	else
	{
		updateCreditsScreen();
	}
}

void Game::draw()
{
	if (status == STATUS_START)
	{
		drawStartScreen();
	}
	else if (status == STATUS_PLAYING)
	{
		drawLevelScreen();
	}
	else
	{
		drawCreditsScreen();
	}
}

Game::Game()
{
}

Game::~Game()
{
	if (player)
	{
		delete player;
	}
}

void Game::updateStartScreen()
{
	// All the possible scenarios while in Start Menu.
	// Here is the code for the first Menu.
	timePassedOnStartScreen = timePassedOnStartScreen + graphics::getDeltaTime();

	if (timePassedOnStartScreen > 200 && isOnStartMenu && !subMenuChooseDifficulty)
	{
		// For the UP and DOWN keys in the Start Menu.
		if (graphics::getKeyState(graphics::SCANCODE_DOWN) && (menuOption == 1))
		{
			graphics::playSound(std::string(ASSET_PATH) + "fx_menu_up_down.mp3", 0.5f, false);
			menuOption = 2;
			timePassedOnStartScreen = 0.0f;
			justPressedButtonMainMenu = true;
		}

		if (graphics::getKeyState(graphics::SCANCODE_UP) && (menuOption == 2))
		{
			graphics::playSound(std::string(ASSET_PATH) + "fx_menu_up_down.mp3", 0.5f, false);
			menuOption = 1;
			timePassedOnStartScreen = 0.0f;
		}

		if (graphics::getKeyState(graphics::SCANCODE_DOWN) && (menuOption == 2) && !(justPressedButtonMainMenu))
		{
			graphics::playSound(std::string(ASSET_PATH) + "fx_menu_up_down.mp3", 0.5f, false);
			menuOption = 3;
			timePassedOnStartScreen = 0.0f;
		}

		if (graphics::getKeyState(graphics::SCANCODE_UP) && (menuOption == 3))
		{
			graphics::playSound(std::string(ASSET_PATH) + "fx_menu_up_down.mp3", 0.5f, false);
			menuOption = 2;
			timePassedOnStartScreen = 0.0f;
		}

		// The Player chooses the New Game option.
		if (graphics::getKeyState(graphics::SCANCODE_RETURN) && (menuOption == 1))
		{
			graphics::playSound(std::string(ASSET_PATH) + "fx_menu_enter.mp3", 0.4f, false);
			isOnStartMenu = false;
			subMenuChooseDifficulty = true;
			subMenuOption = 1;
			justPressedButtonMainMenu = true;
			justPressedEnterForSubMenu = true;
		}

		// The Player chooses to watch the Credits Screen.
		if (graphics::getKeyState(graphics::SCANCODE_RETURN) && (menuOption == 2) && !justPressedButtonMainMenu)
		{
			graphics::playSound(std::string(ASSET_PATH) + "fx_menu_enter.mp3", 0.4f, false);
			status = STATUS_CREDITS;
			isOnStartMenu = false;
			subMenuChooseDifficulty = false;
			timePassedOnCreditScreen = 0.0f;
			whatToShowOnCreditScreen = 0;
			graphics::playMusic(std::string(ASSET_PATH) + "bgm_childhoods_end.mp3", 0.5f, false, 8000);


		}
		justPressedButtonMainMenu = false;
	}

	// Here is the code for the Choose Mode Menu.
	timePassedOnSubMenu = timePassedOnSubMenu + graphics::getDeltaTime();
	if (timePassedOnSubMenu > 200 && subMenuChooseDifficulty && !isOnStartMenu)
	{
		if (graphics::getKeyState(graphics::SCANCODE_DOWN))
		{
			graphics::playSound(std::string(ASSET_PATH) + "fx_menu_up_down.mp3", 0.5f, false);
			subMenuOption = 2;
			timePassedOnSubMenu = 0.0f;
		}
		if (graphics::getKeyState(graphics::SCANCODE_UP))
		{
			graphics::playSound(std::string(ASSET_PATH) + "fx_menu_up_down.mp3", 0.5f, false);
			subMenuOption = 1;
			timePassedOnSubMenu = 0.0f;
		}

		if (justPressedEnterForSubMenu)
		{
			timeToDistinguishEntersBetweenMainAndSubMenus = timeToDistinguishEntersBetweenMainAndSubMenus + graphics::getDeltaTime();
			if (timeToDistinguishEntersBetweenMainAndSubMenus > 400) {
				justPressedEnterForSubMenu = false;
				timeToDistinguishEntersBetweenMainAndSubMenus = 0.0f;
			}
		}
		if (graphics::getKeyState(graphics::SCANCODE_RETURN) && !(justPressedEnterForSubMenu))
		{
			graphics::playSound(std::string(ASSET_PATH) + "fx_menu_enter.mp3", 0.4f, false);
			status = STATUS_PLAYING;
			graphics::playMusic(std::string(ASSET_PATH) + "bgm_sacrilegium.mp3", 0.5f, true, 4000);
			timePassedOnSubMenu = 0.0f;
			isOnStartMenu = false;
			subMenuChooseDifficulty = false;
		}

		if (graphics::getKeyState(graphics::SCANCODE_BACKSPACE))
		{
			graphics::playSound(std::string(ASSET_PATH) + "fx_menu_backspace.mp3", 1.0f, false);
			subMenuChooseDifficulty = false;
			timePassedOnSubMenu = 0.0f;
			isOnStartMenu = true;
		}
	}

	// Here the player chooses to see the Controls submenu.
	if (graphics::getKeyState(graphics::SCANCODE_RETURN) && (menuOption == 3))
	{
		graphics::playSound(std::string(ASSET_PATH) + "fx_menu_enter.mp3", 0.4f, false);
		pressedControls = true;
		isOnStartMenu = false;
	}

	// Here he can come back to the Start Menu.
	if (graphics::getKeyState(graphics::SCANCODE_BACKSPACE) && (menuOption == 3))
	{
		graphics::playSound(std::string(ASSET_PATH) + "fx_menu_backspace.mp3", 1.0f, false);
		pressedControls = false;
		isOnStartMenu = true;
	}
	justPressedButtonMainMenu = false;
}

void Game::drawStartScreen()
{
	std::string option; // Needed for printing text
	graphics::Brush br;

	// Draws the background of the Start Screen
	br.texture = std::string(ASSET_PATH) + "background_mainmenu.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	// Draws the game's logo.
	br.texture = std::string(ASSET_PATH) + "logo.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, 62, 295, 193.11f, br);

	// Here the Start Menu is drawn.
	if (isOnStartMenu && !subMenuChooseDifficulty) {
		if (menuOption == 1)
		{
			option = "> ";
			graphics::drawText(CANVAS_WIDTH / 2 - 200, CANVAS_HEIGHT / 2 - 150, 50, option, br);
		}
		option = "New Game";
		graphics::drawText(CANVAS_WIDTH / 2 - 160, CANVAS_HEIGHT / 2 - 150, 50, option, br);

		if (menuOption == 2)
		{
			option = "> ";
			graphics::drawText(CANVAS_WIDTH / 2 - 180, CANVAS_HEIGHT / 2 - 50, 50, option, br);
		}
		option = "Credits";
		graphics::drawText(CANVAS_WIDTH / 2 - 140, CANVAS_HEIGHT / 2 - 50, 50, option, br);

		if (menuOption == 3)
		{
			option = "> ";
			graphics::drawText(CANVAS_WIDTH / 2 - 210, CANVAS_HEIGHT / 2 + 50, 50, option, br);
		}
		option = "Controls";
		graphics::drawText(CANVAS_WIDTH / 2 - 170, CANVAS_HEIGHT / 2 + 50, 50, option, br);

		// Here the the keys and their text are being drawn.
		br.texture = std::string(ASSET_PATH) + "key_enter.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 - 150, CANVAS_HEIGHT / 2 + 130, 50, 24, br);

		option = "Confirm";
		graphics::drawText(CANVAS_WIDTH / 2 - 220, CANVAS_HEIGHT / 2 + 110, 20, option, br);

		br.texture = std::string(ASSET_PATH) + "keys_up_down.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 + 20, CANVAS_HEIGHT / 2 + 135, 20, 44.35f, br);

		option = "Navigate";
		graphics::drawText(CANVAS_WIDTH / 2 - 50, CANVAS_HEIGHT / 2 + 110, 20, option, br);

		br.texture = std::string(ASSET_PATH) + "key_esc.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 + 180, CANVAS_HEIGHT / 2 + 130, 25, 23.5f, br);

		option = "Exit";
		graphics::drawText(CANVAS_WIDTH / 2 + 150, CANVAS_HEIGHT / 2 + 110, 20, option, br);
	}

	// Here the sub menu Choose Mode is drawn
	if (subMenuChooseDifficulty)
	{
		option = "Choose Mode";
		graphics::drawText(CANVAS_WIDTH / 2 - 225, CANVAS_HEIGHT / 2 - 150, 50, option, br);

		if (subMenuOption == 1)
		{
			option = "> ";
			graphics::drawText(CANVAS_WIDTH / 2 - 135, CANVAS_HEIGHT / 2 - 50, 30, option, br);
		}
		option = "The Gamer";
		graphics::drawText(CANVAS_WIDTH / 2 - 110, CANVAS_HEIGHT / 2 - 50, 30, option, br);

		if (subMenuOption == 2)
		{
			option = "> ";
			graphics::drawText(CANVAS_WIDTH / 2 - 150, CANVAS_HEIGHT / 2 + 30, 30, option, br);
		}
		option = "The Realist";
		graphics::drawText(CANVAS_WIDTH / 2 - 125, CANVAS_HEIGHT / 2 + 30, 30, option, br);



		br.texture = std::string(ASSET_PATH) + "key_enter.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 - 150, CANVAS_HEIGHT / 2 + 130, 50, 24, br);

		option = "Confirm";
		graphics::drawText(CANVAS_WIDTH / 2 - 220, CANVAS_HEIGHT / 2 + 110, 20, option, br);

		br.texture = std::string(ASSET_PATH) + "keys_up_down.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 + 20, CANVAS_HEIGHT / 2 + 135, 20, 44.35f, br);

		option = "Navigate";
		graphics::drawText(CANVAS_WIDTH / 2 - 50, CANVAS_HEIGHT / 2 + 110, 20, option, br);

		br.texture = std::string(ASSET_PATH) + "key_backspace.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 + 180, CANVAS_HEIGHT / 2 + 130, 50, 23.95f, br);

		option = "Back";
		graphics::drawText(CANVAS_WIDTH / 2 + 150, CANVAS_HEIGHT / 2 + 110, 20, option, br);
	}

	// Here the Controls submenu is being drawn.
	if (pressedControls)
	{
		br.texture = std::string(ASSET_PATH) + "keys_wasd.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 + 100, CANVAS_HEIGHT / 2 - 120, 200, 127.12f, br);

		option = "Move";
		graphics::drawText(CANVAS_WIDTH / 2 - 200, CANVAS_HEIGHT / 2 - 110, 50, option, br);

		br.texture = std::string(ASSET_PATH) + "left_click.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 + 110, CANVAS_HEIGHT / 2, 50, 109.28f, br);

		option = "Shoot";
		graphics::drawText(CANVAS_WIDTH / 2 - 200, CANVAS_HEIGHT / 2 + 30, 50, option, br);

		br.texture = std::string(ASSET_PATH) + "key_backspace.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 - 100, CANVAS_HEIGHT / 2 + 130, 50, 23.95f, br);

		option = "Back";
		graphics::drawText(CANVAS_WIDTH / 2 - 135, CANVAS_HEIGHT / 2 + 110, 20, option, br);

		br.texture = std::string(ASSET_PATH) + "key_esc.png";
		br.outline_opacity = 0.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 + 90, CANVAS_HEIGHT / 2 + 130, 25, 23.5f, br);

		option = "Exit";
		graphics::drawText(CANVAS_WIDTH / 2 + 60, CANVAS_HEIGHT / 2 + 110, 20, option, br);
	}
}

void Game::updateLevelScreen()
{
	// The Level ends when all the waves are killed.
	if (currentWave > MAX_WAVES)
	{
		status = STATUS_CREDITS;
		timePassedOnCreditScreen = 0.0f;
		whatToShowOnCreditScreen = 0;
		graphics::playMusic(std::string(ASSET_PATH) + "bgm_childhoods_end.mp3", 0.5f, false, 8000);
	}
	else {
		// All the actions needed to be taken for Player.
		spawnPlayer(); // When we start the game first we spawn the Player.
		updatePlayer(); // And then it is updated.
		// Here is checked if the player has the left mouse button pressed and if yes bullets are spawn, also the player can shoot only between 1.25 sec for realism.
		graphics::MouseState ms;
		graphics::getMouseState(ms);
		timePassedSincePlayerShooted = timePassedSincePlayerShooted + graphics::getDeltaTime();
		if (ms.button_left_pressed && timePassedSincePlayerShooted >= 1250)
		{
			timePassedSincePlayerShooted = 0;
			playerShootsBullets();
		}
		updatePlayersBullets(); // Updates the Player's Bullets.
		checkPlayersBullets(); // Checks the state of Player's Bullets.

		// All the actions needed to be taken for Enemies.
		spawnEnemies(); // Enemies are being spawn here.
		updateEnemies(); // Here they are updated.
		enemyShootBullet(); // An Enemy is being chosen here to shoot a Bullet. If the same Enemy as last time is chosen no one shoots.
		updateEnemiesBullets(); // Then they are updated.
		checkEnemiesBullets(); // Checks the state of Enemies' Bullets.

		checkCollisions(); // Here all possible Collisions are checked (also here are the actions when Player or an Enemy dies).
	}

}

void Game::drawLevelScreen()
{
	graphics::Brush br;

	// Draws the Level's background.
	br.texture = std::string(ASSET_PATH) + "background_level.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	// Draws the Player.
	if (player_initialized)
	{
		player->draw();
	}

	// Draws the Enemies.
	if (!enemiesOnScreen.empty())
	{
		std::set<Enemy*>::iterator itEnemy;
		for (itEnemy = enemiesOnScreen.begin(); itEnemy != enemiesOnScreen.end(); ++itEnemy)
		{
			(*itEnemy)->draw();
		}
	}

	// Draws Player's Bullets.
	if (!playersBullets.empty())
	{
		std::set<Bullet*>::iterator itPlayersBullet;
		for (itPlayersBullet = playersBullets.begin(); itPlayersBullet != playersBullets.end(); ++itPlayersBullet)
		{
			(*itPlayersBullet)->draw();
		}
	}

	// Draws Enemies' Bullets.
	if (!enemiesBullets.empty())
	{
		std::set<Bullet*>::iterator itEnemyBullet;
		for (itEnemyBullet = enemiesBullets.begin(); itEnemyBullet != enemiesBullets.end(); ++itEnemyBullet)
		{
			(*itEnemyBullet)->draw();
		}
	}

	// Draws the back of the healthbar just to not appear when life < 1.0f.
	br.texture = std::string(ASSET_PATH) + "healthbar_behind.png";
	graphics::drawRect(CANVAS_WIDTH - 75, 50, 80.89f, 15, br);

	// Draws Player's life if the Player chose The Gamer Mode.
	if (subMenuOption == 1)
	{
		float player_life = player ? player->getRemainingLife() : 0.0f;

		br.outline_opacity = 0.0f;
		br.fill_color[0] = 1.0f;
		br.fill_color[1] = 0.2f;
		br.fill_color[2] = 0.2f;
		br.texture = "";
		br.fill_secondary_color[0] = 1.0f * (1.0f - player_life) + player_life * 0.2f;
		br.fill_secondary_color[1] = 0.2f;
		br.fill_secondary_color[2] = 0.2f * (1.0f - player_life) + player_life * 1.0f;
		br.gradient = true;
		br.gradient_dir_u = 1.0f;
		br.gradient_dir_v = 0.0f;
		graphics::drawRect(CANVAS_WIDTH - 73 - ((1.0f - player_life) * 78 / 2), 50, player_life * 78, 15, br);
	}

	// Draws the texture above health.
	br.texture = "";
	br.outline_opacity = 0.0f;
	br.fill_opacity = 1.0f;
	br.fill_color[0] = 1.0f;
	br.fill_color[1] = 1.0f;
	br.fill_color[2] = 1.0f;
	br.fill_secondary_color[0] = 1.0f;
	br.fill_secondary_color[1] = 1.0f;
	br.fill_secondary_color[2] = 1.0f;
	br.texture = std::string(ASSET_PATH) + "healthbar.png";
	graphics::drawRect(CANVAS_WIDTH - 100, 50, 174, 50, br);

	// Here the realistic heart is drawn when the Player chooses The Realist mode.
	if (subMenuOption == 2)
	{
		br.texture = std::string(ASSET_PATH) + "healthbar_realistic_heart.png";
		graphics::drawRect(CANVAS_WIDTH - 163, 50, 45, 40.5, br);
	}

	// Prints the hanging sign just below the healthbar.
	br.texture = std::string(ASSET_PATH) + "hanging_sign_board.png";
	graphics::drawRect(CANVAS_WIDTH - 80, 100, 117.25f, 90, br);

	// Prints above the hanging sign the wave's number
	std::string printWave = "Wave: " + std::to_string(currentWave);
	graphics::drawText(CANVAS_WIDTH - 130, 130, 20, printWave, br);

	// For debugging purposes. Prints the X an Y of the Player.
	if (player && debug_mode)
	{
		char info[40];
		sprintf_s(info, "(%6.2f, %6.2f)", player->getPosX(), player->getPosY());
		graphics::drawText(20, 30, 20, info, br);
	}
}

void Game::spawnPlayer()
{
	if (!player_initialized)
	{
		player = new Player(*this);
		if (subMenuOption == 1)
		{
			player->setLife(1.0f);
		}
		else
		{
			player->setLife(0.0f);
		}
		player_initialized = true;
	}
}

void Game::updatePlayer()
{
	if (player)
	{
		player->update();
	}
}

void Game::playerShootsBullets()
{
	Bullet* bullet = new Bullet(*this, player->getPosX(), player->getPosY(), true);
	playersBullets.insert(bullet);
}

void Game::updatePlayersBullets()
{
	if (!playersBullets.empty())
	{
		std::set<Bullet*>::iterator itPlayersBullet;
		for (itPlayersBullet = playersBullets.begin(); itPlayersBullet != playersBullets.end(); ++itPlayersBullet)
		{
			(*itPlayersBullet)->update();
		}
	}
}

void Game::checkPlayersBullets()
{
	if (!playersBullets.empty())
	{
		std::set<Bullet*>::iterator itPlayersBullet;
		for (itPlayersBullet = playersBullets.begin(); itPlayersBullet != playersBullets.end(); ++itPlayersBullet)
		{
			if (!(*itPlayersBullet)->isActive())
			{
				delete (*itPlayersBullet);
				itPlayersBullet = playersBullets.erase(itPlayersBullet);
			}
		}
	}
}

void Game::spawnEnemies()
{
	if (enemiesOnScreen.empty())
	{
		float tempEnemyPosX = 0;
		for (int i = 0; i < numberOfEnemies; i++)
		{
			tempEnemyPosX = ithEnemysPositionX(i, numberOfEnemies);
			Enemy* tempEnemy = new Enemy(*this, tempEnemyPosX);
			if (subMenuOption == 1)
			{
				tempEnemy->setLife(1.0f);
			}
			else
			{
				tempEnemy->setLife(0.0f);
			}

			enemiesOnScreen.insert(tempEnemy);
			enemiesAlive++;
		}
		currentWave++;
	}
}

float Game::ithEnemysPositionX(int iOfCurrentEnemy, int maxEnemies)
{
	float enemyPosX = iOfCurrentEnemy * (1160.0f / maxEnemies);
	return enemyPosX;
}

void Game::updateEnemies()
{
	if (!enemiesOnScreen.empty())
	{
		std::set<Enemy*>::iterator itEnemy;
		for (itEnemy = enemiesOnScreen.begin(); itEnemy != enemiesOnScreen.end(); ++itEnemy)
		{
			(*itEnemy)->update();
		}
	}
}

void Game::enemyShootBullet()
{
	// If there is only one Enemy rand() is not needed. It slows down the selection.
	if (enemiesAlive == 1)
	{
		twoOrLessEnemiesTimePassedSinceShoot = twoOrLessEnemiesTimePassedSinceShoot + graphics::getDeltaTime();
		if (twoOrLessEnemiesTimePassedSinceShoot >= 1150)
		{
			std::set<Enemy*>::iterator itEnemy;
			for (itEnemy = enemiesOnScreen.begin(); itEnemy != enemiesOnScreen.end(); ++itEnemy)
			{
				Bullet* bullet = new Bullet(*this, (*itEnemy)->getPosX(), (*itEnemy)->getPosY(), false);
				enemiesBullets.insert(bullet);
				twoOrLessEnemiesTimePassedSinceShoot = 0;
			}
		}
	}
	else if (enemiesAlive == 2) { // The same applies for two Enemies. rand() seemed to slow down the selection.
		twoOrLessEnemiesTimePassedSinceShoot = twoOrLessEnemiesTimePassedSinceShoot + graphics::getDeltaTime();
		if (twoOrLessEnemiesTimePassedSinceShoot >= 1250)
		{
			std::set<Enemy*>::iterator itEnemy;
			for (itEnemy = enemiesOnScreen.begin(); itEnemy != enemiesOnScreen.end(); ++itEnemy)
			{
				if (changeBetweenTwoEnemies)
				{
					Bullet* bullet = new Bullet(*this, (*itEnemy)->getPosX(), (*itEnemy)->getPosY(), false);
					enemiesBullets.insert(bullet);
					twoOrLessEnemiesTimePassedSinceShoot = 0;
					changeBetweenTwoEnemies = false;
					break;
				}
				if (!changeBetweenTwoEnemies)
				{
					++itEnemy;
					Bullet* bullet = new Bullet(*this, (*itEnemy)->getPosX(), (*itEnemy)->getPosY(), false);
					enemiesBullets.insert(bullet);
					twoOrLessEnemiesTimePassedSinceShoot = 0;
					changeBetweenTwoEnemies = true;
					break;
				}
			}
		}
	}
	else {
		srand(time(0));
		int randNumb = (rand() % 5) + 1; // random number [1,5]
		int checkingEnemy = 0;

		std::set<Enemy*>::iterator itEnemy;
		for (itEnemy = enemiesOnScreen.begin(); itEnemy != enemiesOnScreen.end(); ++itEnemy)
		{
			if (randNumb == lastEnemyThatShooted) // Every time a different enemy than the last one has to shoot.
			{
				break;
			}

			checkingEnemy++;

			// Anyone but the middle Enemy can shoot the Player when the Level starts.
			if ((checkingEnemy == 3) && (currentWave == 1)) // Don't want to compute it forever.
			{
				timePassedToAllowMiddleEnemyToShoot = timePassedToAllowMiddleEnemyToShoot + graphics::getDeltaTime();
			}
			if ((checkingEnemy == 3) && (currentWave == 1) && (timePassedToAllowMiddleEnemyToShoot > 2000))
			{
				continue;
			}

			if ((randNumb == checkingEnemy)) // If a different Enemy than the last time is chosen to shoot.
			{
				Bullet* bullet = new Bullet(*this, (*itEnemy)->getPosX(), (*itEnemy)->getPosY(), false);
				enemiesBullets.insert(bullet);
				lastEnemyThatShooted = randNumb;
				break;
			}
		}
	}
}

void Game::updateEnemiesBullets()
{
	if (!enemiesBullets.empty())
	{
		std::set<Bullet*>::iterator itEnemyBullet;
		for (itEnemyBullet = enemiesBullets.begin(); itEnemyBullet != enemiesBullets.end(); ++itEnemyBullet)
		{
			(*itEnemyBullet)->update();
		}
	}
}

void Game::checkEnemiesBullets()
{
	if (!enemiesBullets.empty())
	{
		std::set<Bullet*>::iterator itEnemyBullet;
		for (itEnemyBullet = enemiesBullets.begin(); itEnemyBullet != enemiesBullets.end(); ++itEnemyBullet)
		{
			if (!(*itEnemyBullet)->isActive())
			{
				delete (*itEnemyBullet);
				itEnemyBullet = enemiesBullets.erase(itEnemyBullet);
			}
		}
	}
}

void Game::checkCollisions()
{
	// 1) Checks if any of the Player's Bullets collide with any of the Enemies
	std::set<Bullet*>::iterator itPlayersBullet;
	for (itPlayersBullet = playersBullets.begin(); itPlayersBullet != playersBullets.end(); ++itPlayersBullet)
	{
		Disk playersBulletDisk = (*itPlayersBullet)->getCollisionHull();

		std::set<Enemy*>::iterator itEnemy;
		for (itEnemy = enemiesOnScreen.begin(); itEnemy != enemiesOnScreen.end(); ++itEnemy)
		{
			Disk enemysDisk = (*itEnemy)->getCollisionHull();

			float dxPlayersBullet2Soldier = playersBulletDisk.cx - enemysDisk.cx;
			float dyPlayersBullet2Soldier = playersBulletDisk.cy - enemysDisk.cy;

			// Checks if there is a Collision and if yes the Enemy being examined loses life (if life = 0.0f, he dies).
			if (sqrt(dxPlayersBullet2Soldier * dxPlayersBullet2Soldier + dyPlayersBullet2Soldier * dyPlayersBullet2Soldier) < enemysDisk.radius + playersBulletDisk.radius)
			{
				(*itPlayersBullet)->setHitTarget(true);
				(*itEnemy)->drainLife(0.1f);
				graphics::playSound(std::string(ASSET_PATH) + "fx_getting_shot.mp3", 0.5f, false);

				if (!(*itEnemy)->isActive())
				{
					delete (*itEnemy);
					itEnemy = enemiesOnScreen.erase(itEnemy);
					enemiesAlive--;
				}
			}
		}
	}

	// 2) Checks if any of the Enemies' Bullets collide with Player
	Disk playersDisk = player->getCollisionHull();

	std::set<Bullet*>::iterator itEnemiesBullet;
	for (itEnemiesBullet = enemiesBullets.begin(); itEnemiesBullet != enemiesBullets.end(); ++itEnemiesBullet)
	{
		Disk enemiesBulletDisk = (*itEnemiesBullet)->getCollisionHull();

		float dxEnemiesBullet2Player = playersDisk.cx - enemiesBulletDisk.cx;
		float dyEnemiesBullet2Player = playersDisk.cy - enemiesBulletDisk.cy;

		// Checks if there is a Collision and if yes the Player loses life (if life = 0.0f, it dies).
		if (sqrt(dxEnemiesBullet2Player * dxEnemiesBullet2Player + dyEnemiesBullet2Player * dyEnemiesBullet2Player) < playersDisk.radius + enemiesBulletDisk.radius)
		{
			(*itEnemiesBullet)->setHitTarget(true);
			player->drainLife(0.1f);
			graphics::playSound(std::string(ASSET_PATH) + "fx_getting_shot.mp3", 0.5f, false);

			if (!player->isActive())
			{
				delete player;
				player_initialized = false;
				status = STATUS_CREDITS;
				timePassedOnCreditScreen = 0.0f;
				whatToShowOnCreditScreen = 0;
				graphics::playMusic(std::string(ASSET_PATH) + "bgm_childhoods_end.mp3", 0.5f, false, 8000);

			}
		}
	}
}

void Game::updateCreditsScreen()
{
	// Here all the events of the Credit Screen are timed.
	timePassedOnCreditScreen = timePassedOnCreditScreen + graphics::getDeltaTime();
	if (timePassedOnCreditScreen >= 2000 && (whatToShowOnCreditScreen == 0))
	{
		whatToShowOnCreditScreen = 1;
	}
	if (timePassedOnCreditScreen >= 7500 && (whatToShowOnCreditScreen == 1))
	{
		whatToShowOnCreditScreen = 2;
	}
	if (timePassedOnCreditScreen >= 12750 && (whatToShowOnCreditScreen == 2))
	{
		whatToShowOnCreditScreen = 3;
	}
	if (timePassedOnCreditScreen >= 18250 && (whatToShowOnCreditScreen == 3))
	{
		whatToShowOnCreditScreen = 4;
	}
	if (timePassedOnCreditScreen >= 20450 && (whatToShowOnCreditScreen == 4))
	{
		whatToShowOnCreditScreen = 5;
	}
	if (timePassedOnCreditScreen >= 30000 && (whatToShowOnCreditScreen == 5))
	{
		wipeFirstCreditScreen = true;
	}
	if (timePassedOnCreditScreen >= 31250 && wipeFirstCreditScreen)
	{
		whatToShowOnCreditScreen = 6;
	}
	if (timePassedOnCreditScreen >= 32250 && wipeFirstCreditScreen)
	{
		whatToShowOnCreditScreen = 7;
	}
	if (timePassedOnCreditScreen >= 33250 && wipeFirstCreditScreen)
	{
		whatToShowOnCreditScreen = 8;
	}
	if (timePassedOnCreditScreen >= 34250 && wipeFirstCreditScreen)
	{
		whatToShowOnCreditScreen = 9;
	}
	if (timePassedOnCreditScreen >= 35250 && wipeFirstCreditScreen)
	{
		whatToShowOnCreditScreen = 10;
	}
	if (timePassedOnCreditScreen >= 36250 && (whatToShowOnCreditScreen == 10))
	{
		whatToShowOnCreditScreen = 11;
	}
	if (timePassedOnCreditScreen >= 37250 && (whatToShowOnCreditScreen == 11))
	{
		whatToShowOnCreditScreen = 12;
	}
	if (timePassedOnCreditScreen >= 41250 && (whatToShowOnCreditScreen == 12))
	{
		whatToShowOnCreditScreen = 13;
	}
	if (timePassedOnCreditScreen >= 42250 && (whatToShowOnCreditScreen == 13))
	{
		whatToShowOnCreditScreen = 14;
	}
	if (timePassedOnCreditScreen >= 46250 && (whatToShowOnCreditScreen == 14))
	{
		whatToShowOnCreditScreen = 15;
	}
	if (timePassedOnCreditScreen >= 280000)
	{
		status = STATUS_START;
		graphics::playMusic(std::string(ASSET_PATH) + "bgm_what_a_wonderful_world.mp3", 0.5f, true, 4000); // edw prepei na valw to its a berautiful world
		menuOption = 1;
		isOnStartMenu = true;
		subMenuChooseDifficulty = false;
		menuOption = 1;
		timePassedOnCreditScreen = 0.0f;
		whatToShowOnCreditScreen = 0;
		wipeFirstCreditScreen = false;
		clearAll();
	}

	// If Backspace is pressed the game returns to the Start Screen.
	if (graphics::getKeyState(graphics::SCANCODE_BACKSPACE))
	{
		status = STATUS_START;
		graphics::playSound(std::string(ASSET_PATH) + "fx_menu_backspace.mp3", 1.0f, false);
		graphics::playMusic(std::string(ASSET_PATH) + "bgm_what_a_wonderful_world.mp3", 0.5f, true, 4000);
		isOnStartMenu = true;
		subMenuChooseDifficulty = false;
		menuOption = 1;
		timePassedOnCreditScreen = 0.0f;
		whatToShowOnCreditScreen = 0;
		wipeFirstCreditScreen = false;
		clearAll();
	}
}

void Game::drawCreditsScreen()
{
	graphics::Brush br;
	std::string text;

	// Draws the background of the Credits Screen.
	br.texture = std::string(ASSET_PATH) + "background_credits.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2, CANVAS_WIDTH, CANVAS_HEIGHT, br);

	// Draws the keys and their text.
	br.texture = std::string(ASSET_PATH) + "key_backspace.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(200, CANVAS_HEIGHT - 50, 100, 47.9f, br);

	text = "Back to Main Menu";
	graphics::drawText(30, CANVAS_HEIGHT - 90, 30, text, br);

	br.texture = std::string(ASSET_PATH) + "key_esc.png";
	br.outline_opacity = 0.0f;
	graphics::drawRect(CANVAS_WIDTH - 150, CANVAS_HEIGHT - 50, 50, 47, br);

	text = "Exit";
	graphics::drawText(CANVAS_WIDTH - 200, CANVAS_HEIGHT - 90, 30, text, br);

	// Draws the window.
	br.texture = std::string(ASSET_PATH) + "window.png";
	br.outline_opacity = 0.0f;
	br.fill_opacity = 0.75f;
	graphics::drawRect(CANVAS_WIDTH / 2, CANVAS_HEIGHT / 2 - 50, 1000, 518, br);

	// The interplay between drawCreditsScreen() and updateCreditsScreen() draws the event-driven Credits Screen.
	if (whatToShowOnCreditScreen <= 5)
	{
		text = "This game is dedicated to all child";
		graphics::drawText(260, CANVAS_HEIGHT / 2 - 200, 30, text, br);

		text = "soldiers, that are, have been and will be.";
		graphics::drawText(190, CANVAS_HEIGHT / 2 - 160, 30, text, br);
	}

	if (whatToShowOnCreditScreen >= 2 && whatToShowOnCreditScreen < 7)
	{
		text = "Especially children shouldn't be used";
		graphics::drawText(210, CANVAS_HEIGHT / 2 - 100, 30, text, br);

		text = "in conflict.";
		graphics::drawText(CANVAS_WIDTH / 2 - 120, CANVAS_HEIGHT / 2 - 60, 30, text, br);
	}
	if (whatToShowOnCreditScreen >= 3 && whatToShowOnCreditScreen < 8)
	{
		text = "Although it is hard to count, there are";
		graphics::drawText(210, CANVAS_HEIGHT / 2, 30, text, br);

		text = "an estimated 250,000 child soldiers in";
		graphics::drawText(210, CANVAS_HEIGHT / 2 + 40, 30, text, br);

		text = "the world today.";
		graphics::drawText(CANVAS_WIDTH / 2 - 160, CANVAS_HEIGHT / 2 + 80, 30, text, br);
	}
	if (whatToShowOnCreditScreen >= 4 && whatToShowOnCreditScreen < 9)
	{
		text = "Stop the war.";
		graphics::drawText(CANVAS_WIDTH / 2 - 120, CANVAS_HEIGHT / 2 + 140, 30, text, br);
	}
	if (whatToShowOnCreditScreen >= 5 && whatToShowOnCreditScreen < 9)
	{
		br.texture = std::string(ASSET_PATH) + "stop_the_war.png";
		br.outline_opacity = 0.0f;
		br.fill_opacity = 1.0f;
		graphics::drawRect(CANVAS_WIDTH / 2 + 220, CANVAS_HEIGHT / 2 + 150, 100, 122.84f, br);
	}
	if (whatToShowOnCreditScreen >= 10 && whatToShowOnCreditScreen <= 14)
	{
		text = "CREDITS";
		graphics::drawText(CANVAS_WIDTH / 2 - 80, CANVAS_HEIGHT / 2 - 220, 30, text, br);
	}
	if (whatToShowOnCreditScreen >= 11 && whatToShowOnCreditScreen <= 12)
	{
		text = "Concept inspired by Heaven Shall Burn's";
		graphics::drawText(200, CANVAS_HEIGHT / 2 - 160, 30, text, br);

		text = "music video \"Combat\".";
		graphics::drawText(CANVAS_WIDTH / 2 - 200, CANVAS_HEIGHT / 2 - 120, 30, text, br);
	}
	if (whatToShowOnCreditScreen == 12)
	{
		text = "Almost all assets are cropped by frames";
		graphics::drawText(200, CANVAS_HEIGHT / 2 - 60, 30, text, br);

		text = "of that video.";
		graphics::drawText(CANVAS_WIDTH / 2 - 120, CANVAS_HEIGHT / 2 - 20, 30, text, br);
	}

	if (whatToShowOnCreditScreen >= 13 && whatToShowOnCreditScreen <= 14)
	{
		text = "Music Used:";
		graphics::drawText(CANVAS_WIDTH / 2 - 120, CANVAS_HEIGHT / 2 - 160, 30, text, br);
	}
	if (whatToShowOnCreditScreen == 14)
	{
		text = "Louis Armstrong - What A Beautiful World";
		graphics::drawText(190, CANVAS_HEIGHT / 2 - 100, 30, text, br);

		text = "ZEAL & ARDOR - Sacrilegium III";
		graphics::drawText(320, CANVAS_HEIGHT / 2 - 60, 30, text, br);

		text = "Iron Maiden - Childhood's End";
		graphics::drawText(315, CANVAS_HEIGHT / 2 - 20, 30, text, br);
	}
	if (whatToShowOnCreditScreen == 15)
	{
		text = "Thanks for playing!";
		graphics::drawText(CANVAS_WIDTH / 2 - 280, CANVAS_HEIGHT / 2 - 50, 40, text, br);
	}
}

// This method makes sure that when the Player returns from the Credits screen all things (dynamically allocated things mainly) are erased.
void Game::clearAll()
{
	if (!player)
	{
		delete player;
	}
	player_initialized = false;
	timePassedSincePlayerShooted = 0;

	playersBullets.clear();

	numberOfEnemies = 5;
	currentWave = 0;
	enemiesAlive = 0;
	changeBetweenTwoEnemies = false;
	twoOrLessEnemiesTimePassedSinceShoot = 0.0f;
	enemiesOnScreen.clear();

	enemiesBullets.clear();
	lastEnemyThatShooted = 0;

	timePassedOnCreditScreen = 0.0f;
	whatToShowOnCreditScreen = 0;
	wipeFirstCreditScreen = false;
}

#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <random>
#include <ctime>
#include <stdlib.h>

using namespace sf;

int main()
{
	srand((unsigned)time(0)); 

	RenderWindow window(VideoMode(700, 900), "Cooler Jump", Style::Close); 

	window.setFramerateLimit(60);


	// define textures

	Texture 
				backgroundTexture, 
				gameoverTexture, 
				playerTexture,
				platformTexture,
				platformTexture1,
				platformTexture2;

	//load textures from files

	backgroundTexture.loadFromFile("images/background.png");
	playerTexture.loadFromFile("images/doodle.png");
	platformTexture.loadFromFile("images/cloud1.png");
	platformTexture1.loadFromFile("images/cloud2.png");
	platformTexture2.loadFromFile("images/cloud3.png");
	gameoverTexture.loadFromFile("images/gameover.png");

	// to change the platforms' shapes
	int random_platform;

	// create sprites
	Sprite 
				background(backgroundTexture), 
				player(playerTexture),
				platform(platformTexture),
				platform1(platformTexture1),
				platform2(platformTexture2),
				gameover(gameoverTexture);
	/*
	*      ----Size of images----
	*    background.png size: 700 * 900
	*    doodle.png     size:  120 *  120
	*    platform.png   size:  68 *  14
	*/

	// create custom color to match the background

	Color customColor(255, 154, 131);

	// load font

	Font font;
	font.loadFromFile("font/anak.ttf");

	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(50);
	scoreText.setFillColor(customColor);

	Text gameoverText;
	gameoverText.setFont(font);
	gameoverText.setString("Game Over!");
	gameoverText.setCharacterSize(100);
	gameoverText.setFillColor(customColor);

	// sound settings
	// SoundBuffer buffer;

	// buffer.loadFromFile("sound/jump.wav");
	// Sound sound_jump;

	// sound_jump.setBuffer(buffer);

	// initialize platforms

	Vector2u platformPosition[13];
	std::uniform_int_distribution<unsigned> x(0, 700 - platformTexture.getSize().x);
	std::uniform_int_distribution<unsigned> y(0, 800);
	std::default_random_engine e(time(0));

	for (size_t i = 0; i < 13; i++)
	{
		platformPosition[i].x = x(e);
		platformPosition[i].y = y(e);
	}

	// player's positon and down velocity

	int playerX = 250;
	int playerY = 151;
	float dy = 12;
	int height = 150;
	int score = 0;

	// check touching

	const int leftside = 120;
	const int rightside = 120;
	const int bottom = 120;

	while (window.isOpen())
	{
		
		Event event;

		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::A) || Keyboard::isKeyPressed(Keyboard::Left))
			playerX -= 4;
		if (Keyboard::isKeyPressed(Keyboard::D) || Keyboard::isKeyPressed(Keyboard::Right))
			playerX += 4;


		if (playerX > 700)
			playerX = 0;
		if (playerX < -40)
			playerX = window.getSize().x - playerTexture.getSize().x;

		scoreText.setPosition(270, 800);
		scoreText.setString("Score: " + std::to_string(score));

		if (playerY == height && dy < (-1.62))
		{
			score += 1;
		}

		
		dy += 0.15;
		playerY += dy;

		if (playerY < height)
			for (size_t i = 0; i < 13; i++)
			{
				playerY = height;
				platformPosition[i].y -= dy; 
				if (platformPosition[i].y > 750) 
				{
					platformPosition[i].y = 0;
					platformPosition[i].x = x(e);
				}
					
			}
		// detect jump on the platform
		for (size_t i = 0; i < 13; i++)
		{
			if ((playerX + rightside > platformPosition[i].x) && (playerX + leftside < platformPosition[i].x + platformTexture.getSize().x)        
				&& (playerY + bottom > platformPosition[i].y) && (playerY + bottom < platformPosition[i].y + platformTexture.getSize().y)  
				&& (dy > 0))
			{
				// sound_jump.play();
				dy = -13;
			}
				
		}
		player.setPosition(playerX, playerY);
		
		window.draw(background);		
		window.draw(player);

		

		// randomize cloud shapes
		for (size_t i = 0; i < 13; i++)
		{
			random_platform = (rand()%3);
			
			if (random_platform == 0 || random_platform == 1) {
				platform.setPosition(platformPosition[i].x, platformPosition[i].y);
			window.draw(platform);
			}
			if (random_platform == 1 || random_platform == 2) {
				platform1.setPosition(platformPosition[i].x, platformPosition[i].y);
			window.draw(platform1);
			}

			if (random_platform == 2 || random_platform == 0) {
				platform2.setPosition(platformPosition[i].x, platformPosition[i].y);
				window.draw(platform2);
			}
			
			
		}

		// game over
		if (playerY > 800)
		{
			gameoverText.setPosition(70, 650);
			scoreText.setPosition(170, 770);
			goto gameover;
		}
		window.draw(scoreText);
		window.display();
	}

	// Game Over
gameover:
	while (window.isOpen())
	{
		
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}
		window.draw(gameover);
		window.draw(gameoverText);
		window.draw(scoreText);
		window.display();
	}
    return 0;
}

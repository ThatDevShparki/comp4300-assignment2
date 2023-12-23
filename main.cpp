#include <SFML/Graphics.hpp>

#include "Game.h"
#include <iostream>

int main()
{
	Game game("config.txt");
	game.run();
}
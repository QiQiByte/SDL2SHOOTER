#include <iostream>


#include "Game.h"

int main(int , char **) {
    std::cout << "Hello, World!" << std::endl;

    Game &game = Game::getInstance();
    game.init();
    game.run();
    return 0;
}
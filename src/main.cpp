#include <iostream>


#include "Game.h"

int main(int , char **) {
    std::cout << "Hello, World!" << std::endl;

    Game game;
    game.init();
    game.run();
    game.clean();
    return 0;
}
#ifndef GAME_H
#define GAME_H

class Game{
public:
    Game();
    ~Game();

    void init();
    void run();
    void clean();

private:
    bool isRunning;
    
};

#endif
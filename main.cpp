// File: main.cpp
// Author: Timothy Kiyui (4316886)

#include "Looks.h"
#include "Text.h"
#include "Nyan.h"
#include "Asteroid.h"
#include "Game.h"
#include "Screen.h"
#include <fstream>
#include <thread>
#include <chrono>

using namespace std;

vector<string> fromFile(string fileName)
{
    ifstream vectorFile(fileName.c_str());
    vector<string> fileVector;
    string vectorLine("");
    if (vectorFile.is_open())
    {
        while (getline(vectorFile, vectorLine))
            fileVector.push_back(vectorLine);
    }
    return fileVector;
}

void gameThread(Game *game, Screen *gameScreen, Nyan *_nyan, bool *gameLoop)
{
    while (*gameLoop)
    {
        this_thread::sleep_for(chrono::milliseconds(50));
        gameScreen->print(game->print());
        if (_nyan->getScore() == -100)
            *gameLoop = false;
    }
}

void inputThread(Nyan *_nyan, bool *gameLoop)
{
    char userInput;
    while (*gameLoop)
    {
        userInput = getch();
        switch (userInput)
        {
            case 'q': case 'Q':
                *gameLoop = false;
                break;
            case 'w': case 'W':
                _nyan->goUp();
                break;
            case 's': case 'S':
                _nyan->goDown();
                break;
        }
    }
}

int main(void)
{
    Nyan *_nyan = new Nyan(fromFile("NYAN-CAT"), 5, 20);
    Text *_start = new Text(fromFile("NYAN-SPLASH"), "Nyan Curses");
    Text *_end = new Text(fromFile("NYAN-OUT"), "Nyan Out");
    Game *game = new Game(80, 25, _nyan, _start, _end);
    Screen *gameScreen = new Screen(80);
    if (gameScreen == NULL) return 1;
#ifdef USES_CURSES_SCREEN
    initscr();
#endif
    bool gameLoop(true);
    gameScreen->print(game->splash());
    getch();
    thread _gameThread(gameThread, game, gameScreen, _nyan, &gameLoop);
    thread _inputThread(inputThread, _nyan, &gameLoop);
    _gameThread.join();
    _inputThread.join();
    clear();
    gameScreen->print(game->gameOver());
    getch();
#ifdef USES_CURSES_SCREEN
    endwin();
#endif
    delete game;
    delete gameScreen;
}

// File: Game.cpp
// Author: Timothy Kiyui (4316886)

#include "Game.h"
#include "Nyan.h"
#include "Asteroid.h"
#include "Text.h"
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>
#include <ctime>

using namespace std;

string itos(int i)
{
    stringstream myStream;
    string myString("");
    myStream << i;
    myStream >> myString;
    return myString;
}

Game::Game(int width, int height, Nyan *nyan, Text *start, Text *end)
{
    // Set the random seed as this game depends on many random values
    srand(time(NULL));
    // Initialize game values
    if (nyan != NULL)
        _nyan = nyan;
    if (start != NULL)
        _start = start;
    if (end != NULL)
        _end = end;
    // Minimum width for the game is 80 characters, the typical
    // Windows terminal size
    width < 80 ?
        (_width = 80):
        (_width = width);
    // Minimum height for the game is 25 characters, the typical
    // Windows terminal size
    height < 25 ?
        (_height = 25):
        (_height = height);
    // Create asteroids
    char asteroid_icons[8] = {'*', '+', '@', '.', 'X', 'O', '~', '-'};
    for (int i(0); i < 8; i++)
    {
        Asteroid *_asteroid = new Asteroid(asteroid_icons[i],
                rand() % width,
                rand() % height,
                _width,
                _height);
        _asteroids.push_back(_asteroid);
    }
}

Game::~Game()
{
    if (_nyan != NULL)
        delete _nyan;
    if (_start != NULL)
        delete _start;
    if (_end != NULL)
        delete _end;
    for (Asteroid *asteroid: _asteroids)
        delete asteroid;
}

// Game start splash screen
vector<string> Game::splash() { return _start->getLooks(); }

// Game over splash screen
vector<string> Game::gameOver() { return _end->getLooks(); }

// Update the game and return the printable string vector
vector<string> Game::print()
{
    vector<string> toPrint;
    // Status bar
    string statusBar("Score: " + itos(_nyan->getScore()));
    toPrint.push_back(statusBar);
    // Create empty white lines
    string whiteLine("");
    for (int x(0); x < _width; x++) whiteLine += " ";
    // Add white lines until Nyan
    for (int y(1); y < _nyan->getY(); y++)
        toPrint.push_back(whiteLine);
    vector<string> nyanVector = _nyan->getLooks();
    toPrint.insert(toPrint.end(),
            nyanVector.begin(),
            nyanVector.end());
    while (toPrint.size() < _height)
        toPrint.push_back(whiteLine);
    // Normalize the vector with equal lines
    for (int y(0); y < _height; y++)
    {
        while (toPrint[y].length() < _width)
            toPrint[y] += ' ';
    }
    // Update asteroid movements
    for (Asteroid *asteroid: _asteroids)
    {
        asteroid->move();
        // This is to avoid crashes on 64 bit machines
        int asteroidX = asteroid->getX();
        int asteroidY = asteroid->getY();
        if (asteroidX == _width)
            asteroid->setY(rand() % _height);
        // Check for collision with Nyan
        if (asteroidX >= _nyan->getX() && asteroidX <= _nyan->getX() + nyanVector[0].length())
            if (asteroidY >= _nyan->getY() && asteroidY <= _nyan->getY() + nyanVector.size())
                asteroid->collide(_nyan);
        toPrint[asteroid->getY()][asteroidX] = asteroid->getLooks();
    }
    return toPrint;
}

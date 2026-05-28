#include "game.h"
#include <random>
#include "graphics.h"
#include "candy.h"
#include <iostream>
#include <fstream>
#include <string>

Game::Game()
{
    // Implement your code here
    m_board = new Board(10, 10);
    m_frameCount = 0;
    m_score = 0;
    m_gameOver = false;
}

Game::~Game()
{
    // Implement your code here
    if (m_board != nullptr)
    {
        delete m_board;
    }
}

void Game::update(const Controller &controller)
{
    // Implement your code here
}

void Game::render(GraphicManager &graphics)
{
    // Implement your code here
    // Note: the following code exhibits the main graphic library features
    // Board: border [draw rectangles] and a single piece of candy
    const int board_size = 10;
    const int board_padding = 3;
    graphics.drawRectangle(
        CANDY_IMAGE_HEIGHT * board_padding, CANDY_IMAGE_HEIGHT * board_padding,
        CANDY_IMAGE_WIDTH * board_size,
        CANDY_IMAGE_HEIGHT * board_size,
        5, 150, 150, 150);
    // Board: place a candy piece
    for (int y = 0; y < m_board->getHeight(); y++)
    {
        for (int x = 0; x < m_board->getWidth(); x++)
        {
            Candy* currCandy = m_board->getCell(x, y);
            if (currCandy != nullptr)
            {
                int posX = (x + board_padding) * CANDY_IMAGE_WIDTH;
                int posY = (y + board_padding) * CANDY_IMAGE_HEIGHT;
                graphics.drawImage(currCandy->getResourceName(), posX, posY);
            }
        }
    }
    // Title [draw images]
    graphics.drawImage("img/logo_small.png", 10, 10);
    // Score and footer [draw text]
    graphics.drawText("Movement: [Up] [Down] [Left] [Right]  --  "
                      "Buttons: [Q] [W] [E]  --  Exit [ESC]",
                      25, 700, 20, 100, 100, 100);
    string scoreText = "Score: " + to_string(m_score);
    graphics.drawText(scoreText, 500, 30, 32, 125, 200, 125);
}

void Game::run()
{
    const int screen_width = 750;
    const int screen_height = 750;
    const int bg_red = 255;
    const int bg_green = 255;
    const int bg_blue = 255;
    runGraphicGame(*this, screen_width, screen_height, bg_red, bg_green, bg_blue);
}

bool Game::dump(const std::string &output_path) const
{
    // Implement your code here
    bool dumpedGame = false;
    ofstream file(output_path);
    if (file.is_open())
    {
        file << m_frameCount << " " << m_score << " " << m_gameOver << endl;
        dumpedGame = m_board->dump(output_path);
    }
    return dumpedGame;
}

bool Game::load(const std::string &input_path)
{
    // Implement your code here
    bool loadedGame = false;
    ifstream file(input_path);
    if (file.is_open())
    {
        file >> m_frameCount >> m_score >> m_gameOver;
        loadedGame = m_board->load(input_path);
    }
    return loadedGame;
}

bool Game::operator==(const Game &other) const
{
    // Implement your code here
    bool equalGame = m_score == other.m_score && m_frameCount == other.m_frameCount && m_gameOver == other.m_gameOver;
    for (int y = 0; y < m_board->getHeight() && equalGame; y++)
    {
        for (int x = 0; x < m_board->getWidth() && equalGame; x++)
        {
            if (m_board->getCell(x, y) != other.m_board->getCell(x, y))
            {
                equalGame = false;
            }
        }
    }
    return false;
}
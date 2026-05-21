#include <filesystem>
#include "board.h"
#include "candy.h"
#include "controller.h"
#include "game.h"
#include "util.h"

using namespace std;

bool test()
{
    // Test board 2D container
    Candy* c = new Candy(CandyType::TYPE_ORANGE);
    Board b(10, 10);
    b.setCell(c, 0, 0);
    if (b.getCell(0, 0) != c) return false;

    // Explotar 3 caramelos en vertical
    b.setCell(new Candy(CandyType::TYPE_RED), 1, 7);
    b.setCell(new Candy(CandyType::TYPE_RED), 1, 8);
    b.setCell(new Candy(CandyType::TYPE_RED), 1, 9);
    if (!b.shouldExplode(1, 7)) return false;

    // Explotar 3 caramelos en horizontal
    b.setCell(new Candy(CandyType::TYPE_BLUE), 3, 9);
    b.setCell(new Candy(CandyType::TYPE_BLUE), 4, 9);
    b.setCell(new Candy(CandyType::TYPE_BLUE), 5, 9);
    if (!b.shouldExplode(3, 9)) return false;

    // Explotar 3 caramelos en diagonal
    b.setCell(new Candy(CandyType::TYPE_GREEN), 7, 7);
    b.setCell(new Candy(CandyType::TYPE_GREEN), 8, 8);
    b.setCell(new Candy(CandyType::TYPE_GREEN), 9, 9);
    if (!b.shouldExplode(7, 7)) return false;
    
    // Explotar y bajar caramelo
    Candy* c1 = new Candy(CandyType::TYPE_BLUE);
    b.setCell(c1, 1, 6);
    vector<Candy*> explodedCandies = b.explodeAndDrop();
    if (b.getCell(1, 9) != c1) return false;
    if (explodedCandies.empty()) return false;
    
    // Dump and load board
    {
        Board b2(10, 10);
        Candy* c2 = new Candy(CandyType::TYPE_YELLOW);
        if (!b.dump(getDataDirPath() + "dump_board.txt"))
        {
            return false;
        }
        if (!b2.load(getDataDirPath() + "dump_board.txt"))
        {
            return false;
        }
        b2.setCell(c2, 0, 0);
        if (b2.getCell(0, 0)->getType() != c2->getType())
        {
            return false;
        }
        filesystem::remove(getDataDirPath() + "dump_board.txt");
    }
    
    // Dump and load game
    {
        Game g;
        Controller cont;
        g.update(cont);
        if (!g.dump(getDataDirPath() + "dump_game.txt"))
        {
            return false;
        }
        Game g2;
        if (!g2.load(getDataDirPath() + "dump_game.txt"))
        {
            return false;
        }
        if (g != g2)
        {
            return false;
        }
        filesystem::remove(getDataDirPath() + "dump_game.txt");
    }
    return true;
}

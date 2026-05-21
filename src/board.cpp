#include "board.h"
#include <memory>
#include <iostream>
#include <fstream>
#include <vector>
#include "candy.h"
using namespace std;

Board::Board(int width, int height)
{
	// Implement your code here
	m_width = width;
	m_height = height;
	m_board = new Candy**[width];
	for (int y = 0; y < height; y++)
	{
		m_board[y] = new Candy*[height];
		for (int x = 0; x < width; x++)
		{
			m_board[y][x] = nullptr;
		}
	}
}

Board::~Board()
{
	// Implement your code here
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			if (m_board[y][x] != nullptr)
			{
				delete m_board[y][x];
			}
		}
		if (m_board[y] != nullptr)
		{
			delete[] m_board[y];
		}
	}
	if (m_board != nullptr)
	{
		delete m_board;
	}
}

Candy* Board::getCell(int x, int y) const
{
	// Implement your code here
	Candy* candy = nullptr;
	if (x >= 0 && y >= 0 && x < m_width && y < m_height)
	{
		candy = m_board[y][x];
	}
	return candy;
}

void Board::setCell(Candy* candy, int x, int y)
{
	// Implement your code here
	if (x >= 0 && x < m_width && y >= 0 && y < m_height)
	{
		m_board[y][x] = candy;
	}
}

int Board::getWidth() const
{
	// Implement your code here
	return m_width;
}

int Board::getHeight() const
{
	// Implement your code here
	return m_height;
}

bool Board::shouldExplode(int x, int y) const
{
	Candy* candy_actual = getCell(x, y);
	if (x < 0 || x >= m_width || y < 0 || y >= m_height || m_board[y][x] == nullptr) // Si está fuera de los límites del tablero o es una casilla vacía
	{
		return false;
	}

	// Miramos si hay 3 o más casillas del mismo tipo en horizontal
	CandyType tipo = candy_actual->getType();
	int horizontal = 1;
	int i = x + 1;
	while (i < m_width && getCell(i, y) != nullptr && getCell(i, y)->getType() == tipo)
	{
		horizontal++;
		i++;
	}
	i = x - 1;
	while (i >= 0 && getCell(i, y) != nullptr && getCell(i, y)->getType() == tipo)
	{
		horizontal++;
		i--;
	}
	if (horizontal >= SHORTEST_EXPLOSION_LINE)
	{
		return true;
	}

	// Miramos si hay 3 o más casillas del mismo tipo en vertical
	int vertical = 1;
	int j = y + 1;
	while (j < m_height && getCell(x, j) != nullptr && getCell(x, j)->getType() == tipo)
	{
		vertical++;
		j++;
	}
	j = y - 1;
	while (j >= 0 && getCell(x, j) != nullptr && getCell(x, j)->getType() == tipo)
	{
		vertical++;
		j--;
	}
	if (vertical >= SHORTEST_EXPLOSION_LINE)
	{
		return true;
	}

	//Miramos si hay 3 o más casillas del mismo tipo en diagonal
	int diagonal_abajo = 1; // diagonal que baja hacia la derecha
	i = x + 1;
	j = y + 1;
	while (i < m_width && j < m_height && getCell(i, j) != nullptr && getCell(i, j)->getType() == tipo) // mientras que a lo largo de laa diagonal, el tipo sea igual, no sea vacio y no se salga de la matriz ir sumando iteraciones de iguales
	{
		diagonal_abajo++;
		i++;
		j++;
	}
	i = x - 1;
	j = y - 1;
	while (i >= 0 && j >= 0 && getCell(i, j) != nullptr && getCell(i, j)->getType() == tipo) // mientras que a lo largo de la diagonal, el tipo sea igual, no sea vacio y no se salga de la matriz ir sumando iteraciones de iguales
	{
		diagonal_abajo++;
		i--;
		j--;
	}
	if (diagonal_abajo >= SHORTEST_EXPLOSION_LINE)
	{
		return true;
	}
	int diagonal_arriba = 1; // Diagonal que sube hacia la derecha
	i = x + 1;
	j = y - 1;
	while (i < m_width && j >= 0 && getCell(i, j) != nullptr && getCell(i, j)->getType() == tipo) // mientras que a lo largo de la diagonal, el tipo sea igual, no sea vacio y no se salga de la matriz ir sumando iteraciones de iguales
	{
		diagonal_arriba++;
		i++;
		j--;
	}
	i = x - 1;
	j = y + 1;
	while (i >= 0 && j < m_height && getCell(i, j) != nullptr && getCell(i, j)->getType() == tipo) // mientras que a lo largo de la diagonal, el tipo sea igual, no sea vacio y no se salga de la matriz ir sumando iteraciones de iguales
	{
		diagonal_arriba++;
		i--;
		j++;
	}
	if (diagonal_arriba >= SHORTEST_EXPLOSION_LINE)
	{
		return true;
	}
	return false;
}

vector<Candy*> Board::explodeAndDrop()
{
	// Implement your code here
	vector<Candy*> explodedCandies;
	vector<vector<bool>> toExplode(m_height, vector<bool>(m_width, false));
	// Marcamos las casillas que han de explotar
	bool haExplotat;
	do//añadido para que repita el proceso de xplosion cada vez que sea necesario
	{
		haExplotat = false;
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				toExplode[y][x] = shouldExplode(x, y);

				if (toExplode[y][x])
				{
					haExplotat = true;
				}
			}
		}
		if (!haExplotat)
		{
			return explodedCandies;
		}
		// Eliminamos los caramelos explotados del tablero
		for (int y = 0; y < m_height; y++)
		{
			for (int x = 0; x < m_width; x++)
			{
				if (toExplode[y][x])//hayq ue arreglar esta parte, ya que sino no se borran los caramelos del tablero
				{
					Candy* explodedCandy = getCell(x, y);
					explodedCandies.push_back(explodedCandy);
					setCell(nullptr, x, y);//parte añadida para eliminar el caramelo explotado del tablero
				}
			}
		}
		// Movemos los caramelos a posiciones vacías, recorriendo el tablero de abajo arriba, para marcar los caramelos que tienen que caer
		for (int x = 0; x < m_width; x++)
		{
			int k = m_height - 1;
			for (int y = m_height - 1; y >= 0; y--)
			{
				Candy* currCandy = getCell(x, y);
				if (currCandy != nullptr)
				{
					if (y != k)//añadido para que no se muevan los caramelos que ya estan en la parte mas baja, ya que sino se invierte el orden
					{
					setCell(nullptr, x, y);
					setCell(currCandy, x, k);
					}
					k--;

				}
			}
		}
	} while (true);
	return explodedCandies;
}

bool Board::dump(const string& output_path) const
{
	// Implement your code here
	ofstream file(output_path);
	if (!file.is_open())
	{
		return false;
	}
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			Candy* currCandy = getCell(x, y);
			int candyType = -1; // Codificación del caramelo inexistente
			if (currCandy != nullptr)
			{
				candyType = (int)currCandy->getType();
			}
			file << candyType << endl;
			if (!file.good())
			{
				return false;
			}
		}
	}
	file.close();
	return true;
}

bool Board::load(const string& input_path)
{
	// Implement your code here
	ifstream file(input_path);
	if (!file.is_open())
	{
		return false;
	}
	int candyType = 0;
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			if (!(file >> candyType))
				return false;
			Candy* oldCandy = getCell(x, y);
			if (oldCandy != nullptr)
			{
				delete oldCandy;
			}
			if (candyType == -1)
			{
				setCell(nullptr, x, y);
			}
			else
			{
				Candy* loadedCandy = new Candy((CandyType)candyType);
				setCell(loadedCandy, x, y);
			}
		}
	}
	file.close();
	return true;
}
#include "piece.h"
#include <string>

Piece::Piece(int posX, int posY) :
	m_posX(posX),
	m_posY(posY)
{

}

void Piece::init(int posX, int posY) 
{
    m_posX = posX;
    m_posY = posY;
}

Piece::~Piece() 
{
	// Empty
}

bool Piece::canMoveTo(int targetX, int targetY) 
{
    for (unsigned int i = 0 ; i < m_availableMovements.size() ; i++) 
	{
        if (m_availableMovements[i][0] == targetX && m_availableMovements[i][1] == targetY) 
		{
            return true;
        }
    }

    return false;
}

void Piece::moveTo(int targetX, int targetY) 
{
    m_posX = targetX;
    m_posY = targetY;
}

const std::vector<int> Piece::getPosition() 
{
    std::vector<int> position;
    position.resize(2);
    position[0] = m_posX;
    position[1] = m_posY;
    return position;
}

void Piece::computeAvailableMovements(std::vector<Piece*> own, std::vector<Piece*> opp) 
{
	// Empty
}

std::string Piece::toString() const 
{
    std::string s = "Piece at " ;
    s = std::to_string(m_posX) + " and " + std::to_string(m_posY) ;
    return s;
}

void Piece::clearAvailableMovements() {
    m_availableMovements.clear();
}

void Piece::deleteAvailableMovements(std::vector<int>) 
{
	// Empty
}

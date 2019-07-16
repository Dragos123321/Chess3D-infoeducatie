#include "player.h"
#include <typeinfo>

Player::~Player() 
{
	// Empty
}

void Player::init(int side) 
{
    m_number = side;
}

void Player::init(int side,  std::vector<Piece*> const & pieces) 
{

    m_number = side;
    m_pieces = pieces;

    for(unsigned int i = 0; i < pieces.size(); i++)
	{
        if(pieces[i]->getName() == "King" ) 
		{
            m_king = static_cast<King*>(pieces[i]);
            break;
        }
    }
}

void Player::loose() 
{
	// TODO
}

void Player::win() 
{
	// TODO
}

void Player::computeAvailableMovements(std::vector<Piece*> own, std::vector<Piece*> opp) 
{
    for (unsigned int i = 0; i < m_pieces.size(); i++) 
	{
        m_pieces[i]->clearAvailableMovements();
        m_pieces[i]->computeAvailableMovements(own, opp);
    }
}



Piece * Player::getPieceByVao(int VaoId)
{
    for(unsigned int i = 0; i < m_pieces.size(); i++)
	{
        if(m_pieces[i]->getVaoID() == VaoId)
		{
            return m_pieces[i];
        }
    }

    return nullptr;
}

void Player::deletePieceAt(std::vector<int> position) 
{
    for (int i = 0; i < m_pieces.size(); i++) 
	{
        if (m_pieces[i]->getPosition() == position) 
		{
            m_pieces.erase(m_pieces.begin() + i);
            return;
        }
    }
}

void Player::deletePieces() 
{
    for (int i = 0; i < m_pieces.size(); i++) 
	{
        delete m_pieces[i];
    }
}

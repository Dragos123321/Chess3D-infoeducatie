#include "game.h"
#include <fstream>

void Game::tryMovement(int vaoId1, int vaoId2)
{
    Player &current2 = (m_turn == 2) ? m_player1 : m_player2;
    Piece* piece_player2 = current2.getPieceByVao(vaoId2);
    
    if (piece_player2 != nullptr)
    {
        tryMovement(vaoId1, piece_player2->getPosition()[0], piece_player2->getPosition()[1]);
    }
}

void Game::tryMovement(int vaoId, int caseX, int caseY)
{
    Player &current = (m_turn == 1) ? m_player1 : m_player2;
    Player &opponent = (m_turn == 1) ? m_player2 : m_player1;
    Piece *current_piece = current.getPieceByVao(vaoId);

    if (current_piece != nullptr)
    {
        std::cout << "--> Player " << m_turn << " clicked on cell (" << caseX << "," << caseY << ")" <<  std::endl;

        if (current_piece->canMoveTo(caseX, caseY))
        {
            std::vector<int> tempPos = current_piece->getPosition();
            current_piece->moveTo(caseX, caseY);
            opponent.computeAvailableMovements(opponent.getPieces(), current.getPieces());

            if (check(current, opponent, current.getKing()->getPosition()).size() == 0)
			{
                current_piece->moveTo(tempPos);
                ejectPiece(caseX, caseY);
                m_board.movePieceTo(current_piece->getVaoID(), caseX, caseY);
                changeTurn();
            }
			
			else
			{
                std::cout  << "\tInvalid movement, your king is in check!\n";
                current_piece->moveTo(tempPos);
                opponent.computeAvailableMovements(opponent.getPieces(),current.getPieces());
            }
        }

        else
		{
            std::cout  << "\tInvalid movement\n"; 
		}
    }
    else
        std::cout << "--> Player " << m_turn << " clicked on cell (" << caseX << "," << caseY << ") VAO NULLPOINTER" <<  std::endl;
}

void Game::initClassicGame(Scene* scene) 
{
    // Create a new normal part 
    m_scene = scene;

    std::vector<std::vector<Piece*>> pieces;
    pieces = m_board.initClassic(scene);

    m_player1.deletePieces();
    m_player2.deletePieces();
    m_none.init(0);
    m_player1.init(1, pieces[0]);
    m_player2.init(2, pieces[1]);

    m_turn = 1;
    computeAvailableMovements();
}

void Game::loadFromFile(Scene* scene) 
{
    // Reload a previous part
    std::cout << "Opening a previous game..." << std::endl;
    
    std::vector<std::vector<Piece*>> pieces;
    pieces = m_board.initWithFile(scene, "save.txt");
    
    m_none.init(0);
    m_player1.init(1, pieces[0]);
    m_player2.init(2, pieces[1]);
    
    std::string line;
    std::ifstream myfile;
    myfile.open("save.txt");
    
    myfile >> line;
    
    scene->unselect();
    m_turn = std::stoi(line);
    
    computeAvailableMovements();
}

void Game::saveToFile() 
{
    // Save the current game
    std::cout << "Saving the game..." << std::endl;
    
    std::ofstream myfile;
    myfile.open("save.txt");

    myfile << m_turn << std::endl;
    
    for (Piece* piece : m_player1.getPieces())
        myfile << "1 " << piece->getName() << " " << piece->getPosition()[0] << " " << piece->getPosition()[1] << std::endl;
    
    for (Piece* piece : m_player2.getPieces())
        myfile << "2 " << piece->getName() << " " << piece->getPosition()[0] << " " << piece->getPosition()[1] << std::endl;
    
    myfile.close();
}

std::vector<Piece *> Game::check(Player player, Player opponent, std::vector<int> KingPos) {

    std::vector<Piece* > threateningPieces = std::vector<Piece* >(0);

    for (unsigned int i = 0; i < opponent.getPieces().size(); i++) 
	{
        for (unsigned int j = 0; j < opponent.getPieces()[i]->getAvailableMovements().size(); j++) 
		{
            std::vector<int> checkSquare = opponent.getPieces()[i]->getAvailableMovements()[j];
            if ( checkSquare[0] == KingPos[0] && checkSquare[1] == KingPos[1] ) 
			{
                threateningPieces.push_back(opponent.getPieces()[i]);
                if(threateningPieces.size() >= 2)
				{
                    return threateningPieces;
                }
            }
        }
    }

    return threateningPieces;
}

bool Game::checkMate(Player player, Player opponent, std::vector<Piece*> threateningPieces) 
{
    bool isInCheckMate = true;
    std::vector<int> tempPos = player.getKing()->getPosition();
    std::vector<int> kingPos = tempPos;

    for (unsigned int j = 0; j < player.getKing()->getAvailableMovements().size(); j++) 
	{
        kingPos = player.getKing()->getAvailableMovements()[j];
        player.getKing()->moveTo(kingPos[0], kingPos[1]);
        opponent.computeAvailableMovements(opponent.getPieces(), player.getPieces());
        if(check(player, opponent,kingPos).size() == 0)
		{
            isInCheckMate = false;
            break;
        }
    }

    player.getKing()->moveTo(tempPos[0], tempPos[1]);
    opponent.computeAvailableMovements(opponent.getPieces(), player.getPieces());

    if(!isInCheckMate) return false;

    if(threateningPieces.size() > 1) return true;

    std::vector<int> positionToReach = threateningPieces[0]->getPosition();

    // Looks if we can take the threatening piece directly (effective for knights and pawns)
    for (unsigned int i = 0; i < player.getPieces().size(); i++) 
	{
        for (unsigned int j = 0; j < player.getPieces()[i]->getAvailableMovements().size(); j++) 
		{
            std::vector<int> possibleMovement = player.getPieces()[i]->getAvailableMovements()[j];
            if( (possibleMovement[0] == positionToReach[0]) && (possibleMovement[1] == positionToReach[1]) ) return false;
        }
    }

    kingPos = player.getKing()->getPosition();
    for (unsigned int i = 0; i < player.getPieces().size(); i++) 
	{
        if(player.getPieces()[i]->getName() == "King" ) continue;
        tempPos = player.getPieces()[i]->getPosition();
        for (unsigned int j = 0; j < player.getPieces()[i]->getAvailableMovements().size(); j++)
		{
            std::vector<int> possibleMovement = player.getPieces()[i]->getAvailableMovements()[j];
            player.getPieces()[i]->moveTo(possibleMovement);
            opponent.computeAvailableMovements(opponent.getPieces(), player.getPieces());
            if( check(player, opponent, kingPos).size() == 0 ) 
			{
                player.getPieces()[i]->moveTo(tempPos);
                return false;
            }
        }
        player.getPieces()[i]->moveTo(tempPos);
    }

    std::cout << "\nCHECKMATE !\n";

	m_isFinished = true;

    return true;
}

void Game::changeTurn() 
{
    // Change the current player 
    m_turn = (m_turn == 1) ? 2 : 1;

    computeAvailableMovements();

    m_scene->unselect();

    if(m_turn == 1)
	{
        std::vector<Piece*> checkState = check(m_player1, m_player2, m_player1.getKing()->getPosition());
        if (checkState.size() > 0)
		{
            std::cout << "\nPlayer 1 is in check!" << std::endl;
            m_scene->setSelectTex(1);
            if(checkMate(m_player1, m_player2,checkState)) endGame(2);
        } 
		
		else 
		{
            m_scene->setSelectTex(0);
        }
    } 
	
	else if (m_turn == 2)
	{
        std::vector<Piece *> checkState = check(m_player2, m_player1, m_player2.getKing()->getPosition());
        if (checkState.size())
		{
            std::cout << "\nPlayer 2 is in check!" << std::endl;
            m_scene->setSelectTex(1);
            if(checkMate(m_player2, m_player1, checkState)) endGame(1);
        } 
		
		else 
		{
            m_scene->setSelectTex(0);
        }
    }
}

void Game::computeAvailableMovements() {
    // Calculates the movements available for all pieces 
    m_player1.computeAvailableMovements(m_player1.getPieces(), m_player2.getPieces());
    m_player2.computeAvailableMovements(m_player2.getPieces(), m_player1.getPieces());
}

void Game::testDebug() 
{
    std::vector<Piece*> pieces = m_player1.getPieces();
    std::vector<std::vector<int> > debugMovements = pieces[0]->getAvailableMovements();

    std::cout << std::endl << "AVAILABLE MOVEMENTS" << std::endl;

    for (unsigned int i = 0; i < debugMovements.size(); i++) 
	{
        std::cout << debugMovements[i][0] << " " << debugMovements[i][1] << std::endl;
    }
}

void Game::ejectPiece(int x, int y) 
{
    Piece* piece;
    std::vector<int> position;
    position.resize(2);
    position[0] = x;
    position[1] = y;

    for (unsigned int i = 0; i < m_player1.getPieces().size(); i++) 
	{
        piece = m_player1.getPieces()[i];
        if (piece->getPosition()[0] == position[0] && piece->getPosition()[1] == position[1]) 
		{
            m_scene->ejectVAO(piece->getVaoID() - 1);
            m_player1.deletePieceAt(position);
        }
    }
    for (unsigned int j = 0; j < m_player2.getPieces().size(); j++) 
	{
        piece = m_player2.getPieces()[j];
        if (piece->getPosition()[0] == position[0] && piece->getPosition()[1] == position[1]) 
		{
            m_scene->ejectVAO(piece->getVaoID() - 1);
            m_player2.deletePieceAt(position);
        }
    }
}

void Game::endGame(int winner)
{
    m_scene->setSelectTex(2);

    Player looser;
    if(winner == 1)
	{
        looser = m_player2;
    }
	
	else
	{
        looser = m_player1;
    }

    std::cout << "\nPlayer " << winner << " wins the game! ";

    for (unsigned int i = 0; i < looser.getPieces().size(); i++)
	{
        Piece* piece = looser.getPieces()[i];
        if(piece->getName() == "King") continue;

        m_scene->ejectVAO(piece->getVaoID() - 1);
    }
}

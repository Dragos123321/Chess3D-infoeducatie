#pragma once

/*
	class Game
	brief Class that implements chess rules
*/

#include <iostream>
#include "board.h"
#include "player.h"
#include "scene.h"
#include "piece.h"

class Game
{
public:
    void initClassicGame(Scene* scene);
    void loadFromFile(Scene* scene);
    void saveToFile();

    // Checks if the designated player is in check
    std::vector<Piece*> check(Player player, Player opponent, std::vector<int> KingPos);
    // Checks if the designated player is in checkmate
    bool checkMate(Player player, Player opponent, std::vector<Piece*> threateningPieces);

    void changeTurn();

    void computeAvailableMovements();
    //void computeKingMovements(Player player, Player opponent);

    void tryMovement(int vaoId, int caseX, int caseY);
    void tryMovement(int vaoId1, int vaoId2);

    int getPlayerId() const { return m_turn; }
    void endGame(int winner);
    void testDebug();
    void ejectPiece(int, int);

	bool isFinished() const { return m_isFinished; }

private:
    Scene* m_scene;
    Board m_board;
    Player m_player1, m_player2, m_none;
    int m_turn;

	bool m_isFinished = false;
};

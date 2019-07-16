#pragma once

#include <vector>

#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

class Player
{
public:
    Player() = default;
    virtual ~Player();

    void init(int side);
    void init(int side, const std::vector<Piece*>& pieces);
    void loose();
    void win();

    std::vector<Piece*> getPieces() const { return m_pieces; }
    Piece * getPieceByVao(int VaoId);
    Piece* getKing() const { return m_king; }

    // Calculates the movements available for all player's pieces
    void computeAvailableMovements(std::vector<Piece*> own, std::vector<Piece*> opp);
    void deletePieceAt(std::vector<int> position);
    void deletePieces();


private:
    // Player's number
    int m_number;
    // List of player's pieces
    std::vector<Piece*> m_pieces;
    // a direct reference to the king
    King* m_king;
};

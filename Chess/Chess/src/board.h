#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <map>

#include "scene.h"
#include "piece.h"
#include "pawn.h"
#include "rook.h"
#include "knight.h"
#include "bishop.h"
#include "queen.h"
#include "king.h"

class Board
{
public:
    Board() = default;
    virtual ~Board();

    std::vector<std::vector<Piece*>> initClassic(Scene* scene);
    std::vector<std::vector<Piece*>> initWithFile(Scene* scene, std::string fileName);

    // Returns the position of a cell in space
    inline glm::vec3 getPosAt(int i, int j) { return m_squares[i][j]; }
    inline glm::vec3 getPosAt(std::vector<int> square) { return m_squares[square[0]][square[1]]; }

    // Calculates and stores the position in squares
    void computeAllSquares();

    Piece* getPieceByVao(int vao) { return m_vaoIDsMap[vao]; }

    void movePieceTo(int vao, int i, int j);
    const glm::vec3 getOut() { return m_outOfBound; }

private:
    // Creates the 16 pieces of a player
    std::vector<Piece*> initPieceFromFile(int side);
    std::vector<Piece*> initPiece(int side);

    Scene* m_scene;
    std::map<int, Piece *> m_vaoIDsMap;

    // Returns the actual position of a box on the board
    glm::vec3 computeRealPosition(int i, int j);

    // Maps a set of 2 integers in real positions in space
    std::vector<std::vector<glm::vec3>> m_squares;

    // List of all parts
    const glm::vec3 m_centerToSquare0 = glm::vec3(-265.0f, 0.0f, -267.0f);
    const glm::vec3 m_outOfBound = glm::vec3(2000.0f, 0.0f, 2000.0f);
    const float m_squareOffset = 76.0f;
};

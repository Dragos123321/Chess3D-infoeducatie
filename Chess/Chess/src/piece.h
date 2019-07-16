#pragma once

#include <iostream>
#include <vector>
#include <string>

class Piece
{
public:
    Piece() = default;
	Piece(int posX, int posY);

    virtual void init(int posX, int posY);
    virtual ~Piece();
    virtual int getVaoID() const {return m_vaoID;}
    virtual void setVaoID(int vaoID) { m_vaoID = vaoID; }

    // Returns true if the piece can go to the box
    bool canMoveTo(int targetX, int targetY);

    // Move the piece
    void moveTo(int targetX, int targetY);
    void moveTo(std::vector<int> target) { moveTo(target[0], target[1]); }
    virtual std::string getModelPath() const { return ""; }
    const std::vector<std::vector<int>>& getAvailableMovements() const { return m_availableMovements; }
    virtual std::vector<int> const getPosition();

    // Calculates all the movements of a piece
    virtual void computeAvailableMovements(std::vector<Piece*>, std::vector<Piece*>);
    virtual void deleteAvailableMovements(std::vector<int>);
    void clearAvailableMovements();
    virtual std::string toString() const;
    virtual std::string getName() const = 0;

protected:
    int m_posX;
    int m_posY;
    int m_vaoID;
    std::vector<std::vector<int>> m_availableMovements;
};


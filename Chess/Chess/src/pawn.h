#pragma once

#include "piece.h"

class Pawn : public Piece
{
public:
    Pawn(int side);
    virtual void computeAvailableMovements(std::vector<Piece*> own, std::vector<Piece*> opp) override;
    virtual std::string getModelPath() const override { return "models/pion.obj"; }
    virtual std::string getName() const override { return "Pawn"; }
private:
    int d;
};

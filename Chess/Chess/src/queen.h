#pragma once

#include "piece.h"

class Queen : public Piece
{
public:
    virtual void computeAvailableMovements(std::vector<Piece*> own, std::vector<Piece*> opp) override;
    virtual std::string getModelPath() const override { return "models/dame.obj"; }
    virtual std::string getName() const override { return "Queen"; }
};

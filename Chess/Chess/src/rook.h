#pragma once

#include "piece.h"

class Rook : public Piece
{
public:
    virtual void computeAvailableMovements(std::vector<Piece*> own, std::vector<Piece*> opp) override;
	virtual std::string getModelPath() const override { return "models/tour.obj"; }
	virtual std::string getName() const override { return "Rook"; }
};

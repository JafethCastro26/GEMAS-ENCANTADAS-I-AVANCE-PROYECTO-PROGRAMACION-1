#pragma once
#include "Gem.h"

class iceGem : public Gem {
private:
    int iceStrength = 2; 

public:
    iceGem();
    ~iceGem();
    bool isIce() override;
    void doMoveEffect(float frametime) override;
    void setIceStrength(int pIceStrength) override;
    int getType();
    void setType();
    void weakenIce() override; 
    bool isBroken() override;
    int getIceStrength();
};



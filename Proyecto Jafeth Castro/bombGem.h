#include "gem.h"
#include <iostream>
using namespace std;
#pragma once

class bombGem : public Gem
{
public:
	bombGem();
	int getType() override;
	void setType() override;
	bool isBomb() override;

};


#pragma once
#include "gem.h"
using namespace std;
using namespace sf;

class Board{
private:
	Gem* board[8][8];
	Gem* selectedGem1;
	Gem* selectedGem2;
	int selectedRow1, selectedRow2;
	int selectedColumn1, selectedColumn2;
	int movesLeft;
	int actualScore;
	int goalScore;
	bool pendingMatchProcess;
	bool lastSwapWasValid;

public:
	Board();
	~Board();
	int getMovesLeft();
	int getActualScore();
	int getGoalScore();
	bool getPendingMatchProcess();
	bool getLastSwapWasValid();
	void setPendingMatchProcess(bool pPendingMatchProcess);
	void setLastSwapWasValid(bool pLastSwapWasValid);
	void drawGems(RenderWindow& window);
	bool checkVerticalMatches(bool toRemove[8][8]);
	bool checkHorizontalMatches(bool toRemove[8][8]);
	void dropGemsAndFill(bool toRemove[8][8]);
	bool areGemsAdjacent(int row1,int col1,int row2,int col2);
	void updateGemsPosition();
	bool processMatchesAndScore();
	void detectCicksOverGems(Vector2f mousePos);
	void swapSelectedGems();
	void revertLastSwap();
	void clearSelection();
	void decrementMovesLeft();
	bool areTwoSelected();
	void moveGems(float frameTime);
	bool allGemsAtTarget();


};




#include "Board.h"
#include <iostream>
#include "Gem.h"
using namespace std;
using namespace sf;
Board::Board() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			float x = j * 64;
			float y = i * 64;
			board[i][j] = new Gem();
			board[i][j]->generateGem();
			board[i][j]->setGemPosition({x, y});
		}
	}
	selectedGem1 = nullptr;
	selectedGem2 = nullptr;
	selectedRow1 = 0;
	selectedRow2 = 0;
	selectedColumn1 = 0;
	selectedColumn2 = 0;
	movesLeft = 20;
	goalScore = 1500;
	pendingMatchProcess = false;
	lastSwapWasValid = false;

	while (processMatchesAndScore()) {};
	actualScore = 0;
}


Board::~Board() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			delete board[i][j];
		}
	}
}

int Board::getMovesLeft()
{
	return movesLeft;
}

int Board::getActualScore()
{
	return actualScore;
}

int Board::getGoalScore()
{
	return goalScore;
}

bool Board::getPendingMatchProcess()
{
	return pendingMatchProcess;
}

bool Board::getLastSwapWasValid()
{
	return lastSwapWasValid;
}

void Board::setPendingMatchProcess(bool pPendingMatchProcess)
{
	pendingMatchProcess = pPendingMatchProcess;
}

void Board::setLastSwapWasValid(bool pLastSwapWasValid)
{
	lastSwapWasValid = pLastSwapWasValid;
}

void Board::drawGems(RenderWindow& window) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j]->drawGem(window); 
		}
	}
}

bool Board::checkVerticalMatches(bool toRemove[8][8]) {
	int matchStart, matchType, matchLength;
	bool foundMatch = false;
	for (int col = 0; col < 8; col++) {
		matchStart = 0;
		while (matchStart < 8) {
			matchType = board[matchStart][col]->getType();
			matchLength = 1;
			for (int row = matchStart + 1; row < 8; row++) {
				if (board[row][col]->getType() == matchType) {
					matchLength++;
				}
				else {
					break;
				}
			}
			if (matchLength >= 3) {
				foundMatch = true;
				for (int matchIndex = matchStart; matchIndex < matchStart + matchLength; matchIndex++) {
					toRemove[matchIndex][col] = true;
				}
			}
			matchStart += matchLength;
		}
	}
	return foundMatch;
}


bool Board::checkHorizontalMatches(bool toRemove[8][8]) {
	int matchStart, matchType, matchLength;
	bool foundMatch = false;
	for (int row = 0; row < 8; row++) {
		matchStart = 0;
		while (matchStart < 8) {
			matchType = board[row][matchStart]->getType();
			matchLength = 1;
			for (int col = matchStart + 1; col < 8; col++) {
				if (board[row][col]->getType() == matchType) {
					matchLength++;
				}
				else {
					break;
				}
			}
			if (matchLength >= 3) {
				foundMatch = true;
				for (int matchIndex = matchStart; matchIndex < matchStart + matchLength; matchIndex++) {
					toRemove[row][matchIndex] = true;
				}
			}
			matchStart += matchLength;
		}
	}
	return foundMatch;
}


void Board::dropGemsAndFill(bool toRemove[8][8]) {
	int writeRow, newType;
	for (int col = 0; col < 8; col++) {
		writeRow = 7;
		for (int row = 7; row >= 0; row--) {
			if (!toRemove[row][col]) {
				if (writeRow != row) {
					newType = board[row][col]->getType();
					board[writeRow][col]->setType(newType);
					board[writeRow][col]->setTexture();
					board[writeRow][col]->setGemPosition(Vector2f(col * 64, row * 64));
					board[writeRow][col]->setTargetPosition(Vector2f(col * 64, writeRow * 64));
				}
				writeRow--;
			}
		}
		
		for (int row = writeRow; row >= 0; row--) {
			board[row][col]->generateGem();
			board[row][col]->setGemPosition(Vector2f(col * 64, -64));
			board[row][col]->setTargetPosition(Vector2f(col * 64, row * 64));
		}
	}
}

void Board::updateGemsPosition() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			board[i][j]->setTargetPosition(Vector2f(j * 64, i * 64));
		}
	}
}

bool Board::processMatchesAndScore() {
	bool toRemove[8][8] = { {false} };
	bool foundVertical = checkVerticalMatches(toRemove);
	bool foundHorizontal = checkHorizontalMatches(toRemove);
	bool foundMatch = foundVertical || foundHorizontal;
	int matchCount = 0;
	if (foundMatch) {
		for (int i = 0; i < 8; i++) {
			for (int j = 0; j < 8; j++) {
				if (toRemove[i][j]) {
					matchCount++;
				}
			}
		}
		actualScore += matchCount * 10;
		dropGemsAndFill(toRemove);
		updateGemsPosition();
	}
	return foundMatch;
}

bool Board::areGemsAdjacent(int row1,int col1,int row2,int col2) {
	int rowsDiff = abs(row1 - row2);
	int colsDiff = abs(col1 - col2);
	return (rowsDiff == 1 && colsDiff == 0) || (rowsDiff == 0 && colsDiff == 1);
}

void Board::swapSelectedGems() {
	Gem* gemA = board[selectedRow1][selectedColumn1];
	Gem* gemB = board[selectedRow2][selectedColumn2];
	if (!selectedGem1 || !selectedGem2)  return;
	if (!areGemsAdjacent(selectedRow1, selectedColumn1, selectedRow2, selectedColumn2)) {
		cout << "¡Las gemas que se intercambian deben ser adyacentes!" << endl;
		selectedGem1 = nullptr;
		selectedGem2 = nullptr;
		return;
	}

		swap(board[selectedRow1][selectedColumn1], board[selectedRow2][selectedColumn2]);
		board[selectedRow1][selectedColumn1]->setTargetPosition(Vector2f(selectedColumn1 * 64, selectedRow1 * 64));
		board[selectedRow2][selectedColumn2]->setTargetPosition(Vector2f(selectedColumn2 * 64, selectedRow2 * 64));
		pendingMatchProcess = true;
		lastSwapWasValid = false;

}

void Board::revertLastSwap() {
	cout << "Movimiento invalido,las gemas deben provocar un match!" << endl;
	swap(board[selectedRow1][selectedColumn1], board[selectedRow2][selectedColumn2]);
	board[selectedRow1][selectedColumn1]->setTargetPosition(Vector2f(selectedColumn1 * 64, selectedRow1 * 64));
	board[selectedRow2][selectedColumn2]->setTargetPosition(Vector2f(selectedColumn2 * 64, selectedRow2 * 64));
}

void Board::clearSelection()
{
	selectedGem1 = nullptr;
	selectedGem2 = nullptr;
}

void Board::decrementMovesLeft()
{
	movesLeft--;
}

bool Board::areTwoSelected()
{
	return selectedGem1 != nullptr && selectedGem2 != nullptr;
}



void Board::detectCicksOverGems(Vector2f mousePos) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j]->getGlobalBounds().contains(static_cast<Vector2f>(mousePos))) {
				if (!selectedGem1) {
					selectedGem1 = board[i][j];
					selectedRow1 = i;
					selectedColumn1 = j;
					
				}
				else if (!selectedGem2 && board[i][j] != selectedGem1) {
					selectedGem2 = board[i][j];
					selectedRow2 = i;
					selectedColumn2 = j;
					
				}
				return;
			}
		}
	}

}

void Board::moveGems(float frameTime) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			board[i][j]->doMoveEffect(frameTime);
		}
	}
} 

bool Board::allGemsAtTarget() {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (!board[i][j]->isAtTarget()) {
				return false;
			}
		}
	}
	return true;
}




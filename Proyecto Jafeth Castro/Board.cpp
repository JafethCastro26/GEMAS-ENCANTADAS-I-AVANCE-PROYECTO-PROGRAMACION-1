#include "Board.h"
#include <iostream>
#include "iceGem.h"
#include "bombGem.h"
#include "normalGem.h"
using namespace std;
using namespace sf;
Board::Board(int level) {
	this->level = level;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			float x = j * 64;
			float y = i * 64;
			board[i][j] = new normalGem();
			board[i][j]->generateGem();
			board[i][j]->setGemPosition({ x, y });
		}
	}
	selectedGem1 = nullptr;
	selectedGem2 = nullptr;
	selectedRow1 = 0;
	selectedRow2 = 0;
	selectedColumn1 = 0;
	selectedColumn2 = 0;
	movesLeft = 20;
	goalScore = 1000;
	pendingMatchProcess = false;
	lastSwapWasValid = false;
	supressBombGeneration = true;

	while (processMatchesAndScore()) {};

	while (countIceGems() == 0 && level>1) {
			for (int i = 0; i < 8; i++) {
				for (int j = 0; j < 8; j++) {
					if (rand() % 100 < 4) {
						delete board[i][j];
						board[i][j] = new iceGem();
						board[i][j]->setType(5);
						board[i][j]->setGemPosition(Vector2f(j * 64, i * 64));
						board[i][j]->setTexture();
					}
				}
			}
	}
	actualScore = 0;
	supressBombGeneration = false;
	if (level == 3) goalScore = 2000;
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

int Board::getBombsActivationCounter()
{
	return bombsActivationCounter;
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
			if (board[i][j]) {
				board[i][j]->drawGem(window);
			}
		}
	}
}

bool Board::checkVerticalMatches(bool toRemove[8][8]) {
	int matchStart, matchType, matchLength;
	bool foundMatch = false;
	for (int col = 0; col < 8; col++) {
		matchStart = 0;
		while (matchStart < 8) {
			if (!board[matchStart][col]) { 
				matchStart++;
				continue;
			}
			matchType = board[matchStart][col]->getType();
			matchLength = 1;
			for (int row = matchStart + 1; row < 8; row++) {
				if ( board[row][col] && board[row][col]->getType() == matchType) {
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
			if (!board[row][matchStart]) {
				matchStart++;
				continue;
			}
			matchType = board[row][matchStart]->getType();
			matchLength = 1;
			for (int col = matchStart + 1; col < 8; col++) {
				if (board[row][col] && board[row][col]->getType() == matchType) {
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

void Board::generateHorizontalBombs(bool toRemove[8][8]) {
	int matchStartRow = 0;
	int matchStartCol = 0;
	int matchLength = 0;
	int bombRow = 0;
	int bombCol = 0;
	int col = 0;
	for (int row = 0; row < 8; ++row) {
	     col = 0;
		while (col < 8) {
			if (!toRemove[row][col]) {
				++col;
				continue;
			}

			matchStartCol = col;
			matchLength = 1;

			// Contar cuántas gemas seguidas están marcadas en esta fila
			for (int currentCol = matchStartCol + 1; currentCol < 8 && toRemove[row][currentCol]; ++currentCol) {
				++matchLength;
			}

			// Crear bomba si hay 4 o más seguidas
			if (matchLength >= 4) {
				bombCol = matchStartCol + matchLength / 2;

				if (board[row][bombCol] && !board[row][bombCol]->isIce() && !board[row][bombCol]->isBomb())
				{
					delete board[row][bombCol];
					board[row][bombCol] = new bombGem();
					board[row][bombCol]->setGemPosition(Vector2f(bombCol * 64, row * 64));
					board[row][bombCol]->setTexture();
					toRemove[row][bombCol] = false;
				}
			}

			// Saltar al final del grupo
			col = matchStartCol + matchLength;
		}
	}

}

void Board::generateVerticalBombs(bool toRemove[8][8])
{
	int matchStartRow = 0;
	int matchStartCol = 0;
	int matchLength = 0;
	int bombRow = 0;
	int bombCol = 0;
	int row = 0;

	for (int col = 0; col < 8; ++col) {
		 row = 0;
		while (row < 8) {
			if (!toRemove[row][col]) {
				++row;
				continue;
			}

			matchStartRow = row;
			matchLength = 1;

			// Contar cuántas gemas seguidas están marcadas en esta columna
			for (int currentRow = matchStartRow + 1; currentRow < 8 && toRemove[currentRow][col]; ++currentRow) {
				++matchLength;
			}

			// Crear bomba si hay 4 o más seguidas
			if (matchLength >= 4) {
				bombRow = matchStartRow + matchLength / 2;

				if (board[bombRow][col] && !board[bombRow][col]->isIce() && !board[row][bombCol]->isBomb())
				{
					delete board[bombRow][col];
					board[bombRow][col] = new bombGem();
					board[bombRow][col]->setGemPosition(Vector2f(col * 64, bombRow * 64));
					board[bombRow][col]->setTexture();
					toRemove[bombRow][col] = false;
				}
			}

			// Saltar al final del grupo
			row = matchStartRow + matchLength;
		}
	}


}

void Board::triggerBombsOnMove(int bombRow, int bombCol) {
	bool toRemove[8][8] = { false };
	for (int i = 0; i < 8; ++i) {
		toRemove[bombRow][i] = true;
		toRemove[i][bombCol] = true;
	}
	int removedCount = 0;
	for (int r = 0; r < 8; ++r) {
		for (int c = 0; c < 8; ++c) {
			if (toRemove[r][c]) ++removedCount;
		}
	}
	actualScore += removedCount * 10;
	bombsActivationCounter++;
	movesLeft--;

	toRemove[bombRow][bombCol] = true;
	supressBombGeneration = true;
	if (board[bombRow][bombCol]) {
		delete board[bombRow][bombCol];
		board[bombRow][bombCol] = nullptr;
	}
	dropGemsAndFill(toRemove);
	while (processMatchesAndScore()) {};
	supressBombGeneration = false;
	updateGemsPosition();
	clearSelection();
	pendingMatchProcess = false;
}


void Board::dropGemsAndFill(bool toRemove[8][8]) {
	if (level == 3 && !supressBombGeneration) {
		generateHorizontalBombs(toRemove);
		generateVerticalBombs(toRemove);

	}

	for (int col = 0; col < 8; col++){
		for (int row = 0; row < 8; row++) {
			if (toRemove[row][col]) {
				delete board[row][col];
				board[row][col] = nullptr;
			}
		}

	
		int iceRow = -1;
		for (int row = 7; row >= 0; row--) {
			if (board[row][col] && board[row][col]->isIce()) {
				iceRow = row;
				break;
			}
		}

		// Gravedad arriba del hielo
		int writeRow = (iceRow == -1) ? 7 : iceRow - 1;
		int foundRowAbove;
		for (int row = writeRow; row >= 0; row--) {
			if (!board[row][col]) {
				 foundRowAbove = row - 1;
				while (foundRowAbove >= 0 && (!board[foundRowAbove][col] || board[foundRowAbove][col]->isIce())) {
					foundRowAbove--;
				}
				if (foundRowAbove >= 0) {
					board[row][col] = board[foundRowAbove][col];
					board[foundRowAbove][col] = nullptr;
					board[row][col]->setTargetPosition(Vector2f(col * 64, row * 64));
				}
				else {
					board[row][col] = new normalGem();
					board[row][col]->generateGem();
					board[row][col]->setGemPosition(Vector2f(col * 64, -64));
					board[row][col]->setTargetPosition(Vector2f(col * 64, row * 64));
				}
			}
		}

		// Gravedad debajo del hielo
		if (iceRow != -1) {
			int writeRow = 7, foundRowBelow;
			for (int row = 7; row > iceRow; row--) {
				if (board[row][col] && board[row][col]->isIce()) continue; 
				if (!board[row][col]) {
					foundRowBelow = row - 1;
					while (foundRowBelow > iceRow && (!board[foundRowBelow][col] || board[foundRowBelow][col]->isIce())) {
						foundRowBelow--;
					}
					if (foundRowBelow > iceRow) {
						board[row][col] = board[foundRowBelow][col];
						board[foundRowBelow][col] = nullptr;
						board[row][col]->setTargetPosition(Vector2f(col * 64, row * 64));
					}
				}
			}
		}
	}
}

void Board::updateGemsPosition() {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j]) {
				board[i][j]->setTargetPosition(Vector2f(j * 64, i * 64));
			}
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
		checkAdjacentIce(toRemove);
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

	if (!board[selectedRow1][selectedColumn1] || !board[selectedRow2][selectedColumn2]) {
		selectedGem1 = nullptr;
		selectedGem2 = nullptr;
		return;
	}
	if (gemA->isIce() || gemB->isIce()) {
		cout << "¡No puedes mover una gema de hielo!" << endl;
		selectedGem1 = nullptr;
		selectedGem2 = nullptr;
		return;
	}
	

		swap(board[selectedRow1][selectedColumn1], board[selectedRow2][selectedColumn2]);
		board[selectedRow1][selectedColumn1]->setTargetPosition(Vector2f(selectedColumn1 * 64, selectedRow1 * 64));
		board[selectedRow2][selectedColumn2]->setTargetPosition(Vector2f(selectedColumn2 * 64, selectedRow2 * 64));
		pendingMatchProcess = true;
		lastSwapWasValid = false;

		if (level == 3) {
			bool bombExploded = false;
			if (board[selectedRow1][selectedColumn1]->isBomb()) {
				triggerBombsOnMove(selectedRow1, selectedColumn1);
				bombExploded = true;
			}
			if (board[selectedRow2][selectedColumn2]->isBomb()) {
				triggerBombsOnMove(selectedRow2, selectedColumn2);
				bombExploded = true;
			}
			if (bombExploded) return;
		}
		

}

void Board::revertLastSwap() {
	cout << "Movimiento invalido,las gemas deben provocar un match!" << endl;
	swap(board[selectedRow1][selectedColumn1], board[selectedRow2][selectedColumn2]);
	if (board[selectedRow1][selectedColumn1])
		board[selectedRow1][selectedColumn1]->setTargetPosition(Vector2f(selectedColumn1 * 64, selectedRow1 * 64));
	if (board[selectedRow2][selectedColumn2])
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
			if (board[i][j] && board[i][j]->getGlobalBounds().contains(static_cast<Vector2f>(mousePos))) {
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
			if (board[i][j]) {
				board[i][j]->doMoveEffect(frameTime);
			}
		}
	}
} 

bool Board::allGemsAtTarget() {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j] && !board[i][j]->isAtTarget()) {
				return false;
			}
		}
	}
	return true;
}
void Board::checkAdjacentIce(bool toRemove[8][8]) {
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (toRemove[i][j]) {
				// Arriba
				if (i > 0 && board[i - 1][j] && board[i - 1][j]->isIce()) {
					board[i - 1][j]->weakenIce();
					if (board[i - 1][j]->isBroken()) {
						delete board[i - 1][j];
						board[i - 1][j] = nullptr;
					}
				}
				// Abajo
				if (i < 7 && board[i + 1][j] && board[i + 1][j]->isIce()) {
					board[i + 1][j]->weakenIce();
					if (board[i + 1][j]->isBroken()) {
						delete board[i + 1][j];
						board[i + 1][j] = nullptr;
					}
				}
				// Izquierda
				if (j > 0 && board[i][j - 1] && board[i][j - 1]->isIce()) {
					board[i][j - 1]->weakenIce();
					if (board[i][j - 1]->isBroken()) {
						delete board[i][j - 1];
						board[i][j - 1] = nullptr;
					}
				}
				// Derecha
				if (j < 7 && board[i][j + 1] && board[i][j + 1]->isIce()) {
					board[i][j + 1]->weakenIce();
					if (board[i][j + 1]->isBroken()) {
						delete board[i][j + 1];
						board[i][j + 1] = nullptr;
					}
				}
			}
		}
	}
}

int Board::countIceGems()
{
	int iceCounter = 0;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (board[i][j] && board[i][j]->isIce()) {
				iceCounter++;
			}
		}
	}
	return iceCounter;
}


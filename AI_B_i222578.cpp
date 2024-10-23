#include <iostream>
#include <cstdlib>
#include <ctime>
#include <cmath>
using namespace std;

class Cell {
public:
    int value;
    Cell* rightNeighbor;
    Cell* belowNeighbor;

    Cell(int inputVal) {
        value = inputVal;
        rightNeighbor = nullptr;
        belowNeighbor = nullptr;
    }
};

struct Position {
    int row;
    int col;
    Position(int r = 0, int c = 0) : row(r), col(c) {}
};

Cell* createGrid(int totalRows, int totalCols) {
    Cell* gridStart = nullptr;
    Cell* rowHead = nullptr;
    Cell* prevRowHead = nullptr;

    for (int r = 0; r < totalRows; ++r) {
        Cell* leftNeighbor = nullptr;
        for (int c = 0; c < totalCols; ++c) {
            Cell* newCell = new Cell(0);
            if (c == 0) rowHead = newCell;
            if (leftNeighbor) leftNeighbor->rightNeighbor = newCell;
            if (prevRowHead) {
                prevRowHead->belowNeighbor = newCell;
                prevRowHead = prevRowHead->rightNeighbor;
            }
            leftNeighbor = newCell;
        }
        prevRowHead = rowHead;
        if (r == 0) gridStart = rowHead;
    }
    return gridStart;
}

void showGrid(Cell* gridStart, int totalRows, int totalCols, const Position& player, bool hasKey, const Position* coinLocs, const bool* pickedCoins, int totalCoins) {
    Cell* row = gridStart;
    int rowNum = 0;
    while (row) {
        Cell* col = row;
        int colNum = 0;
        while (col) {
            if (rowNum == player.row && colNum == player.col) {
                cout << "P   ";
            }
            else if (rowNum == 0 || rowNum == totalRows - 1 || colNum == 0 || colNum == totalCols - 1) {
                cout << "#   ";
            }
            else {
                bool coinVisible = false;
                for (int i = 0; i < totalCoins; i++) {
                    if (!pickedCoins[i] && coinLocs[i].row == rowNum && coinLocs[i].col == colNum) {
                        coinVisible = true;
                        break;
                    }
                }
                cout << (coinVisible ? "C   " : ".   ");
            }
            col = col->rightNeighbor;
            colNum++;
        }
        cout << endl;
        row = row->belowNeighbor;
        rowNum++;
    }
}

Position randomCoord(int maxRows, int maxCols) {
    int x = rand() % (maxRows - 2) + 1;
    int y = rand() % (maxCols - 2) + 1;
    return Position(x, y);
}

void distributeCoins(Position coinLocs[], int totalCoins, int maxRows, int maxCols) {
    for (int i = 0; i < totalCoins; ++i) {
        coinLocs[i] = randomCoord(maxRows, maxCols);
    }
}

bool checkCoin(const Position& player, const Position coinLocs[], bool pickedCoins[], int totalCoins) {
    for (int i = 0; i < totalCoins; ++i) {
        if (!pickedCoins[i] && player.row == coinLocs[i].row && player.col == coinLocs[i].col) {
            pickedCoins[i] = true;
            return true;
        }
    }
    return false;
}

int calcDistance(const Position& a, const Position& b) {
    return abs(a.row - b.row) + abs(a.col - b.col);
}

void provideHint(const Position& player, const Position& key, const Position& door, int prevKeyDist, int prevDoorDist) {
    int currKeyDist = calcDistance(player, key);
    int currDoorDist = calcDistance(player, door);

    if (currKeyDist < prevKeyDist) {
        cout << "Hint: Getting closer to the key.\n";
    }
    else if (currKeyDist > prevKeyDist) {
        cout << "Hint: Further away from the key.\n";
    }

    if (currDoorDist < prevDoorDist) {
        cout << "Hint: Getting closer to the door.\n";
    }
    else if (currDoorDist > prevDoorDist) {
        cout << "Hint: Further away from the door.\n";
    }
}

void placeBombs(Position bombLocs[], int totalBombs, int maxRows, int maxCols) {
    for (int i = 0; i < totalBombs; ++i) {
        bombLocs[i] = randomCoord(maxRows, maxCols);
    }
}

bool checkBomb(const Position& player, const Position bombLocs[], int totalBombs) {
    for (int i = 0; i < totalBombs; ++i) {
        if (player.row == bombLocs[i].row && player.col == bombLocs[i].col) {
            return true;
        }
    }
    return false;
}

int main() {
    srand(static_cast<unsigned>(time(0)));

    int totalRows, totalCols, remainingMoves, availableUndos;
    string gameLevel;

    cout << "Choose difficulty level (Easy, Medium, Hard): ";
    cin >> gameLevel;

    switch (tolower(gameLevel[0])) {
    case 'e':
        totalRows = totalCols = 10;
        remainingMoves = 24;
        availableUndos = 6;
        break;
    case 'm':
        totalRows = totalCols = 15;
        remainingMoves = 20;
        availableUndos = 4;
        break;
    case 'h':
        totalRows = totalCols = 20;
        remainingMoves = 18;
        availableUndos = 1;
        break;
    default:
        cout << "Invalid input. Defaulting to Easy level.\n";
        totalRows = totalCols = 10;
        remainingMoves = 24;
        availableUndos = 6;
    }

    Cell* grid = createGrid(totalRows, totalCols);

    Position playerPos(1, 1);
    Position keyLoc = randomCoord(totalRows, totalCols);
    Position doorLoc = randomCoord(totalRows, totalCols);
    const int numCoins = 5;
    Position coinLocs[numCoins];
    bool pickedCoins[numCoins] = { false };

    distributeCoins(coinLocs, numCoins, totalRows, totalCols);
    Position bombLocs[8];
    int totalBombs;

    if (gameLevel == "Easy") {
        totalBombs = 4;
    }
    else if (gameLevel == "Medium") {
        totalBombs = 6;
    }
    else {
        totalBombs = 8;
    }

    placeBombs(bombLocs, totalBombs, totalRows, totalCols);

    bool hasKey = false;
    int gameScore = 0;
    int coinsCollected = 0;
    char command;

    int prevKeyDist = calcDistance(playerPos, keyLoc);
    int prevDoorDist = calcDistance(playerPos, doorLoc);
    Position previousPositions[10];
    int undoIndex = 0;

    while (true) {
        if (remainingMoves <= 0) {
            cout << "Game Over: You're out of moves!\n";
            break;
        }

        cout << "Moves left: " << remainingMoves << "\tUndos left: " << availableUndos
            << "\tScore: " << gameScore << "\nCoins collected: " << coinsCollected << "\n";
        cout << "Your position: (" << playerPos.row << ", " << playerPos.col << ")\n";

        showGrid(grid, totalRows, totalCols, playerPos, hasKey, coinLocs, pickedCoins, numCoins);

        cout << "Enter command (W/A/S/D to move, U to undo): ";
        cin >> command;
        command = tolower(command);

        Position previousPosition = playerPos;

        switch (command) {
        case 'w':
            if (playerPos.row > 1) playerPos.row--;
            break;
        case 's':
            if (playerPos.row < totalRows - 2) playerPos.row++;
            break;
        case 'a':
            if (playerPos.col > 1) playerPos.col--;
            break;
        case 'd':
            if (playerPos.col < totalCols - 2) playerPos.col++;
            break;
        case 'u':
            if (availableUndos > 0 && undoIndex > 0) {
                playerPos = previousPositions[--undoIndex];
                availableUndos--;
                continue;
            }
            else {
                cout << "No undo moves left!\n";
                continue;
            }
            break;
        default:
            cout << "Invalid command! Use W/A/S/D to move, U to undo.\n";
            continue;
        }

        if (undoIndex < 10) {
            previousPositions[undoIndex++] = previousPosition;
        }
        else {
            for (int i = 1; i < 10; ++i) {
                previousPositions[i - 1] = previousPositions[i];
            }
            previousPositions[9] = previousPosition;
        }

        if (checkCoin(playerPos, coinLocs, pickedCoins, numCoins)) {
            coinsCollected++;
            gameScore += 10;
        }

        if (checkBomb(playerPos, bombLocs, totalBombs)) {
            cout << "Game Over: You hit a bomb!\n";
            break;
        }

        if (playerPos.row == keyLoc.row && playerPos.col == keyLoc.col) {
            hasKey = true;
            cout << "You found the key!\n";
        }

        if (playerPos.row == doorLoc.row && playerPos.col == doorLoc.col) {
            if (hasKey) {
                cout << "Congratulations! You escaped with the key!\n";
                break;
            }
            else {
                cout << "You need to find the key to escape!\n";
            }
        }

        remainingMoves--;
        provideHint(playerPos, keyLoc, doorLoc, prevKeyDist, prevDoorDist);
        prevKeyDist = calcDistance(playerPos, keyLoc);
        prevDoorDist = calcDistance(playerPos, doorLoc);
    }

    return 0;
}

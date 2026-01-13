#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
#include <fstream>
using namespace std;
const size_t MAX = 15;
const size_t MAXIMUM = MAX * MAX;
const size_t MAXBOATS = 10;
const char WATER = '~';
const char HIT = 'x';
const char MISS = 'o';
const char BOAT = '#';
const int DESTROYER_SIZE = 1;
const int SUBMARINE_SIZE = 2;
const int CRUISER_SIZE = 3;
const int CARRIER_SIZE = 4;
const int DESTROYERS_COUNT = 4;
const int SUBMARINES_COUNT = 3;
const int CRUISERS_COUNT = 2;
const int CARRIERS_COUNT = 1;
bool saveG = false;
const char *typeBoat[] = {"Destroyer", "Submarine", "Cruiser", "Carrier"};

char fromNumberToChar(int num)
{
    return num + '0';
}

int fromCharToNumber(char str)
{
    return str - '0';
}

void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void shuffle(int arr[], int n)
{

    for (int i = n - 1; i > 0; i--)
    {
        int j = rand() % (i + 1);
        swap(&arr[i], &arr[j]);
    }
}

int absoluteValue(int a)
{
    if (a > 0)
        return a;
    else
        return -a;
}

void reajustCoordinates(int *X, int *Y)
{
    *X = *X - 1;
    *Y = *Y - 1;
}

void fillWithWater(char board[MAX][MAX], const size_t size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = WATER;
        }
    }
}

bool areValidCoordinates(int startX, int startY, int endX, int endY, int boatSize, const size_t size)
{

    if (startX > 0 && startY > 0 && endY <= size && startY <= size && startY <= endY && startX <= endX)
    {
        if (((startX == endX) && absoluteValue(startY - endY) == boatSize - 1) ||
            ((startY == endY) && absoluteValue(startX - endX) == boatSize - 1))
            return true;
        else
            return false;
    }
    else
    {
        return false;
    }
}

bool areAlreadyHit(char board[MAX][MAX], int X, int Y)
{
    reajustCoordinates(&X, &Y);
    return (board[X][Y] != WATER);
}

bool areOccupied(char board[MAX][MAX], int startX, int startY, int endX, int endY)
{
    bool occupied = false;
    if (startX == endX)
    {
        for (int i = startY - 1; i <= endY - 1; i++)
        {
            if (board[startX - 1][i] != WATER)
            {
                occupied = true;
            }
        }
    }
    else if (startY == endY)
    {
        for (int j = startX - 1; j <= endX - 1; j++)
        {
            if (board[j][startY - 1] != WATER)
            {
                occupied = true;
            }
        }
    }
    return occupied;
}

void printBoards(char firstBoard[MAX][MAX], char secondBoard[MAX][MAX], const size_t size)
{
    for (int i = 0; i <= size; i++)
    {
        if (i < 9)
            cout << i << "  ";
        else
            cout << i << " ";
    }
    cout << "\n";
    for (int i = 0; i < size; i++)
    {
        if (i + 1 < 10)
            cout << i + 1 << "  ";
        else
            cout << i + 1 << " ";
        for (int j = 0; j < size; j++)
        {
            cout << firstBoard[i][j] << "  ";
        }
        cout << "| ";
        for (int k = 0; k < size; k++)
        {
            cout << secondBoard[i][k] << "  ";
        }
        cout << "\n";
    }
}

void printBoard(char board[MAX][MAX], const size_t size)
{
    for (int i = 0; i <= size; i++)
    {
        if (i < 9)
            cout << i << "  ";
        else
            cout << i << " ";
    }
    cout << "\n";
    for (int i = 0; i < size; i++)
    {
        if (i + 1 < 10)
            cout << i + 1 << "  ";
        else
            cout << i + 1 << " ";
        for (int j = 0; j < size; j++)
        {
            cout << board[i][j] << "  ";
        }
        cout << "\n";
    }
}

void fillInBoatPositions(char board[MAX][MAX], char showBoard[MAX][MAX], const size_t size, const int amount, const int boatSize, int *boatID)
{
    for (int t = 0; t < amount; t++)
    {
        printBoard(showBoard, size);
        bool incorrectPlacement = true;
        int startX, startY, endX, endY;
        while (incorrectPlacement)
        {
            cout << "\n"
                 << typeBoat[boatSize - 1] << " Start Position: ";
            cin >> startX >> startY;
            if (boatSize != 1)
            {
                cout << "\n"
                     << typeBoat[boatSize - 1] << " End Position: ";
                cin >> endX >> endY;
            }
            else
            {
                endX = startX;
                endY = startY;
            }
            if (!areValidCoordinates(startX, startY, endX, endY, boatSize, size))
            {
                cout << "Try again with new coordinates. These are not valid.";
            }
            else
            {
                if (!areOccupied(board, startX, startY, endX, endY))
                {
                    incorrectPlacement = false;
                }
                else
                {
                    cout << "Try again with new coordinates. These are already occupied.";
                }
            }
        }

        {
            reajustCoordinates(&startX, &startY);
            reajustCoordinates(&endX, &endY);
            if (startX == endX)
            {
                for (int j = startY; j <= endY; j++)
                {
                    showBoard[startX][j] = BOAT;
                    board[startX][j] = fromNumberToChar(*boatID);
                }
            }
            else
            {
                for (int i = startX; i <= endX; i++)
                {
                    showBoard[i][startY] = BOAT;
                    board[i][startY] = fromNumberToChar(*boatID);
                }
            }
        }
        (*boatID)++;
    }
}

void fillInBoatPositionsAuto(char board[MAX][MAX], char showBoard[MAX][MAX], const size_t size, const int amount, const int boatSize, int *boatID, bool computer)
{
    for (int t = 0; t < amount; t++)
    {
        bool incorrectPlacement = true;
        int startX, startY, endX, endY;
        while (incorrectPlacement)
        {
            int orientation = rand() % 2;
            if (orientation == 0)
            {
                startX = (rand() % size) + 1;
                endX = startX;
                startY = (rand() % (size - boatSize + 1)) + 1;
                endY = startY + boatSize - 1;
            }
            else
            {
                startX = (rand() % (size - boatSize + 1)) + 1;
                endX = startX + boatSize - 1;
                startY = (rand() % size) + 1;
                endY = startY;
            }

            if (!areOccupied(board, startX, startY, endX, endY))
            {
                incorrectPlacement = false;
            }
        }
        reajustCoordinates(&startX, &endX);
        reajustCoordinates(&startY, &endY);
        if (startX == endX)
        {
            for (int j = startY; j <= endY; j++)
            {
                board[startX][j] = fromNumberToChar(*boatID);
                if (!computer)
                    showBoard[startX][j] = BOAT;
            }
        }
        else
        {
            for (int i = startX; i <= endX; i++)
            {
                board[i][startY] = fromNumberToChar(*boatID);
                if (!computer)
                    showBoard[i][startY] = BOAT;
            }
        }
        (*boatID)++;
    }
}

void computerPicksCoordinatesToHit(char firstBoard[MAX][MAX], char PlayerShowBoard[MAX][MAX], int playerBoatHitAmount[MAXBOATS], const size_t size, const int moves[MAXIMUM], int move, int boatSize[MAXBOATS], int *aliveBoatsPlayer)
{
    int X;
    int Y;
    move = move - 1;
    X = (moves[move] / size) % size;
    Y = moves[move] % size;
    move++;
    if (firstBoard[X][Y] == WATER)
    {
        PlayerShowBoard[X][Y] = MISS;
    }
    else
    {
        PlayerShowBoard[X][Y] = HIT;
        int hitPosition = fromCharToNumber(firstBoard[X][Y]);
        playerBoatHitAmount[hitPosition]++;
        if (playerBoatHitAmount[hitPosition] == boatSize[hitPosition])
        {
            *aliveBoatsPlayer ^= (1 << (hitPosition));
        }
        if (!(*aliveBoatsPlayer))
        {
            cout << "\nGame over!\n";
        }
        else
            computerPicksCoordinatesToHit(firstBoard, PlayerShowBoard, playerBoatHitAmount, size, moves, ++move, boatSize, aliveBoatsPlayer);
    }
}

void playerPicksCoordinatesToHit(char firstShowBoard[MAX][MAX], char firstBoard[MAX][MAX], char PlayerShowBoard[MAX][MAX], int computerBoatHitAmount[MAXBOATS], int boatSize[MAXBOATS], const size_t size, int *aliveBoatsComputer)
{
    int X = -2, Y = -2;
    while (!(X == -1 && Y == -1) && ((X < 1 || X > size) || (Y < 1 || Y > size) || areAlreadyHit(firstShowBoard, X, Y)))
    {
        cout << "\nPick coordinates: ";
        cin >> X;
        cin >> Y;
    }
    if (X == -1)
    {
        saveG = true;
    }
    else
    {
        reajustCoordinates(&X, &Y);
        if (firstBoard[X][Y] == WATER)
        {
            firstShowBoard[X][Y] = MISS;
            cout << "\nMissed!\n";
        }
        else
        {
            firstShowBoard[X][Y] = HIT;
            int hitPosition = fromCharToNumber(firstBoard[X][Y]);
            cout << "\nHit!\n";
            computerBoatHitAmount[hitPosition]++;
            if (computerBoatHitAmount[hitPosition] == boatSize[hitPosition])
            {
                cout << "\nSunk!\n";
                *aliveBoatsComputer ^= (1 << (hitPosition));
                if (!(*aliveBoatsComputer))
                {
                    cout << "\nGame over!\n";
                }
            }
            printBoards(firstShowBoard, PlayerShowBoard, size);
            if ((*aliveBoatsComputer))
                playerPicksCoordinatesToHit(firstShowBoard, firstBoard, PlayerShowBoard, computerBoatHitAmount, boatSize, size, aliveBoatsComputer);
        }
    }
}

void fillInAllBoats(char board[MAX][MAX], char showBoard[MAX][MAX], size_t size, int automatic, int *boatID, bool computer)
{
    switch (automatic)
    {
    case 1:
        fillInBoatPositionsAuto(board, showBoard, size, CARRIERS_COUNT, CARRIER_SIZE, boatID, computer);
        fillInBoatPositionsAuto(board, showBoard, size, CRUISERS_COUNT, CRUISER_SIZE, boatID, computer);
        fillInBoatPositionsAuto(board, showBoard, size, SUBMARINES_COUNT, SUBMARINE_SIZE, boatID, computer);
        fillInBoatPositionsAuto(board, showBoard, size, DESTROYERS_COUNT, DESTROYER_SIZE, boatID, computer);
        break;
    case 2:

        fillInBoatPositions(board, showBoard, size, CARRIERS_COUNT, CARRIER_SIZE, boatID);
        fillInBoatPositions(board, showBoard, size, CRUISERS_COUNT, CRUISER_SIZE, boatID);
        fillInBoatPositions(board, showBoard, size, SUBMARINES_COUNT, SUBMARINE_SIZE, boatID);
        fillInBoatPositions(board, showBoard, size, DESTROYERS_COUNT, DESTROYER_SIZE, boatID);
        break;
    };
}
void setBoatSizes(int boatSize[MAXBOATS])
{
    int counter = 0;
    for (int i = 0; i < CARRIERS_COUNT; i++)
    {
        boatSize[i] = CARRIER_SIZE;
    }
    counter += CARRIERS_COUNT;
    for (int i = counter; i < CRUISERS_COUNT + counter; i++)
    {
        boatSize[i] = CRUISER_SIZE;
    }
    counter += CRUISERS_COUNT;
    for (int i = counter; i < SUBMARINES_COUNT + counter; i++)
    {
        boatSize[i] = SUBMARINE_SIZE;
    }
    counter += SUBMARINES_COUNT;
    for (int i = counter; i < DESTROYERS_COUNT + counter; i++)
    {
        boatSize[i] = DESTROYER_SIZE;
    }
}
void setUpGameDetailsFromFile(char firstShowBoard[MAX][MAX],
                              char secondShowBoard[MAX][MAX],
                              char firstBoard[MAX][MAX],
                              char secondBoard[MAX][MAX],
                              size_t *size,
                              int *aliveBoatsPlayer,
                              int *aliveBoatsComputer,
                              int *move,
                              int moves[MAXIMUM],
                              int playerBoatHitAmount[MAXBOATS],
                              int computerBoatHitAmount[MAXBOATS])
{
    ifstream saveGameFile("file.txt");
    if (!saveGameFile)
    {
        cout << "Error with reading file" << endl;
    }
    saveGameFile >> *size;

    for (int i = 0; i < *size; i++)
    {
        for (int j = 0; j < *size; j++)
        {
            saveGameFile >> firstShowBoard[i][j];
        }
    }
    for (int i = 0; i < *size; i++)
    {
        for (int j = 0; j < *size; j++)
        {
            saveGameFile >> secondShowBoard[i][j];
        }
    }
    for (int i = 0; i < *size; i++)
    {
        for (int j = 0; j < *size; j++)
        {
            saveGameFile >> firstBoard[i][j];
        }
    }
    for (int i = 0; i < *size; i++)
    {
        for (int j = 0; j < *size; j++)
        {
            saveGameFile >> secondBoard[i][j];
        }
    }
    saveGameFile >> *aliveBoatsPlayer;
    saveGameFile >> *aliveBoatsComputer;
    saveGameFile >> *move;
    for (int i = 0; i < (*size) * (*size); i++)
    {
        saveGameFile >> moves[i];
    }

    for (int i = 0; i < MAXBOATS; i++)
    {
        saveGameFile >> playerBoatHitAmount[i];
    }
    for (int i = 0; i < MAXBOATS; i++)
    {
        saveGameFile >> computerBoatHitAmount[i];
    }
    saveGameFile.close();
}
void saveGame(char firstShowBoard[MAX][MAX],
              char secondShowBoard[MAX][MAX],
              char firstBoard[MAX][MAX],
              char secondBoard[MAX][MAX],
              size_t size,
              int aliveBoatsPlayer,
              int aliveBoatsComputer,
              int move,
              const int moves[MAXIMUM],
              int playerBoatHitAmount[MAXBOATS],
              int computerBoatHitAmount[MAXBOATS])
{

    ofstream saveGameFile("file.txt");
    if (!saveGameFile)
    {
        cout << "Error with reading file" << endl;
    }
    saveGameFile << size << endl;

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            saveGameFile << firstShowBoard[i][j] << " ";
        }
        saveGameFile << endl;
    }
    saveGameFile << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            saveGameFile << secondShowBoard[i][j] << " ";
        }
        saveGameFile << endl;
    }
    saveGameFile << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            saveGameFile << firstBoard[i][j] << " ";
        }
        saveGameFile << endl;
    }
    saveGameFile << endl;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            saveGameFile << secondBoard[i][j] << " ";
        }
        saveGameFile << endl;
    }
    saveGameFile << endl;
    saveGameFile << aliveBoatsPlayer << " ";
    saveGameFile << aliveBoatsComputer << " ";
    saveGameFile << move << endl;
    for (int i = 0; i < size * size; i++)
    {
        saveGameFile << moves[i] << " ";
    }
    saveGameFile << endl;
    for (int i = 0; i < MAXBOATS; i++)
    {
        saveGameFile << playerBoatHitAmount[i] << " ";
    }
    saveGameFile << endl;
    for (int i = 0; i < MAXBOATS; i++)
    {
        saveGameFile << computerBoatHitAmount[i] << " ";
    }
    saveGameFile << endl;

    saveGameFile.close();
}
void game(char firstShowBoard[MAX][MAX], char firstBoard[MAX][MAX], char secondShowBoard[MAX][MAX], char secondBoard[MAX][MAX], int playerBoatHitAmount[MAXBOATS], int computerBoatHitAmount[MAXBOATS], size_t size, int moves[MAXIMUM], int move, int boatSize[MAXBOATS], int aliveBoatsPlayer, int aliveBoatsComputer)
{
    int whoStartsFirst = rand() % 2;
    while (aliveBoatsComputer && aliveBoatsPlayer)
    {
        if (whoStartsFirst)
        {
            printBoards(firstShowBoard, secondShowBoard, size);
            playerPicksCoordinatesToHit(firstShowBoard, firstBoard, secondShowBoard, computerBoatHitAmount, boatSize, size, &aliveBoatsComputer);
            // computerPicksCoordinatesToHit(firstBoard, firstShowBoard, computerBoatHitAmount, size, moves, move, boatSize, &aliveBoatsComputer);
            if (aliveBoatsComputer && !saveG)
                computerPicksCoordinatesToHit(secondBoard, secondShowBoard, playerBoatHitAmount, size, moves, move, boatSize, &aliveBoatsPlayer);
            cout << "\ncurrent move: " << move << "\n";
            move++;
        }
        else
        {
            computerPicksCoordinatesToHit(secondBoard, secondShowBoard, playerBoatHitAmount, size, moves, move, boatSize, &aliveBoatsPlayer);
            if (aliveBoatsPlayer)
            {
                cout << "\ncurrent move: " << move << "\n";
                move++;
                printBoards(firstShowBoard, secondShowBoard, size);
                playerPicksCoordinatesToHit(firstShowBoard, firstBoard, secondShowBoard, computerBoatHitAmount, boatSize, size, &aliveBoatsComputer);

            } // if (aliveBoatsPlayer)
            //   computerPicksCoordinatesToHit(firstBoard, firstShowBoard, playerBoatHitAmount, size, moves, move, boatSize, &aliveBoatsPlayer);
        }
        if (saveG)
        {
            saveGame(secondShowBoard, firstShowBoard, secondBoard, firstBoard, size, aliveBoatsPlayer, aliveBoatsComputer, move, moves, playerBoatHitAmount, computerBoatHitAmount);
            cout << "The game was saved. See you next time!";
            break;
        }
    }
}

int main()
{
    srand(time(0));
    int selectNewOrSaved = 0;
    while (selectNewOrSaved != 1 && selectNewOrSaved != 2)
    {
        cout << "Start a new game: 1\nContinue a saved game: 2\n(Enter (-1, -1) as coordinates to save the game at any time when playing.)\n";
        cin >> selectNewOrSaved;
    }
    int boatsNumber = (1 << (DESTROYERS_COUNT + SUBMARINES_COUNT + CRUISERS_COUNT + CARRIERS_COUNT)) - 1;
    int aliveBoatsPlayer = boatsNumber;
    int aliveBoatsComputer = boatsNumber;
    int computerBoatHitAmount[MAXBOATS] = {0};
    int playerBoatHitAmount[MAXBOATS] = {0};
    int boatSize[MAXBOATS] = {0};
    setBoatSizes(boatSize);
    char computerBoard[MAX][MAX];
    char computerShowBoard[MAX][MAX];
    char PlayerBoard[MAX][MAX];
    char PlayerShowBoard[MAX][MAX];
    size_t size;
    int move;
    size_t sizeRandom;
    int computerPicks[MAXIMUM];

    if (selectNewOrSaved == 1)
    {
        move = 1;
        int type = -1;
        while (type < 1 || type > 3)
        {
            cout << "Enter 1 for Calm Waters (10x10)\nEnter 2 for Rough Seas (12x12)\nEnter 3 for Storm of Steel (15x15)\n";
            cin >> type;
        }
        switch (type)
        {
        case 1:
            type = 10;
            break;
        case 2:
            type = 12;
            break;
        case 3:
            type = 15;
            break;
        };
        size = type;
        sizeRandom = size * size;
        for (int i = 0; i < sizeRandom; i++)
            computerPicks[i] = i;
        shuffle(computerPicks, sizeRandom);

        fillWithWater(computerShowBoard, size);
        fillWithWater(PlayerBoard, size);
        fillWithWater(computerBoard, size);
        fillWithWater(PlayerShowBoard, size);

        int automatic = -1;
        while (automatic < 1 || automatic > 2)
        {
            cout << "Enter 1 for automatic arrangment\nEnter 2 to set up the board yourself\n";
            cin >> automatic;
        }
        int startID = 0;
        fillInAllBoats(PlayerBoard, PlayerShowBoard, size, automatic, &startID, 0);
        int start2ID = 0;
        fillInAllBoats(computerBoard, computerShowBoard, size, 1, &start2ID, 1);
        printBoards(computerShowBoard, PlayerShowBoard, size);

        // saveGame(PlayerShowBoard,computerShowBoard,PlayerBoard,computerBoard,size, aliveBoatsPlayer,aliveBoatsComputer,move,computerPicks, playerBoatHitAmount, computerBoatHitAmount);
        //  printBoards(computerBoard, PlayerBoard, size);
    }
    else
    {
        setUpGameDetailsFromFile(PlayerShowBoard, computerShowBoard, PlayerBoard, computerBoard, &size, &aliveBoatsPlayer, &aliveBoatsComputer, &move, computerPicks, playerBoatHitAmount, computerBoatHitAmount);
    }
    game(computerShowBoard, computerBoard, PlayerShowBoard, PlayerBoard, playerBoatHitAmount, computerBoatHitAmount, size, computerPicks, move, boatSize, aliveBoatsPlayer, aliveBoatsComputer);

    return 0;
}
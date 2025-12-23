#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;
const size_t MAX = 15;
const size_t MAXIMUM = MAX * MAX;
const size_t MAXBOATS = 10;
const char WATER = '~';
const char HIT = 'x';
const char MISS = 'o';
const char BOAT = '#';
const int DESTROYERSIZE = 1;
const int SUBMARINESIZE = 2;
const int CRUISERSIZE = 3;
const int CARRIERSIZE = 4;
const int DESTROYERS = 4;
const int SUBMARINES = 3;
const int CRUISERS = 2;
const int CARRIERS = 1;
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
    srand(time(0));

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

void reajust(int *X, int *Y)
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
    reajust(&X, &Y);
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
            incorrectPlacement = false;
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
                incorrectPlacement = true;
                cout << "Try again with new coordinates. These are not valid.";
            }
            else
            {
                if (!incorrectPlacement && !areOccupied(board, startX, startY, endX, endY))
                {
                    incorrectPlacement = false;
                }
                else
                {
                    incorrectPlacement = true;
                    cout << "Try again with new coordinates. These are already occupied.";
                }
            }
        }

        {
            reajust(&startX, &startY);
            reajust(&endX, &endY);
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
            incorrectPlacement = false;
            srand(time(0));
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

            if (!areValidCoordinates(startX, startY, endX, endY, boatSize, size))
            {
                incorrectPlacement = true;
            }
            if (!incorrectPlacement && !areOccupied(board, startX, startY, endX, endY))
            {
                incorrectPlacement = false;
            }
            else
            {
                incorrectPlacement = true;
            }
        }

        if (startX == endX)
        {
            for (int j = startY - 1; j <= endY - 1; j++)
            {
                board[startX - 1][j] = fromNumberToChar(*boatID);
                if (!computer)
                    showBoard[startX - 1][j] = BOAT;
            }
        }
        else
        {
            for (int i = startX - 1; i <= endX - 1; i++)
            {
                board[i][startY - 1] = fromNumberToChar(*boatID);
                if (!computer)
                    showBoard[i][startY - 1] = BOAT;
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
    int X = -1, Y = -1;
    while ((X < 1 || X > size) || (Y < 1 || Y > size) || areAlreadyHit(firstShowBoard, X, Y))
    {
        cout << "\nPick coordinates: ";
        cin >> X;
        cin >> Y;
    }
    reajust(&X, &Y);
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
        playerPicksCoordinatesToHit(firstShowBoard, firstBoard, PlayerShowBoard, computerBoatHitAmount, boatSize, size, aliveBoatsComputer);
    }
}

void fillInAllBoats(char board[MAX][MAX], char showBoard[MAX][MAX], size_t size, int automatic, int *boatID, bool computer)
{
    switch (automatic)
    {
    case 1:
        fillInBoatPositionsAuto(board, showBoard, size, CARRIERS, CARRIERSIZE, boatID, computer);
        fillInBoatPositionsAuto(board, showBoard, size, CRUISERS, CRUISERSIZE, boatID, computer);
        fillInBoatPositionsAuto(board, showBoard, size, SUBMARINES, SUBMARINESIZE, boatID, computer);
        fillInBoatPositionsAuto(board, showBoard, size, DESTROYERS, DESTROYERSIZE, boatID, computer);
        break;
    case 2:

        fillInBoatPositions(board, showBoard, size, CARRIERS, CARRIERSIZE, boatID);
        fillInBoatPositions(board, showBoard, size, CRUISERS, CRUISERSIZE, boatID);
        fillInBoatPositions(board, showBoard, size, SUBMARINES, SUBMARINESIZE, boatID);
        fillInBoatPositions(board, showBoard, size, DESTROYERS, DESTROYERSIZE, boatID);
        break;
    };
}
void setBoatSizes(int boatSize[MAXBOATS])
{
    int counter = 0;
    for (int i = 0; i < CARRIERS; i++)
    {
        boatSize[i] = CARRIERSIZE;
    }
    counter += CARRIERS;
    for (int i = counter; i < CRUISERS + counter; i++)
    {
        boatSize[i] = CRUISERSIZE;
    }
    counter += CRUISERS;
    for (int i = counter; i < SUBMARINES + counter; i++)
    {
        boatSize[i] = SUBMARINESIZE;
    }
    counter += SUBMARINES;
    for (int i = counter; i < DESTROYERS + counter; i++)
    {
        boatSize[i] = DESTROYERSIZE;
    }
}
void game(char firstShowBoard[MAX][MAX], char firstBoard[MAX][MAX], char secondShowBoard[MAX][MAX], char secondBoard[MAX][MAX], int playerBoatHitAmount[MAXBOATS], int computerBoatHitAmount[MAXBOATS], const size_t size, const int moves[MAXIMUM], int move, int boatSize[MAXBOATS], int aliveBoatsPlayer, int aliveBoatsComputer)
{
    int whoStartsFirst = rand() % 2;

    while (aliveBoatsComputer && aliveBoatsPlayer)
    {
        if (whoStartsFirst)
        {
            playerPicksCoordinatesToHit(firstShowBoard, firstBoard, secondShowBoard, computerBoatHitAmount, boatSize, size, &aliveBoatsComputer);
            // computerPicksCoordinatesToHit(firstBoard, firstShowBoard, computerBoatHitAmount, size, moves, move, boatSize, &aliveBoatsComputer);
            if (aliveBoatsComputer)
                computerPicksCoordinatesToHit(secondBoard, secondShowBoard, playerBoatHitAmount, size, moves, move, boatSize, &aliveBoatsPlayer);
            cout << "\ncurrent move: " << move << "\n";
            move++;
            printBoards(firstShowBoard, secondShowBoard, size);
        }
        else
        {
            computerPicksCoordinatesToHit(secondBoard, secondShowBoard, playerBoatHitAmount, size, moves, move, boatSize, &aliveBoatsPlayer);
            if (aliveBoatsPlayer)
                playerPicksCoordinatesToHit(firstShowBoard, firstBoard, secondShowBoard, computerBoatHitAmount, boatSize, size, &aliveBoatsComputer);
            // if (aliveBoatsPlayer)
            //   computerPicksCoordinatesToHit(firstBoard, firstShowBoard, playerBoatHitAmount, size, moves, move, boatSize, &aliveBoatsPlayer);
            cout << "\ncurrent move: " << move << "\n";
            move++;
            printBoards(firstShowBoard, secondShowBoard, size);
        }
    }
}

int main()
{
    int boatsNumber = (1 << (DESTROYERS + SUBMARINES + CRUISERS + CARRIERS)) - 1;
    int aliveBoatsPlayer = boatsNumber;
    int aliveBoatsComputer = boatsNumber;
    int computerBoatHitAmount[MAXBOATS] = {0};
    int playerBoatHitAmount[MAXBOATS] = {0};
    int boatSize[MAXBOATS] = {0};
    setBoatSizes(boatSize);

    int move = 1;
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
    const size_t size = type;

    const size_t sizeRandom = size * size;
    int computerPicks[MAXIMUM];
    for (int i = 0; i < sizeRandom; i++)
        computerPicks[i] = i;
    shuffle(computerPicks, sizeRandom);

    char computerBoard[MAX][MAX];
    char computerShowBoard[MAX][MAX];
    char PlayerBoard[MAX][MAX];
    char PlayerShowBoard[MAX][MAX];
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

    printBoards(computerShowBoard, PlayerShowBoard, size);
    int start2ID = 0;
    fillInAllBoats(computerBoard, computerShowBoard, size, 1, &start2ID, 1);

    // printBoards(computerBoard, PlayerBoard, size);

    game(computerShowBoard, computerBoard, PlayerShowBoard, PlayerBoard, playerBoatHitAmount, computerBoatHitAmount, size, computerPicks, move, boatSize, aliveBoatsPlayer, aliveBoatsComputer);

    return 0;
}
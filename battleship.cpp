#include <iostream>
#include <cstdlib>
#include <ctime>
#include <windows.h>
using namespace std;
const size_t MAX = 15;
const size_t MAXIMUM = MAX * MAX;
const char water = '~';
const char hit = 'x';
const char miss = 'o';
const char boat = '#';
const int destroyerSize = 1;
const int submarineSize = 2;
const int cruiserSize = 3;
const int carrierSize = 4;
const int destroyers = 4;
const int submarines = 3;
const int cruisers = 2;
const int carriers = 1;
const char *typeBoat[] = {"Destroyer", "Submarine", "Cruiser", "Carrier"};

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
            board[i][j] = water;
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
    return (board[X][Y] != water);
}

bool areOccupied(char board[MAX][MAX], int startX, int startY, int endX, int endY)
{
    bool occupied = false;
    if (startX == endX)
    {
        for (int i = startY - 1; i <= endY - 1; i++)
        {
            if (board[startX - 1][i] == boat)
            {
                occupied = true;
            }
        }
    }
    else if (startY == endY)
    {
        for (int j = startX - 1; j <= endX - 1; j++)
        {
            if (board[j][startY - 1] == boat)
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


void fillInBoatPositions(char board[MAX][MAX], const size_t size, const int amount, const int boatSize)
{
    for (int t = 0; t < amount; t++)
    {
        bool incorrectPlacement = true;
        int startX;
        int startY;
        int endX;
        int endY;
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
                    board[startX][j] = boat;
                }
            }
            else
            {
                for (int i = startX; i <= endX; i++)
                {
                    board[i][startY] = boat;
                }
            }
        }
        printBoard(board, size);
    }
}

void fillInBoatPositionsAuto(char board[MAX][MAX], const size_t size, const int amount, const int boatSize)
{
    for (int t = 0; t < amount; t++)
    {
        bool incorrectPlacement = true;
        int startX;
        int startY;
        int endX;
        int endY;
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
                board[startX - 1][j] = boat;
            }
        }
        else
        {
            for (int i = startX - 1; i <= endX - 1; i++)
            {
                board[i][startY - 1] = boat;
            }
        }
    }
}



void computerPicksCoordinatesToHit(char firstBoard[MAX][MAX], const size_t size, const int moves[MAXIMUM], int move)
{
    int X;
    int Y;
    move = move - 1;
    X = (moves[move] / size) % size;
    Y = moves[move] % size;

    if (firstBoard[X][Y] == water)
    {
        firstBoard[X][Y] = miss;
    }
    else
    {
        firstBoard[X][Y] = hit;
    }
    move++;
}

void playerPicksCoordinatesToHit(char firstShowBoard[MAX][MAX], char firstBoard[MAX][MAX], const size_t size)
{
    int X = -1, Y = -1;
    while ((X < 1 || X > size) || (Y < 1 || Y > size) || areAlreadyHit(firstShowBoard, X, Y))
    {
        cout << "\nPick coordinates: ";
        cin >> X;
        cin >> Y;
    }
    reajust(&X, &Y);
    if (firstBoard[X][Y] == water)
    {
        firstShowBoard[X][Y] = miss;
    }
    else
    {
        firstShowBoard[X][Y] = hit;
    }
}

void fillInAllBoats(char board[MAX][MAX], size_t size, int automatic)
{
    switch (automatic)
    {
    case 1:
        fillInBoatPositionsAuto(board, size, carriers, carrierSize);
        fillInBoatPositionsAuto(board, size, cruisers, cruiserSize);
        fillInBoatPositionsAuto(board, size, submarines, submarineSize);
        fillInBoatPositionsAuto(board, size, destroyers, destroyerSize);
        break;
    case 2:
        fillInBoatPositions(board, size, destroyers, destroyerSize);
        fillInBoatPositions(board, size, submarines, submarineSize);
        fillInBoatPositions(board, size, cruisers, cruiserSize);
        fillInBoatPositions(board, size, carriers, carrierSize);
        break;
    };
}

void game(char firstShowBoard[MAX][MAX], char firstBoard[MAX][MAX], char secondBoard[MAX][MAX], const size_t size, const int moves[MAXIMUM], int move)
{
    int whoStartsFirst = rand() % 2;
    while (true)
    {
        if (whoStartsFirst)
        {
            playerPicksCoordinatesToHit(firstShowBoard, firstBoard, size);
            computerPicksCoordinatesToHit(secondBoard, size, moves, move);
            cout << "\ncurrent move: " << move << "\n";
            move++;
            printBoards(firstShowBoard, secondBoard, size);
        }
        else
        {
            computerPicksCoordinatesToHit(secondBoard, size, moves, move);
            playerPicksCoordinatesToHit(firstShowBoard, firstBoard, size);
            cout << "\ncurrent move: " << move << "\n";
            move++;
            printBoards(firstShowBoard, secondBoard, size);
        }
    }
}

int main()
{
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
    fillWithWater(computerShowBoard, size);
    fillWithWater(PlayerBoard, size);
    fillWithWater(computerBoard, size);

    int automatic = -1;
    while (automatic < 1 || automatic > 2)
    {
        cout << "Enter 1 for automatic arrangment\nEnter 2 to set up the board yourself\n";
        cin >> automatic;
    }
    fillInAllBoats(PlayerBoard, size, automatic);

    printBoards(computerShowBoard, PlayerBoard, size);

    fillInAllBoats(computerBoard, size, 1);

   // printBoards(computerBoard, PlayerBoard, size);

    game(computerShowBoard, computerBoard, PlayerBoard, size, computerPicks, move);

    return 0;
}
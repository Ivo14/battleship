#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;
const size_t MAX = 15;

void fillWithWater(char board[MAX][MAX], const size_t size)
{
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
        {
            board[i][j] = '~';
        }
    }
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

int main()
{
    const size_t size = 15;
    char computerBoard[size][size];
    char PlayerBoard[size][size];
    fillWithWater(computerBoard, size);
    fillWithWater(PlayerBoard, size);
    printBoards(computerBoard, PlayerBoard, size);

    return 0;
}
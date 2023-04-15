#include <stdio.h>
#include <math.h>
#include <iostream>
#include <stdlib.h>
#include <fstream>

// Sample test ./sequential 9 ./9.txt ./9_ans.txt
using namespace std;
// Function to print the Sudoku grid
void read_grid(int *grid, int n, string filename)
{
    ifstream file(filename);
    if (file.is_open())
    {
        for (int i = 0; i < n * n; i++)
        {
            file >> grid[i];
        }
        file.close();
    }
    else
    {
        cout << "Error: Unable to open input file." << endl;
    }
}

// Function to print the Sudoku grid
void print_grid(int *grid, int n)
{
    for (int i = 0; i < n * n; i++)
    {
        cout << grid[i] << " ";
        if ((i + 1) % n == 0)
            cout << endl;
    }
}

// Function to check the Sudoku solve grid is correct or not
bool check_grid(int *grid, int n, string filename)
{
    bool right = true;
    int temp;
    ifstream file(filename);
    if (file.is_open())
    {
        for (int i = 0; i < n * n; i++)
        {
            file >> temp;
            if (temp != grid[i])
            {
                right = false;
                break;
            }
        }
        file.close();
    }
    else
    {
        cout << "Error: Unable to open answer file." << endl;
        return false;
    }
    return right;
}

// Function to check if a number is safe to be placed in a cell
bool is_safe(int *grid, int n, int row, int col, int num)
{
    int idx;
    // Check if the number is already in the row
    for (int i = 0; i < n; i++)
    {
        idx = row * n + i;
        if (grid[idx] == num)
        {
            return false;
        }
    }
    // Check if the number is already in the column, can be optimized by data locality when size increase
    for (int i = 0; i < n; i++)
    {
        idx = i * n + col;
        if (grid[idx] == num)
        {
            return false;
        }
    }

    // Check if the number is already in the sqrt(n) box
    int root = sqrt(n);
    int box_row = row - row % root;
    int box_col = col - col % root;
    for (int i = box_row; i < box_row + root; i++)
    {
        for (int j = box_col; j < box_col + root; j++)
        {
            idx = i * n + j;
            if (grid[idx] == num)
            {
                return false;
            }
        }
    }
    return true;
}

// Function to solve the Sudoku puzzle using backtracking
bool solve_sudoku(int *grid, int n, int row, int col)
{
    // If all rows and columns are processed, the puzzle is solved
    if (row == n - 1 && col == n)
    {
        return true;
    }
    // If all columns of a row are processed, move to the next row
    if (col == n)
    {
        row++;
        col = 0;
    }

    int idx = row * n + col;
    // If the current cell is not empty, skip to the next cell
    if (grid[idx] != 0)
    {
        return solve_sudoku(grid, n, row, col + 1);
    }
    // Try to place a number in the current cell
    for (int num = 1; num <= n; num++)
    {
        if (is_safe(grid, n, row, col, num))
        {
            grid[idx] = num;
            if (solve_sudoku(grid, n, row, col + 1))
            {
                return true;
            }
            grid[idx] = 0; // backtrack
        }
    }
    return false;
}

int main(int argc, char **argv)
{

    if (argc < 4)
    {
        printf("Usage: ./sequential <int:n> <string:input_file> <string:answer_file> \n");
        abort();
    }
    int n = atoi(argv[1]);
    if (sqrt(n) * sqrt(n) != n)
    {
        printf("Error: n must be a perfect square. \n");
        abort();
    }
    string input_filename = argv[2];
    string answer_file = argv[3];

    int *grid = (int *)malloc(n * n * sizeof(int));
    read_grid(grid, n, input_filename);

    cout << "Input grid: " << endl;
    print_grid(grid, n);
    if (solve_sudoku(grid, n, 0, 0))
    {
        if (check_grid(grid, n, answer_file))
            printf("Solved grid: Correct! \n");
        else
            printf("Solved grid: Wrong! \n");
        print_grid(grid, n);
    }
    else
    {
        cout << "No solution exists" << endl;
    }
    free(grid);
    return 0;
}

#include <cstdlib>
#include <iostream>

// g++ -g -O0 val_test01_solved.cpp -o val_test01_solved && valgrind ./val_test01_solved
// valgrind --leak-check=full ./val_test01_solved
// valgrind --tool=callgrind ./val_test01_solved
// Use the "--leak-check=full" option to enable detailed reporting of memory leaks.
// Use the "--show-reachable=yes" option to report on memory that is still accessible but has not been freed.
// Use the "--track-origins=yes" option to track the source of uninitialized values.
// Use the "--suppressions=file" option to suppress known false positives.
using namespace std;

int main();
void f(int n);

//****************************************************************************80

int main()

//****************************************************************************80
//
//  Purpose:
//
//    MAIN is the main program for TEST01.
//
//  Discussion:
//
//    TEST01 calls F, which has a memory "leak".  This memory leak can be
//    detected by VALGRID.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 May 2011
//
{
    int n = 10;

    cout << "\n";
    cout << "TEST01\n";
    cout << "  C++ version.\n";
    cout << "  A sample code for analysis by VALGRIND.\n";

    f(n);
    //
    //  Terminate.
    //
    cout << "\n";
    cout << "TEST01\n";
    cout << "  Normal end of execution.\n";

    return 0;
}
//****************************************************************************80

void f(int n)

//****************************************************************************80
//
//  Purpose:
//
//    F computes N+1 entries of the Fibonacci sequence.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 May 2011
//
{
    int i;
    int *x;

    x = (int *)malloc((n + 1) * sizeof(int)); // It should allocate n+1 space not n. Otherwise it would out of index.

    x[0] = 1;
    cout << "  " << 0 << "  " << x[0] << "\n";

    x[1] = 1;
    cout << "  " << 1 << "  " << x[1] << "\n";

    for (i = 2; i <= n; i++)
    {
        x[i] = x[i - 1] + x[i - 2];
        cout << "  " << i << "  " << x[i] << "\n";
    }

    // delete[] x;
    // Here is the bug, it should be free, but not delete.
    free(x);

    return;
}

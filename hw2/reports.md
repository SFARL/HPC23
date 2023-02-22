# HW2 wf2099

## Finding Memory bugs using valgrind

### Val_test01

First Run:
```
valgrind ./val_test01_solved
==9586== Memcheck, a memory error detector
==9586== Invalid write of size 4
==9586== Invalid read of size 4
==9586== Mismatched free() / delete / delete []
==9586== ERROR SUMMARY: 3 errors from 3 contexts (suppressed: 0 from 0)
```

Two Bug:
```C++
    x = (int *)malloc((n + 1) * sizeof(int)); // It should allocate n+1 space not n. Otherwise it would out of index.

    // delete[] x;
    // Here is the bug, it should be free, but not delete.
    free(x);
```

After fix:
```
==10502== ERROR SUMMARY: 0 errors from 0 contexts (suppressed: 0 from 0)
```
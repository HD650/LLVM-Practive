# Dominance Analysis (WIP)

## example
Given a llvm function like:
```
    define void @foo() 
    {
        A:
            br i1 undef, label %B, label %C
        B:
            br i1 undef, label %D, label %E
        C:
            br label %E
        D:
            ret void
        E:
            ret void
    }
```

The cfg is like:
```
       (A)
       / \
      /   \
    (B)   (C)
     |\    |
     | \   |
     |  \  |
     |   \ |
     |    \|
    (D)   (E)
```

Then the dominator tree generated by the analysis is:
```
    ├──[A,0,9]
    │   ├──[B,1,4]
    │   │   └──[D,2,3]
    │   ├──[E,5,6]
    │   └──[C,7,8]
```
## build, debug build and run
Refer to HelloWorld pass.

## Contenet
Implementations of two variants of dominance analysis:
1. Simple Fast Dominator Algorithm (Keith D. Cooper)
2. Lengauer-Tarjan Algorithm

Implementations of a dominator tree that:
1. holds the analysis result
2. could efficiently determine if A dominates B
3. could be efficiently updated (future?)

Implementations of an unittest system:
1. build and link llvm
2. run gtest suit to test the correctness of the analysis

## Details
**Simple Fast Dominator Algorithm (Keith D. Cooper)**
An iterative algorithm for dominator analysis. It's simple but slow, worst case O(N^2). The analysis result is a map contains immediate dominator information, extra code needed to convert the map to a dominator tree that is efficient when query the dominator information.
Details see files `IterativeDominanceAnalysis.h`.

**Lengauer-Tarjan Algorithm**
WIP
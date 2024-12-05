:Namespace test
    data ← ⍎¨⊃⎕NGET '/home/qqr/code/aoc/2024/day2/input' 1
    isSafeOld ← {(^/({{(⍵>0)∧⍵<4}|-/⍵}⌺2)⍵)^(∧/(⍒⍵)=⍳≢⍵)∨∧/(⍋⍵)=⍳≢⍵}
    isSafeOld2 ← {(^/(⍵<0)∧⍵>¯4)∨^/(⍵>0)∧⍵<4}⊃-/↓⍉(⊢⌺2)
    isSafeOld3 ← {(≢⍵)≡1+|+/({{3<|⍵:0⋄×⍵}-/⍵}⌺2)⍵}
    isSafe ← {(≢⍵)≡1+|+/{3<|⍵:0⋄×⍵}¨2-/⍵}
    dropIdx ← {(⍺-1){(-⍺)⌽1↓⍺⌽⍵}⍵}
    result1 ← +/isSafe¨data
    result2 ← +/{(isSafe ⍵)∨∨/(⍳≢⍵){isSafe ⍺ dropIdx ⍵}¨⊂⍵}¨data
:EndNamespace

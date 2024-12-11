⍝ Parse data
data ← ⍎¨⊃⎕NGET './input' 1

⍝ isSafe takes a row ⍵ and returns 1 if it's safe, 0 otherwise
isSafe ← {(≢⍵)≡1+|+/{3<|⍵:0⋄×⍵}¨2-/⍵}


⍝ Drops idx ⍺ from vector ⍵
dropIdx ← {(⍺-1){(-⍺)⌽1↓⍺⌽⍵}⍵}

⍝ Map isSafe over the data and reduce to get part 1
result1 ← +/isSafe¨data

⍝ Now also check every row with every value removed
result2 ← +/{(isSafe ⍵)∨∨/(⍳≢⍵){isSafe ⍺ dropIdx ⍵}¨⊂⍵}¨data

⍝ Output results
⎕←'Part 1:',result1
⎕←'Part 2:',result2

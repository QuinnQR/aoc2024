data ← ⍎¨⊃⎕NGET './input' 1
isSafe ← {(≢⍵)≡1+|+/{3<|⍵:0⋄×⍵}¨2-/⍵}
result1 ← +/isSafe¨data
result2 ← +/{(isSafe ⍵)∨∨/(⊂⍵){isSafe (-⍵)⌽1↓⍵⌽⍺}¨1-⍨⍳≢⍵}¨data
⎕←'Part 1:',result1
⎕←'Part 2:',result2

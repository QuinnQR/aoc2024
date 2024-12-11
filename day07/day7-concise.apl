data  ← {⍎⍵~':'}¨⊃⎕NGET './input' 1
prune ← {⍺,⍵[⍸⍵≤⍺]}
check ← {0 = ≢⍵: {1 < +/⍵=⊃⍵: ⊃⍵  ⋄ 0}⍺ 
                 ((⊃⍺)prune(1↓⍺)⍺⍺(⊃⍵)) ∇ 1↓⍵}
checkLine←{(2↑⍵) ⍺⍺ check 2↓⍵}
⎕PP←17
concat←{⍎(⍕⍺),⍕⍵}
partOneOps ← {(⍺×⍵),⍺+⍵}
partTwoOps ← {(⍺×⍵),(⍺+⍵),⍺concat¨⊂⍵}
r1←+/ partOneOps checkLine ¨ data 
r2←+/ partTwoOps checkLine ¨ data 
⎕←'Part 1:',r1
⎕←'Part 2:',r2

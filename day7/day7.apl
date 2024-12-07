data  ← {⍎⍵~':'}¨⊃⎕NGET './input' 1

    ⍝ check goes from left to right and tries every combination 
    ⍝ of operators (like a breadth first search)
    ⍝ Results are pruned if they go above
    ⍝ the target to try and help reduce
    ⍝ exponential growth of search space
prune ← {⍺,⍵[⍸⍵≤⍺]}
check ← {0 = ≢⍵: {1 < +/⍵=⊃⍵: ⊃⍵  ⋄ 0}⍺ 
                 ((⊃⍺)prune(1↓⍺)⍺⍺(⊃⍵)) ∇ 1↓⍵}

    ⍝ Takes a line (i.e. from data) and applies check
checkLine←{(2↑⍵) ⍺⍺ check 2↓⍵}

    ⍝ concat uses string manipulation to concatenate 2 numbers
    ⍝ PP 17 is needed as concat works with string concatenation and parsing
⎕PP←17
concat←{⍎(⍕⍺),⍕⍵}

    ⍝ checkLine and check are operators and take a function argument. 
    ⍝ These function combine the left and right args in all
    ⍝ the ways that are valid for that part of the problem
partOneOps ← {(⍺×⍵),⍺+⍵}
partTwoOps ← {(⍺×⍵),(⍺+⍵),⍺concat¨⊂⍵}

    ⍝ Check how many lines are valid for each set of functions
    ⍝ and sum them to get the results
r1←+/ partOneOps checkLine ¨ data 
r2←+/ partTwoOps checkLine ¨ data 
⎕←'Part 1:',r1
⎕←'Part 2:',r2

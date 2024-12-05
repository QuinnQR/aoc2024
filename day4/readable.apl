⍝ Add white-space to data so all patterns can be reached from top-left
data ← ↑{⍵,4/⊂(≢⊃⍵)/' '}{⍵,'   '}¨⊃⎕NGET './input' 1

⍝ Get the indices needed for horizontal, vertical and diagonal/anti-diagonal lines of length 4
sqidx ← ↓⍉↑5{(⊂(⍵,⍺-⍵)),(⊂⍵,1),(⊂⍵,⍵),(⊂1,⍵)}¨⍳4

⍝ Get the indices needed for diagonal/anti-diagonal lines of length 3
xidx ← ↓⍉↑4{(⊂⍵,⍺-⍵),(⊂⍵,⍵)}¨⍳3

⍝ Finds all XMAS occurrences
match ← {+/sqidx{+/((⊂'XMAS'), ⊂'SAMX'){(^/⍵=⍺)}¨⊂⍵[⍺]}¨⊂⍵}
⍝ Finds all x-MAS occurrences (part 2)
xmatch ← {^/xidx{+/((⊂'MAS'), ⊂'SAM'){(^/⍵=⍺)}¨⊂⍵[⍺]}¨⊂⍵}

⍝ Result 1 looks at 4x4 windows, result 2 looks at 3x3 windows
result1 ← +/+/(match ⌺ 4 4)data
result2 ← +/+/(xmatch ⌺ 3 3)data
⎕←'Part 1:',result1
⎕←'Part 2:',result2

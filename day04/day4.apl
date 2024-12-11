data ← ↑{⍵,3/⊂(≢⊃⍵)/' '}{⍵,'  '}¨⊃⎕NGET './input' 1
sqidx ← ↓⍉↑5{(⊂(⍵,⍺-⍵)),(⊂⍵,1),(⊂⍵,⍵),(⊂1,⍵)}¨⍳4
xidx ← ↓⍉↑4{(⊂⍵,⍺-⍵),(⊂⍵,⍵)}¨⍳3
match ← {+/sqidx{+/((⊂'XMAS'), ⊂'SAMX'){(^/⍵=⍺)}¨⊂⍵[⍺]}¨⊂⍵}
xmatch ← {^/xidx{+/((⊂'MAS'), ⊂'SAM'){(^/⍵=⍺)}¨⊂⍵[⍺]}¨⊂⍵}
result1 ← +/+/(match ⌺ 4 4)data
result2 ← +/+/(xmatch ⌺ 3 3)data

⎕←'Part 1:',result1
⎕←'Part 2:',result2

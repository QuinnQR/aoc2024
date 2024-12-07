data ← {⍵⊂⍨1,1↓{⍵+⍵⌽⍨¯1}⍵⍷⍨⊂''}⊃⎕NGET './input' 1
rules ← {⍎¨(⊂⍵[1 2]),⊂⍵[4 5]}¨⊃data
updates ← ⍎¨3⊃data

    ⍝ Sorts the list such that all rules are followed.
sort ← {1≡≢⍵: ⍺,⍵ ⋄ ⍺∇argOp ⍵getNewArgs getMoveLocation ⍵}

    ⍝ Gets the index of the last item that should come before the current first item. 
getMoveLocation ← {⌈/⍸1,(⊂⊃⍵){rules∊⍨⊂⍵,⍺}¨1↓⍵}

    ⍝ Converts the move index to recursive arguments
getNewArgs ←{⍵=1: (⊂⊃⍺),(⊂1↓⍺) ⋄ (⊂⍬),(⊂(⍵⊃⍺),⍺~⍵⊃⍺)}

    ⍝ takes a nested list as a right arg and splits it into left/right args
    ⍝ e.g. (1 2) f argOp (3 (4 5)) → (1 2 3) f argOp (4 5)
argOp ← {(⍺,1⊃⍵) ⍺⍺ 2⊃⍵}
    
⍝ Gets the middle value of a list
getMid ← {⍵[⌈2÷⍨≢⍵]}

⍝ List of all updates sorted so they follow rules
sorted ← (⊂⍬)sort¨updates
    
⍝ Sum mid points of all correct/incorrect updates
result1 ← +/getMid¨sorted[⍸^/¨updates=sorted]
result2 ← +/getMid¨sorted[⍸∨/¨updates≠sorted]
⎕←'Part 1:',result1
⎕←'Part 2:',result2

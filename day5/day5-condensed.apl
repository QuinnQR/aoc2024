data ← {⍵⊂⍨1,1↓{⍵+⍵⌽⍨¯1}⍵⍷⍨⊂''}⊃⎕NGET './input' 1
rules ← {⍎¨(⊂⍵[1 2]),⊂⍵[4 5]}¨⊃data
updates ← ⍎¨3⊃data

    ⍝ Sort returns the list sorted such that all rules are followed.
sort ← {1≡≢⍵: ⍺,⍵ ⋄ ⍺∇argOp ⍵getNewArgs getMoveLocation ⍵}

    ⍝ Gets the index of an item that should come before the current first item. 
getMoveLocation ← {⌈/⍸1,(⊂⊃⍵){rules∊⍨⊂⍵,⍺}¨1↓⍵}

    ⍝ Converts swap index to recursive arguments
getNewArgs ←{⍵=1: (⊂⊃⍺),(⊂1↓⍺) ⋄ (⊂⍬),(⊂(⍵⊃⍺),⍺~⍵⊃⍺)}

    ⍝ takes the first element of the right arg and applies it as a left arg instead
    ⍝ e.g. (1 2) f argOp (3 4 5) → (1 2 3) f argOp (4 5)
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

data← {⍺,⍵}/⊃⎕NGET './input' 1
getMuls←'mul\([0-9]+,[0-9]+\)' ⎕S '&'
enable ← '(^|(do\(\)))(.*?)(don''t\(\)|$)' ⎕S '&'
result1 ← +/{×/⍎3↓⍵}¨getMuls data
result2 ← +/{×/⍎3↓⍵}¨getMuls {⍺,⍵}/ enable data
⎕←'Part 1:',result1
⎕←'Part 2:',result2

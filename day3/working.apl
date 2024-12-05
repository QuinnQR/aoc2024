:Namespace test
    data← {⍺,⍵}/⊃⎕NGET '/home/qqr/code/aoc/2024/day3/input' 1
    regex←'mul\([0-9]+,[0-9]+\)' ⎕S '&'
    enable ← '(^|(do\(\)))(.*?)(don''t\(\)|$)' ⎕S '&'
    result1 ← +/{×/⍎3↓⍵}¨regex data
    result2 ← +/{×/⍎3↓⍵}¨regex {⍺,⍵}/ enable data
:EndNamespace

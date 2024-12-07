
    data  ← ↑⊃⎕NGET '/home/qqr/code/aoc/2024/day6/input' 1
    start ← ⍸'^'⍷data

    parse ← {⍵='.':0 ⋄ ⍵='#':¯1 ⋄ ⍵='^':0}
    data  ← parse¨data

    dirs     ← (⊂¯1 0)(⊂0 1)(⊂1 0)(⊂0 ¯1)
    inBounds ← {∧/{(⍵ ≤ ⍴data)∧⍵>0 0}⊃⍵}

    ∇{ret}←reset
        data ← {0⌊⍵}¨data
        dirs ← (⊂¯1 0)(⊂0 1)(⊂1 0)(⊂0 ¯1)
        ret  ← 100
    ∇
    func←{
        ⍺←0
        ~inBounds ⍵: ⍺
        data[⍵]=¯1 : ⍺ ∇ ⍵+(⊃dirs[1])-⊃dirs[4]⊣(dirs⌽⍨←1)
        data[⍵]= 0 : (⍺ ⍺⍺ ⍵)∇ ⍵+(⊃dirs[1])⊣(data[⍵]+←1)
        data[⍵]= 4: ¯1
        ⍺∇⍵+(⊃dirs[1])⊣(data[⍵]+←1)
    }
    ∇ {ret} ← run
        ret ← {(⊃⍺+1),1↓⍺,⍵}func start
        reset
    ∇
    ∇ {ret} ← block tile; temp
         data[⊂tile]⊢←¯1
         ret ← (¯1)≡{⍺+1}func start
         reset
         data[⊂tile]⊢←0
 
    ∇
    firstRun←run
    result1←⊃firstRun  
result2←+/+/block¨2↓firstRun

⎕←'Part 1:',result1
⎕←'Part 2:',result2

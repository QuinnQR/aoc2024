data  ← ↑⊃⎕NGET './input' 1
startCoord ← ⍸'^'⍷data
data  ← {⍵='#':¯1 ⋄ ≢data}¨data
⍝ Up, Right, Down, Left (Note: order of rotation)
dirs     ← (⊂¯1 0)(⊂0 1)(⊂1 0)(⊂0 ¯1)
inBounds ← {∧/{(⍵ ≤ ⍴data)∧⍵>0 0}⊃⍵}
⍝ inBounds returns 1 if coordinate is in grid, 0 otherwise
    
slideRight ← {⍉1↓⍉⍺{⍺=0: ⍵⋄(⍺-1)∇⍵⌊1+¯1⌽⍵}0,⍵}
⍝ Given a row,finds how far to travel left until obstacle
⍝ We can then find other directions with reverse etc
    
⍝ Precompute is 4 matrices, each telling you how far
⍝ until an obstacle in each direction.
⍝ e.g. precompute[1][8;8] tells you how far to travel
⍝ up from the square 8;8
precompute ← 1 1 1 1
precompute[1] ← ⊂⍉(≢data)slideRight⍉data
precompute[2] ← ⊂⌽(≢data)slideRight⌽data
precompute[3] ← ⊂⍉⌽(≢data)slideRight⌽⍉data
precompute[4] ← ⊂(≢data)slideRight data
precopy ← precompute
⍝ If data[coord] has been changed, update coord 
⍝ will update precompute to match
∇{_}←update coord
    (⊃precopy[1])[;2⊃⊃coord]⊢←  (≢data)slideRight  data[;2⊃⊃coord]
    (⊃precopy[2])[⊃⊃coord;]⊢← ⌽(≢data)slideRight ⌽data[⊃⊃coord;]
    (⊃precopy[3])[;2⊃⊃coord]⊢← ⌽(≢data)slideRight ⌽data[;2⊃⊃coord]
    (⊃precopy[4])[⊃⊃coord;]⊢←  (≢data)slideRight  data[⊃⊃coord;]
    _←1
∇ 
⍝ Checks if the layout specified in precopy is a loop
⍝ ⍺ is the direction, ⍵ is the coordinate/position
isLoop←{
    ~inBounds ⍵: 0
    ¯1=(⍺⊃precopy)[⍵]:1
    (1+4|⍺)∇⍵+(⍺⊃dirs)×((⍺⊃precopy)[⍵]⊢←¯1)⊢(⍺⊃precopy)[⍵]
}
⍝ Places a block at coord and check if it forms a loop
∇{createsLoop} ← checkBlock coord
    data[coord]⊢←¯1
    precopy ⊢← precompute
    update coord
    createsLoop ←1 isLoop startCoord
    data[coord]⊢←≢data
∇
⍝ Copies data and dirs so getGuardPath can modify them
dataCopy←data
dirsCopy←dirs
getGuardPath←{
     ⍺←0
     ~inBounds ⍵: ⍺
     dataCopy[⍵]=¯1 : ⍺ ∇ ⍵+(⊃dirsCopy[1])-⊃dirsCopy[4]⊣(dirsCopy⌽⍨←1)
     dataCopy[⍵]= ¯2 : ⍺∇⍵+(⊃dirsCopy[1])
     ((⊃⍺+1),1↓⍺,⍵)∇ ⍵+(⊃dirsCopy[1])⊣(dataCopy[⍵]⊢←¯2)
}
⍝ Finds the path the guard takes, returns
⍝ #of squares visited, sq1, sq2, sq3, ...,sq n
⎕←'Set up complete'
guardPath←getGuardPath startCoord
result1←⊃guardPath
⎕←'Result 1 found'
⍝ part 2 only checks squares visited in part 1
result2←+/+/checkBlock¨⊂¨2↓guardPath
⎕←'Result 2 found'
⎕←'Part 1:',result1
⎕←'Part 2:',result2


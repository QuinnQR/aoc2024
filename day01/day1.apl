#!/usr/local/bin/dyalogscript

⍝ data should be a vector of 1000 elements, each element is an enclosed pair [val1 val2]
data ← {⍵[⍋⍵]}¨↓⍉↑⍎¨⊃⎕NGET './input' 1 

⍝ Zip the data, subtract each pair, find the absolute value, sum over each distance
⎕←'Part 1: ', +/ | -/¨ ↓⍉↑data           

⍝ for each value in the first list (⊃data), count occurences in second list (data[2]) and multiply by itself. Sum over result.
⎕←'Part 2: ', +/ (⊃data)× {+/⍵=⊃data[2]}¨⊃data  


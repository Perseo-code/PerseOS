; Here, we'll enable paging.
; Paging makes reading RAM easier, because if we were to read it 1 byte at a time, it would take a long time, and also the CPU would be 100%.
; Instead, paging measures the RAM in 4KB pages, and when the CPU needs something from the RAM, it would just look it up in the page tables

; Each page has a 12 bit offset. Why?
; Because there is exactly one position for each byte
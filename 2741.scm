(define (solution index n)
  (display index) (newline)
  (cond
	((= index n))
	(else (solution (+ 1 index) n))
	)
  )

(solution 1 (read))

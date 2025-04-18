(define (solve)
  (define input (read))
  (if (eof-object? input)
	'()
	(begin
	  (display (quotient (read) (+ input 1)))
	  (newline)
	  (solve))))

(solve)

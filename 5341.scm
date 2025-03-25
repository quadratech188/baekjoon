(define (solution)
  (let ((n (read)))
	(if (= n 0)
	  '()
	  (begin
		(display (/ (* n (+ n 1)) 2))
		(newline)
		(solution)))))

(solution)

(define (solution)
  (let ((a (read)) (b (read)))
	(if (and (= a 0) (= b 0))
	  0
	  (begin
		(display (+ a b))
		(newline)
		(solution)))))

(solution)

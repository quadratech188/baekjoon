(define (solution n)
  (if (= n 0)
	0
	(begin
	  (display (+ (read) (read)))
	  (newline)
	  (solution (- n 1)))))

(solution (read))

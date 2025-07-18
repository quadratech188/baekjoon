(define (list_input n)
  (if (= n 0)
	'()
	(cons (read) (list_input (- n 1)))))

(define (solution)
  (let (
		(n (read))
		(size_cnts (list_input 6))
		(t (read))
		(p (read))
		)
	(begin
	  (print (apply +
					(map (lambda (x) (ceiling (/ x t))) size_cnts)))
	  (display (quotient n p))
	  (display " ")
	  (display (remainder n p)))))

(solution)


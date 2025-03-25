(define (square x)
  (* x x))

(define (sum-squares n)
  (if (= n 0)
	0
	(+ (square (read)) (sum-squares (- n 1)))))

(display (remainder (sum-squares 5) 10))

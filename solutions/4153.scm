(define (square x) (* x x))

(define (solution)
  (let ((a (read)) (b (read)) (c (read)))
	(if (and (= a 0) (= b 0) (= c 0))
	  '()
	  (begin
		(if (cond
		  ((and (<= a c) (<= b c)) (= (+ (square a) (square b)) (square c)))
		  ((and (<= b a) (<= c a)) (= (+ (square b) (square c)) (square a)))
		  (else (= (+ (square c) (square a)) (square b))))
		  (print "right")
		  (print "wrong"))
	  (solution)))))

(solution)

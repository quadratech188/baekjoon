(define (digitsum n)
  (if (= n 0)
	0
	(+ (remainder n 10) (digitsum (quotient n 10)))))

(define (find-min-rec n)
  (begin
	(define (recurse i)
	  (cond
		((= i n) 0)
		((= (+ i (digitsum i)) n) i)
		(else (recurse (+ i 1)))))
	(recurse 1)))

(display (find-min-rec (read)))

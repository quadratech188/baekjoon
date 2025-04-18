(define (solution n total)
  (if (= n 0)
	total
    (solution (- n 1) (+ total (read)))))

(display (solution 5 0))

(define (displayln contents)
  (display contents) (newline)
  )

(define a (read))
(define b (read))

(if (< a b)
  (displayln "<")
  (if (= a b)
	(displayln "==")
	(displayln ">")
	)
  )

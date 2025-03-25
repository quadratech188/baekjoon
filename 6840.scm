(define (middle a b c)
  (if (< a b)
	(if (< b c)
	  b
	  (if (< a c)
		c
		a))
	(if (< a c)
	  a
	  (if (< b c)
		c
		b))))

(display (middle (read) (read) (read)))

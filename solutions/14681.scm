(define x (read))
(define y (read))

(cond
  ((and (< x 0) (< y 0)) (display 3))
  ((and (> x 0) (< y 0)) (display 4))
  ((and (< x 0) (> y 0)) (display 2))
  ((and (> x 0) (> y 0)) (display 1))
  )

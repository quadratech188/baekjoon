(define n (read))
(define x (read))

(define (solution index)
  (cond 
    [(> index n) 'done]
    [else
     (let ((v (read)))
       (when (< v x)
         (begin
           (display v)
           (display " ")))
       (solution (+ index 1)))]))

(solution 1)

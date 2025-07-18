(import (chicken foreign)) (define fread (foreign-lambda* long () "long long i;scanf(\"%lld\",&i);C_return(i);"))

(let* (
	   (n (fread))
	   (m (fread))
	   (k (fread))
	   (seg-values (make-vector (* 4 n) 0))
	   )
  (define (init-segtree index l r)
	(let
	  ((
		value
		(if (= (- r l) 1)
		  (fread)
		  (let ((center (quotient (+ l r) 2)))
			(+
			  (init-segtree (+ (* 2 index) 1) l center)
			  (init-segtree (+ (* 2 index) 2) center r))))))
	  (vector-set! seg-values index value)
	  value))
  (init-segtree 0 0 n)

  (define (update-segtree value-index l r index new-value)
	(if (= (- r l) 1)
	  (vector-set! seg-values value-index new-value)
	  (let ((left-index (+ (* 2 value-index) 1))
			(right-index (+ (* 2 value-index) 2))
			(center (quotient (+ l r) 2)))
		(begin
		  (if (< index center)
		    (update-segtree left-index l center index new-value)
		    (update-segtree right-index center r index new-value))
		  (vector-set! seg-values value-index (+ (vector-ref seg-values left-index) (vector-ref seg-values right-index)))))))

  (define (query-segtree value-index l r query-l query-r)
	(if (and (<= query-l l) (<= r query-r))
	  (vector-ref seg-values value-index)

	(let ((left-index (+ (* 2 value-index) 1))
		  (right-index (+ (* 2 value-index) 2))
		  (center (quotient (+ l r) 2)))
	  (cond
		((<= query-r center) (query-segtree left-index l center query-l query-r))
		((<= center query-l) (query-segtree right-index center r query-l query-r))
		(else
		  (+ (query-segtree left-index l center query-l query-r)
			 (query-segtree right-index center r query-l query-r)))))))
  
  (do ((i 0 (+ i 1))) ((>= i (+ m k)))
	(if (= 1 (fread))
	  (update-segtree 0 0 n (- (fread) 1) (fread))
	  (print (query-segtree 0 0 n (- (fread) 1) (fread))))))

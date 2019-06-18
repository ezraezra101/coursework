#lang racket
(define-syntax class
  (syntax-rules (initvars method)
    [(class
         (initvars var1 ...) methods ...)
     (lambda (var1 ...)
       (lambda (message)
         (class message methods ...)))]
    [(class message
         (method name (args ...) body) more-stuff ...)
     (cond [(symbol=? message (quote name))
             (lambda (args ...) body)]
           [else
            (class message more-stuff ...)])]
    [(class message)
      (error "nonexistent method")]))


;; Example class
(define arma-class (class
                 (initvars x y)
               (method add-vars (z a)
                       (+ z a x y))))

(define armadillo (arma-class 3 2))



;;; Note: the first class method definition overrides the second
(define class2 (class
                   (initvars x y z)
                 (method stuff () 'yes)
                 (method stuff (x y) 'no)))
(define class2ins (class2 1 2 3))
((class2ins 'stuff)) ;--> 'yes

;;; Way to break this interpretation
(define class3 (class 'x
                   (method x () 'fail)))
(class3) ;--> 'fail
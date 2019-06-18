#lang racket

(define make-dillo-obj
  (lambda (length dead?)
    (lambda message 'longer-than?)
    (lambda (len) (> length len))))

;; A dillo is a (make-dillo number boolean)
(define-struct dillo (length dead?))

;; longer-than? : dillo number -> boolean
;; is dillo longer than given length
(define (longer-than? adillo len)
  (> (dillo-length adillo) len))

;; run-over : dillo -> dillo
;; return dead dillo one unit longer than given dillo
(define (run-over adillo)
  (make-dillo (+ (dillo-length adillo) 1) true))

(define arma (make-dillo 10 false))

(set! arma (run-over arma))
;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname pi_calculator) (read-case-sensitive #t) (teachpacks ((lib "universe.rkt" "teachpack" "2htdp"))) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ((lib "universe.rkt" "teachpack" "2htdp")))))


(define (Pi terms)
  (local [(define (Pi 2n mult terms)
  (if (= terms 0)
      0
      (+ (* mult (/ 12 (+ 2n 1))) (Pi (+ 2n 2) (- mult) (- terms 1)))))]
    (/ (Pi 0 1 terms) 3)))

;(Pi 10000)


(define (Pi2 terms)
  (if (= terms -1)
      0
      (+ (Pi2 (- terms 1)) (pi-term terms))))
(define (pi-term term)
  (local [ (define mult (if (even? term)
                            1
                            -1))]
    (/ (* mult (/ 12 (+ (* 2 term) 1))) 3)))


;(Pi2 4)

;; Returns pi accurate to x decimal points
(define (Pi_dec_acc x)
  (local [(define (check dec term_num)
            (local [(define term (pi-term term_num))]
            (if (> dec (abs term))
                term
                (+ term (check dec (+ term_num 1))))))]
    (check (/ 0.5 (expt 10 x)) 0)))


(Pi_dec_acc 2)

;;Not Quite Equivalent:
(Pi 499)
(Pi2 500)

(require 2htdp/image)

(define WIDTH 2)
(define HEIGHT-MULTIPLIER 16)
(define MAX-RUNS 512)
(define (draw-pi-converge-graph iterations prev)
  (if (>= iterations MAX-RUNS)
      (square 0 'solid 'white)
  (local [(define height (+ (pi-term iterations) prev))]
  (beside/align 'bottom (rectangle WIDTH (* HEIGHT-MULTIPLIER height) 'solid
                                   (if (even? iterations)
                                       (make-color (- 255 (floor (* 256 (/ iterations MAX-RUNS)))) (floor (* 256 (/ iterations MAX-RUNS))) 255 255)
                                       (make-color (floor (* 256 (/ iterations MAX-RUNS))) (- 255 (floor (* 256 (/ iterations MAX-RUNS)))) 255 255)))
                                     
                (draw-pi-converge-graph (+ iterations 1) height)))
  ))

(define pi-bar (rectangle WIDTH (* HEIGHT-MULTIPLIER 3.1415926535897932384626433) 'solid 'black))

(beside/align 'bottom pi-bar (draw-pi-converge-graph 0 0) pi-bar)

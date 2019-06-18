#lang racket

(require 2htdp/image)


(define POINT (square 1 'solid 'blue))

#;#;
(define (graph fn [xmin -10] [xmax 10] [ymin -10] [ymax 10] [res 1] [size 1])
  (overlay (draw-curve fn xmin xmax ymin ymax res size)
           (rectangle (* size (/ (- xmax xmin) res)) (* size (/ (- ymax ymin) res)) 'solid 'white)))

(define (draw-curve fn xcurrent xmax ymin ymax res size)
  (cond [(> xcurrent xmax) (square 0 'solid 'white)]
        [else (beside/align "bottom"
                            (rectangle  size (* size (- (min (max (fn xcurrent) ymin) ymax) ymin))  'solid 'black)
                            (draw-curve fn (+ xcurrent res) xmax ymin ymax res size))]
        ))



;; Graphs a simple function
(define (graph fn [xmin -10] [xmax 10] [ymin -10] [ymax 10] [res 1] [size 1])
  (local [(define height (* (- xmax xmin) size))
          (define width (* (- ymax ymin) size))
          (define point POINT)
          
          (define axis (overlay (rectangle height 1 'solid 'black)
                                (rectangle 1 width 'solid 'black)
                                (rectangle height width 'solid 'white)))
          
          (define draw-curve (lambda (x-current)
                               (cond [(> x-current xmax) axis]
                                     [else
                                      (draw-point x-current (draw-curve (+ x-current res)))])))
          
          (define draw-point (lambda (x-current chart)
                               (let [(y-current (fn x-current))]
                               (cond [(> y-current ymax) chart]
                                     [(< y-current ymin) chart]
                                     [else
                               (underlay/xy chart 
                                            (/ (* (- x-current xmin) size) 1)
                                            (* (- ymax y-current ) size)
                                            point)]))))
          ]
    (draw-curve xmin)))

(scale 5 (graph exp -3 3 -10 10 0.1 5))

;(graph (lambda (x) (* x x)) -3 3 -10 10 0.1 1)






(define (graph-theta fn [xmin 0] [xmax (* 2 pi)] [rmax 1] [res (/ pi 32)] [size 20])
  (local [(define height (* (* rmax 2) size))
          (define width (* (* rmax 2) size))
          (define point POINT)
          
          (define (make-x r theta)
            (* r (cos theta)))
          (define (make-y r theta)
            (* r (sin theta)))
          
          (define axis (overlay (rectangle height 1 'solid 'black)
                                (rectangle 1 width 'solid 'black)
                                (rectangle height width 'solid 'white)))
          
          (define draw-curve (lambda (x-current)
                               (cond [(> x-current xmax) axis]
                                     [else
                                      (draw-point x-current (draw-curve (+ x-current res)))])))
          
          (define draw-point (lambda (x-current chart)
                               (let [(fn-output (fn x-current))]
                                 (cond [(> fn-output rmax) chart]
                                       [(< fn-output (- rmax)) chart]
                                       [else
                                        (underlay/xy chart 
                                                     (/ (* (+ (make-x  fn-output x-current) rmax) size) 1)
                                                     (* (- rmax (make-y fn-output x-current)) size)
                                                     point)]))))
          ]
    (draw-curve xmin)))



(define (convert-to-cartesian r theta)
  (list (* r (cos theta)) (* r (sin theta))))

(scale 5 (graph-theta identity))
(scale 5 (graph-theta cos))
(scale 5 (graph-theta (lambda (x) (+ (/ (cos x) 2) 0.5))))





;; Returns the answer to life, the universe, and everything
(define-syntax-rule (the-answer stuff ...)
  42)
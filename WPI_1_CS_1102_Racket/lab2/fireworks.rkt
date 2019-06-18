;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname fireworks) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
(require 2htdp/image)
(require 2htdp/universe)

"Ezra Davis & Zhijie Wang"


;; Constant Definitions

(define DY 3)
(define DX 3)
(define FUSE 80)
(define FIREWORK-RADIUS 2)
(define EXPLODE-RADIUS 50)
(define EXPLODE-DURATION 5)

(define HEIGHT 600)
(define WIDTH 800)

(define BACKGROUND (empty-scene WIDTH HEIGHT "black"))

;; Data definitions

(define-struct firework (x y dx dy fuse color))
;; Firework is (make-firework Number Number Number Number Number)
;; interp.
;; a firework at position x, y,
;; moving dx pixels in the x direction and dy in the y direction each tick
;; Fuse is a countdown and the firework will go off when (= fuse 0)
(define FIREWORK1 (make-firework 30 40 2 5 40 (make-color 30 60 255 255)))
(define FIREWORK2 (make-firework 70 50 2 5 40 'blue))

#;
(define (fn-for-firework f)
  (... (firework-x f)
       (firework-y f)
       (firework-dx f)
       (firework-dy f)
       (firework-fuse f)
       (firework-color f)
       ))
;; Template rules used:
;; - compound: 5 fields
;; - atomic non-distinct: x field is Number
;; - atomic non-distinct: y field is Number
;; - atomic non-distinct: dx field is Number
;; - atomic non-distinct: dy field is Number
;; - atomic non-distinct: fuse field is Number
;; - atomic non-distinct: color field is Color


;; World State is List of Fireworks which is one of:
;;  - empty
;;  - (cons Firework World-State)
;; interp. a list of fireworks
(define WS-1 empty)
(define WS-2 (cons (make-firework 70 50 2 5 40 'blue) empty))
(define WS-3 (cons (make-firework 49 27 4 2 40 'green) (cons (make-firework 17 500 1 1 50 'purple) empty)))
#;
(define (fn-for-ws ws)
  (cond [(empty? ws) (...)] ;BASE CASE
        [else
         (... (firework-x (first ws))
              (firework-y (first ws))
              (firework-dx (first ws))
              (firework-dy (first ws))
              (firework-fuse (first ws))
              (firework-color (first ws)) 
              (fn-for-ws (rest ws)))])) ;NATURAL RECURSION
;;             /
;;            /
;;       COMBINATION
;; Template rules used:
;; -oneof:2cases
;; - atomic distinct: empty
;; - compound: (cons Firework World-State)
;; - compound: (first ws) is Firework ;; - self-reference: (rest ws) is World-State


;; A good value to start with is empty, which is a World-State

(define (main ws)
  (big-bang ws
            (on-tick move-fireworks)
            (to-draw render)
            (on-mouse handle-mouse)))

;;Move Firework
;; World-State -> World-State 
;; Move the fireworks decrease the fuses.
(check-expect (move-fireworks WS-1) empty)
(check-expect (move-fireworks WS-2) (cons (make-firework (+ 70 2) (- 50 5) 2 5 (- 40 1) 'blue) empty))
(check-expect (move-fireworks WS-3) (cons (make-firework (+ 49 4) (- 27 2) 4 2 (- 40 1) 'green) (cons (make-firework (+ 17 1) (- 500 1) 1 1 (- 50 1) 'purple) empty)))
(check-expect (move-fireworks (cons (make-firework 70 50 2 5 (- EXPLODE-DURATION) 'blue) empty)) empty)


(define (move-fireworks ws) 
  (cond [(empty? ws) empty] ;BASE CASE
        [(>= (- EXPLODE-DURATION) (firework-fuse (first ws))) (move-fireworks (rest ws))] ;; Drops the first firework if its exploded for long enough
        [else
         (cons (make-firework (+ (firework-x (first ws)) (firework-dx (first ws))) ;;Changes firework values appropriately for tick
               (- (firework-y (first ws))(firework-dy (first ws)))
               (firework-dx (first ws))
               (firework-dy (first ws))
               (- (firework-fuse (first ws)) 1)
               (firework-color (first ws))) 
               (move-fireworks (rest ws)))])) ;NATURAL RECURSION
  
;; Creates a firework where the mouse is clicked
;; World-State Number Number MouseEvent -> World-State
(check-expect(= 45 (firework-x (first (handle-mouse WS-1 45 10 "button-down")))) true)
(check-expect (= 10 (firework-y (first (handle-mouse WS-1 45 10 "button-down")))) true)
(check-expect (handle-mouse WS-3 45 10 "move") WS-3)
(define (handle-mouse ws x y me)
  (cond [(mouse=? me "button-down") (cons (create-firework x y) ws)]
        [else
         ws]))

;; Renders the Fireworks on a black background
; World-State -> Image
;!!!
(check-expect (render WS-1) BACKGROUND)
(check-expect (render WS-2) (place-image (circle FIREWORK-RADIUS 'solid 'blue) 70 50 BACKGROUND))
(check-expect (render (cons (make-firework 70 50 2 5 (- 1) 'blue) empty)) (place-image (circle EXPLODE-RADIUS 'solid 'blue) 70 50 BACKGROUND))

(define (render ws)
  (cond [(empty? ws) BACKGROUND] ;BASE CASE
        [else
         (place-image (circle (if (<= (firework-fuse (first ws)) 0) ;Image for place image
                                  EXPLODE-RADIUS
                                  FIREWORK-RADIUS) ; size of circle
                              "solid" ; the circle is filled
                              (firework-color (first ws))) ; color of circle
                      (firework-x (first ws)) ;x value of firework
                      (firework-y (first ws)) ;y value of firework
                      (render (rest ws)))])) ;NATURAL RECURSION
;Creates a firework with the correct x and y
; Number Number -> Firework
(check-expect (firework? (create-firework 15 20)) true)
(check-expect (firework-x (create-firework 47 71209384)) 47)
(check-expect (firework-y (create-firework 1240 23)) 23)
(check-expect (<= 1 (firework-dy (create-firework 90832047 71209384)) (+ DY 1)) true)
(check-expect (<= (/ DX -2) (firework-dx (create-firework 90832047 71209384)) (/ DX 2)) true)
(check-expect (color? (firework-color (create-firework 1240 23))) true)
;(check-expect (create-firework 50 60) (make-firework 50 60 (... DX) (... DY) (... FUSE)))
(define (create-firework x y)
  (make-firework x y (- (random-float DX) (/ DX 2)) (+ (random-float DY) 1) (+ (random-float FUSE) 1) (make-color (random 256) (random 256) (random 256) 255)))


;; Returns a random float of precision 0.001*Number that is less than (or equal to) number
;; Number -> Float
(check-expect (<= (random-float 50) 50) true)
(check-expect (>= (random-float 50) 0) true)
(define (random-float num)
  (* (/ (random 1001) 1000) num))

(main empty)
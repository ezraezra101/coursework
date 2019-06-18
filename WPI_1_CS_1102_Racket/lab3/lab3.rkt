;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname lab3) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
;Ezra Davis

;;Contents of File: ;!!! no longer relevant

;;Data Definitions:
;Widget
;    Widget templates
;Order
;    Order template
;Reply
;    Reply template

;;Functions:
;find-widget-longest-name-original
;find-widget-most-quantitiy-old
;find (meta function)
;find-widget-longest-name
;find-widget-most-quantity
;fulfill-order-immediately
;how-many-widgets-assembled
;how-long-to-produce
;fulfill-order

;;;;;;;;;;;;;;;; Data Definitions:
;; Widget
;; Widget is (make-widget String, Natural, Number, Number[0,∞), (ListOf Widget )
;; interp. Widgets have a name, the number of widgets, a price, the time required to assemble it, and the subcomponents


(define-struct widget (name number price time materials) )

(define metal (make-widget "Metal" 100 0.2 0.05 empty))
(define bolt (make-widget "Bolt" 101 0.5 0.1 (list metal)))
(define nut (make-widget "Nut" 75 0.4 0.08 (list metal)))
(define chassis (make-widget "Chassis" 40 49.99 3 (list nut bolt metal)))
(define fastener (make-widget "Fastener" 50 1 0.3 (list bolt nut)))
(define product (make-widget "Product" 20 100 4 (list bolt nut fastener chassis)))
(define warehouse (make-widget "Warehouse" 0 0 0 (list product)))

(define bolt2 (make-widget "Bolt2" 0 0.5 0.1 (list metal)))
(define metal2 (make-widget "Metal2" 0 0.1 0.1 empty))
(define bolt3 (make-widget "Bolt3" 0 0.5 0.1 (list metal2)))

;; Widget -> ????
#;
(define (fn-for-widget w)
  (... (widget-name w)
       (widget-number w)
       (widget-price w)
       (widget-time w)
       (fn-for-low (widget-materials w))
       ))
;; (ListOf widget) -> ?????
#;
(define (fn-for-low low)
  (cond [(empty? low) (...)]
        [else
         (...
          (fn-for-widget (first low))
          (fn-for-low (rest low))
          )]))



;;Order
;; Order is (make-order String Natural Time)
;; interp. An order has a name of the desired widget (String), a quantity of things ordered, and the time the client is willing to wait
(define-struct order (name number time))

(define bolt-order (make-order "Bolt" 10 0))
(define metal-order (make-order "Metal" 1 1))
(define fastener-order (make-order "Fastener" 101 0.4))

;; Order -> ????
#;
(define (fn-for-order o)
  (... (order-name o)
       (order-number o)
       (order-time o)
       ))

;;Reply
;; Reply is (make-reply Number cost time)
;; interp. A reply has a quantity of the widgets providable, cost, and how long it will take to complete
(define-struct reply (number price time))

(define reply-to-bolt (make-reply (widget-number bolt) (* (order-number bolt-order) (widget-price bolt)) 0))

;; Reply -> ???
#;
(define (fn-for-reply r)
  (... (reply-number r)
       (reply-price r)
       (reply-time r)
       ))


;Producable is (make-producable (Natural Number)
(define-struct producable (number time))
;interp. a producable has the time it takes to make one of a given number of widgets

;; Producable -> ???
#;
(define (fn-for-producable p)
  (...
   (producable-time p)
   (producable-number p)
   ))

;(ListOf Producable)
#;
(define (fn-for-lop lop)
  (cond
    [(empty? lop) (...)]
    [else (...
           (fn-for-producable (first lop))
           (fn-for-lop (rest lop)))]))




;;;;;;;;;;;;;;; Functions:



;;Widget -> Widget
;;Returns the widget with the longest name out of the main widget and all of its subwidgets (recursively)
(check-expect (find-widget-longest-name-original metal) metal)
(check-expect (find-widget-longest-name-original chassis) chassis)
(check-expect (find-widget-longest-name-original bolt) metal)
(check-expect (find-widget-longest-name-original product) fastener)

(define (find-widget-longest-name-original w)
  (local
    [
     (define empty-widget (make-widget "" 0 0 0 empty))
     (define (find-widget-longest-name--widget w)
       (local
         [(define sub-widgets-longest-name (find-widget-longest-name--low (widget-materials w)))]
         (if (> (string-length (widget-name w)) (string-length (widget-name sub-widgets-longest-name)))
             w
             sub-widgets-longest-name
             )))
     (define (find-widget-longest-name--low low)
       
       (cond [(empty? low) empty-widget]
             [else
              (local [(define rest-ans (find-widget-longest-name--low (rest low)))
                      (define first-ans (find-widget-longest-name--widget (first low)))]
                (if (> (string-length (widget-name first-ans)) (string-length (widget-name rest-ans)))
                    first-ans
                    rest-ans
                    ))]))
     ]
    (find-widget-longest-name--widget w)))


;;Widget -> Widget
;;Returns the widget with the highest quantity out of the main widget and all of its subwidgets (recursively)
(check-expect (find-widget-most-quantitiy-old metal) metal)
(check-expect (find-widget-most-quantitiy-old chassis) bolt)
(check-expect (find-widget-most-quantitiy-old bolt) bolt)
(check-expect (find-widget-most-quantitiy-old product) bolt)

(define (find-widget-most-quantitiy-old w)
  (local
    [
     (define empty-widget (make-widget "" 0 0 0 empty))
     (define (find--widget w)
       (local
         [(define sub-widgets-largest (find--low (widget-materials w)))]
         (if (> (widget-number w) (widget-number sub-widgets-largest))
             w
             sub-widgets-largest
             )))
     (define (find--low low)
       
       (cond [(empty? low) empty-widget]
             [else
              (local [(define rest-ans (find--low (rest low)))
                      (define first-ans (find--widget (first low)))]
                (if (> (widget-number first-ans) (widget-number rest-ans))
                    first-ans
                    rest-ans
                    ))]))
     ]
    (find--widget w)))



;;( Widget -> Boolean) Widget -> Widget
;;Returns the widget with the highest quantity out of the main widget and all of its subwidgets (recursively)
(check-expect (find widget-number metal) metal) ;Trivial case
(check-expect (find widget-price chassis) chassis) ; Self case
(check-expect (find widget-time bolt) bolt) ; Extra case
(check-expect (find widget-number product) bolt) ; Non-self case

(define (find funct w)
  (local
    [
     (define empty-widget (make-widget "" 0 0 0 empty))
     (define (find--widget w)
       (local
         [(define sub-widgets-largest (find--low (widget-materials w)))]
         (if (> (funct w) (funct sub-widgets-largest))
             w
             sub-widgets-largest
             )))
     (define (find--low low)
       
       (cond [(empty? low) empty-widget]
             [else
              (local [(define rest-ans (find--low (rest low)))
                      (define first-ans (find--widget (first low)))]
                (if (> (funct first-ans) (funct rest-ans))
                    first-ans
                    rest-ans
                    ))]))
     ]
    (find--widget w)))





;;Widget -> Widget
;;Returns the widget with the longest name out of the main widget and all of its subwidgets (recursively)
(check-expect (find-widget-longest-name metal) metal)
(check-expect (find-widget-longest-name chassis) chassis)
(check-expect (find-widget-longest-name bolt) metal)
(check-expect (find-widget-longest-name product) fastener)

(define (find-widget-longest-name w)
  (local [(define (name-length w) (string-length (widget-name w)))]
    (find name-length w)))

;;Widget -> Widget
;;Returns the widget with the highest quantity out of the main widget and all of its subwidgets (recursively)
(check-expect (find-widget-most-quantity metal) metal)
(check-expect (find-widget-most-quantity chassis) bolt)
(check-expect (find-widget-most-quantity bolt) bolt)
(check-expect (find-widget-most-quantity product) bolt)

(define (find-widget-most-quantity w)
  (find widget-number w))

;; Order Widget -> Reply
;;Computes a reply based on an order (widget isn't necessarily ordered widget, but should include it)
;; the time of the reply is 0, the number is the number in stock, and the price is the order-number * widget-price


(check-expect (fulfill-order-immediately bolt-order warehouse) reply-to-bolt) ;
(check-expect (fulfill-order-immediately metal-order warehouse) (make-reply 100 0.2 0))
;#!!!!! ; more check-expects

(define (fulfill-order-immediately o wares)
  (local [
          
          (define w (find-widget-by-name wares (order-name o)))
          ]
    (make-reply
     (widget-number w)
     (* (order-number o) (widget-price w))
     0
     )))

;;Order Widget -> Natural
;; Returns the number of widgets that can be assembled (based on the widget number and assembleable child widgets)
;; (widget isn't necessarily ordered widget, but should include it)
(check-expect (how-many-widgets-assembled metal-order warehouse) 0) ;trivial
(check-expect (how-many-widgets-assembled bolt-order warehouse) 100) ;simple
(check-expect (how-many-widgets-assembled fastener-order warehouse) (+ (widget-number nut) (how-many-widgets-assembled bolt-order warehouse))) ;75 + 100 ;non-trivial

(define (how-many-widgets-assembled o wares)
  (local [
          
          (define w (find-widget-by-name wares (order-name o)))
          ;; Widget -> Natural
          (define (assembleable--widget w)
            (if (empty? (widget-materials w))
                (widget-number w)
                (+
                 (widget-number w)
                 (assembleable--low (widget-materials w))
                 )))
          (define (min x y) ;If normal min is passed an inexact (e.g. +inf.f), it produces an inexact
            (if (> x y)
                y
                x))
          ;; (ListOf Widget) -> Natural
          (define (assembleable--low low)
            (cond [(empty? low) +inf.0];+inf.f]
                  [else
                   (min
                    (assembleable--widget (first low))
                    (assembleable--low (rest low))
                    )]))
          ]
    (if (empty? (widget-materials w))
        0
        (assembleable--low (widget-materials w)))
    ))



;Widget -> Natural
; This function determines how long it will take to produce a new widget in a slightly innacurate fashion
(check-expect (how-long-to-produce metal) (widget-time metal)) ;trivial
(check-expect (how-long-to-produce bolt) (+ (widget-time bolt) (widget-time metal))) ;not too complex
(check-expect (how-long-to-produce nut) (+ (widget-time nut) (widget-time metal)))
(check-expect (how-long-to-produce fastener) (+ (widget-time fastener) (max (how-long-to-produce nut) (how-long-to-produce bolt))))
(check-expect (how-long-to-produce chassis) (+ (widget-time chassis) (how-long-to-produce bolt))) ;Non trivial (bolt has the largest time to produce of chassis' req. materials
(define (how-long-to-produce w)
  (local [
          ;; Widget -> Number
          (define (--widget w)
            (if (empty? (widget-materials w))
                (widget-time w)
                (+
                 (widget-time w)
                 (--low (widget-materials w))
                 )))
          
          ;; (ListOf Widget) -> Number
          (define (--low low)
            (cond [(empty? low) 0]
                  [else
                   (max
                    (--widget (first low))
                    (--low (rest low))
                    )]))
          ]
    (--widget w)
    ))



;; Hack to make find able to use (string=? as input
;; Widget String -> (Widget -> Natural [0 1])
(check-expect (find-widget-by-name warehouse "Bolt") bolt)
(define (find-widget-by-name w name)
  (local [ 
          (define (eq-string-hack widget)
            (if (string=? name (widget-name widget))
                1
                0))
          ]
    (find eq-string-hack w)
    ))


(check-expect (order-iterate (make-order "Metal" 0 0) warehouse) (make-reply 0 0 0))
(check-expect (order-iterate bolt-order warehouse) (make-reply 10 (* 10 0.5) 0)) ;'simplest' case


;; Order Widget -> Reply
(define (order-iterate order wares)
  (local
    [ 
     (define w (find-widget-by-name wares (order-name order) ))
     (define time-to-make-one (how-long-to-produce2 w))
     ]
    (cond
      
      [(or (<= (order-number order) 0) ;;If no more can/need-to-be shipped
           (< (- (order-time order) time-to-make-one) 0))
       (make-reply 0 0 0)]
      
      [(<= (widget-number w) 0) ;; Constructing more widgets
       (local [(define reply-vals (order-iterate (make-order (order-name order) (order-number order) (- (order-time order) time-to-make-one)) ;;;!!!!!!!
                      (construct-widget w)))]
         (make-reply (+ (reply-number reply-vals) 0)
                     (+ (reply-price reply-vals) 0)
                     (+ (reply-time reply-vals) time-to-make-one)))]
      [else
       (local [ (define time-to-make-one-now (how-long-to-produce2 w))
               (define reply-vals
                 (order-iterate (make-order (order-name order)
                                            (- (order-number order) 1)
                                            (- (order-time order) time-to-make-one));; Should work! :-(
                                (make-widget (widget-name w)
                                             (- (widget-number w) 1)
                                             (widget-price w)
                                             (widget-time w)
                                             (widget-materials w))))];(name number price time materials) )
         
         
         (make-reply (+ (reply-number reply-vals) 1)
                     (+ (reply-price reply-vals) (widget-price w))
                     (+ time-to-make-one-now (reply-time reply-vals))) ;time-to-make-one
         )]
      )))

;; Widget -> Widget
;Returns a widget that has one more constructed widget, and thus, fewer materials
(check-expect (construct-widget metal) (make-widget "Metal" (+ 100 1) 0.2 0.05 empty)) ;Simple case
(check-expect (construct-widget bolt) (make-widget "Bolt" (+ 101 1) 0.5 0.1 (list (make-widget "Metal" (- 100 1) 0.2 0.05 empty)))) ;One deep
(check-expect (construct-widget product) (make-widget "Product" (+ 20 1) 100 4 (list
                                                                                (make-widget "Bolt" (- 101 1) 0.5 0.1 (list metal))
                                                                                (make-widget "Nut" (- 75 1) 0.4 0.08 (list metal))
                                                                                (make-widget "Fastener" (- 50 1) 1 0.3 (list bolt nut))
                                                                                (make-widget "Chassis" (- 40 1) 49.99 3 (list nut bolt metal))) ;complex case
                                                      ))

(check-expect (construct-widget bolt3) (make-widget "Bolt3" (+ 0 1) 0.5 0.1 (list metal2))) ;; Testing recursive creation of widget-materials
(define (construct-widget w)
  (local [
          ;; Widget -> Widget
          (define (--widget w)
            (if (> (widget-number w) 0)
                (make-widget (widget-name w)
                             (- (widget-number w) 1)
                             (widget-price w)
                             (widget-time w)
                             (widget-materials w))
                (--widget (construct-widget w))
                ))
          
          ;; (ListOf Widget) -> (ListOf Widget)
          (define (--low low)
            (cond [(empty? low) empty]
                  [else
                   (cons (--widget (first low)) (--low (rest low)))
                   ]))
          ]
    (make-widget (widget-name w)
                 (+ (widget-number w) 1)
                 (widget-price w)
                 (widget-time w)
                 (--low (widget-materials w)))
    ))



;; Widget -> Number
;; Determines the time to produce a widget (meaning ship it or make it)
(check-expect (how-long-to-produce2 bolt) 0)
(check-expect (how-long-to-produce2 bolt2) (+ 0.1 0))
(check-expect (how-long-to-produce2 bolt3) (+ 0.1 0.1))

(define (how-long-to-produce2 w)
  (local [
          ;; Widget -> Number
          (define (--widget w)
            (if (> (widget-number w) 0)
                0
                (if (empty? (widget-materials w))
                    (widget-time w)
                    (+
                     (widget-time w)
                     (--low (widget-materials w))
                     ))))
          
          ;; (ListOf Widget) -> Number
          (define (--low low)
            (cond [(empty? low) 0]
                  [else
                   (max
                    (--widget (first low))
                    (--low (rest low))
                    )]))
          ]
    (--widget w)
    ))








;Order -> Reply
;!!! (widget isn't necessarily ordered widget, but should include it)
(check-expect (fulfill-order bolt-order warehouse) (make-reply 10 (* 10 0.5) 0)) ;'simplest' case
(check-expect (fulfill-order (make-order "Metal2" 1 0) metal2) (make-reply 0 0 0)) ;;Checking for no time alloted with construction needed 
(check-expect (fulfill-order bolt-order warehouse) (make-reply (order-number bolt-order) (* (order-number bolt-order) (widget-price bolt)) 0));same as last test
(check-expect (fulfill-order fastener-order warehouse)
              (make-reply 51
                          (* 51 (widget-price fastener))
                          (* 1 (how-long-to-produce2 (make-widget "Fastener" 0 1 0.3 (list bolt nut)))))) ;;Only one more fastener can be made in 0.4 time-units
(check-expect (fulfill-order (make-order (widget-name bolt) 10 20) warehouse) (make-reply 10 (* 10 (widget-price bolt)) 0))
(check-expect (fulfill-order (make-order "Bolt3" 1 150) bolt3) (make-reply 1 (* 1 (widget-price bolt3)) (* 1 (+ (widget-time bolt3) (widget-time metal2)))))
(check-expect (fulfill-order (make-order "Bolt2" 100 150) bolt2) (make-reply 100 (* 100 (widget-price bolt2)) (* 100 (+ (widget-time bolt2) 0))));(widget-time metal) <- There is enough metal already


;!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
(define (fulfill-order o wares) (order-iterate o wares))
#;
(define (fulfill-order o wares) (make-reply 0 0 0));Stub
#;
(define (fulfill-order o wares)
  (local
    [
     (define w (find-widget-by-name wares (order-name o)))
     (define time-to-produce-w (how-long-to-produce w))
     
     (define number-has (widget-number w))
     (define number-needs (if (>= number-has (order-number o))
                              0
                              (- number-has (order-number o))))
     (define number-assembleable (how-many-widgets-assembled o wares))
     
     
     ;!!! wrong ;finds nearest whole number of widgets
     (define required-time (* time-to-produce-w  (floor(/ time-to-produce-w (order-time o))))) ;maximum amount of time producing widgets
     
     (define time- (if (> required-time (order-time o))
                       required-time
                       (* time-to-produce-w number-needs)))
     ;!!!
     (define number (+ (/ time- time-to-produce-w)
                       (if (> number-has (order-number o))
                           (order-number o)
                           number-has)))
     ]
    (make-reply
     number
     (* number (widget-price w)) ;price 
     time-
     )))

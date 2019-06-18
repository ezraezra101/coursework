;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-abbr-reader.ss" "lang")((modname mutual-recursion) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
(define-struct elt (name data subs))
;; Element is (make-elt String Integer ListOfElement)
;; interp. an element in a 'file system', with name and EITHER data or subs.
;;    If data is 0, than subs is considered to be a list of sub elements
;;    If data is not 0, than subs is ignored

;; ListOfElement is one of:
;; - empty
;; - (cons Element ListOfElement)
;; interp. a list of file system Elements


(define F1 (make-elt "F1" 1 empty))
(define F2 (make-elt "F2" 2 empty))
(define F3 (make-elt "F3" 3 empty))
(define D4 (make-elt "D4" 0 (list F1 F2)))
(define D5 (make-elt "D5" 0 (list F3)))
(define D6 (make-elt "D6" 0 (list D4 D5)))


#;
(define (fn-for-element e)
  (... (elt-name e) ; String
       (elt-data e) ;Integer
       (fn-for-loe (elt-subs e))
       ))
#;
(define (fn-for-loe loe)
  (cond [(empty?) (...)]
        [else
         (... (fn-for-element (first loe))
              (fn-for-loe (rest loe)))
         ]))



;;Sum all the file data in tree

;; Element -> Integer
;; ListOfElement -> Integer
;; produce the some of all the data in element (and its subs)

(check-expect (sum-data--element F1) 1)
(check-expect (sum-data--loe empty) 0)
(check-expect (sum-data--element D5) 3)
(check-expect (sum-data--element D4) 3)
(check-expect (sum-data--element D6) 6)

;(define (sum-data--element e) 0)
;(define (sum-data--loe loe) 0)

(define (sum-data--element e)
  ;(elt-name e) ; String
  (if (zero? (elt-data e))
      (sum-data--loe (elt-subs e))
      (elt-data e) ;Integer
      ))

(define (sum-data--loe loe)
  (cond [(empty? loe) 0]
        [else
         (+ (sum-data--element (first loe))
            (sum-data--loe (rest loe)) )
         ]))


;; Returns the list of all element names in input
;; Element -> ListOfString
;; LOE -> ListOfString
(check-expect (name-list--element F1) (list "F1"))
(check-expect (name-list--element D6) (list "D6" "D4" "F1" "F2" "D5" "F3"))

(define (name-list--element e)
  (cons
   (elt-name e)
   (name-list--loe (elt-subs e))
   )
  )

(define (name-list--loe loe)
  (cond [(empty? loe) empty]
        [else
         (append (name-list--element (first loe))
                 (name-list--loe (rest loe)))
         ]))








;; Looks for data element with given name and returns it, else produces false
;; String Element -> Integer or false
;; String LOE -> Integer or false

(check-expect (find--loe "F3" empty) false)
(check-expect (find--element "F3" F1) false)
(check-expect (find--element "F3" F3) 3)
(check-expect (find--element "F3" F1) false)
(check-expect (find--element "F3" D6) 3)
(check-expect (find--element "D4" D4) 0)

;(define (find--element n e) false)
;(define (find--loe n loe) false)


(define (find--element n e)
  (if (string=? (elt-name e) n) ; String
      (elt-data e) ;Integer
      (find--loe n (elt-subs e))
      ))

(define (find--loe n loe)
  (cond [(empty? loe) false]
        [else
         (if (not (false? (find--element n (first loe)))) ; is it found in First loe?
             (find--element n (first loe))
             (find--loe n (rest loe)))
         ]))









;; Lecture example



(define-struct anode (name quantity children))
;; Name is String
;; quantity is Number
;; children is ListOfAnode


#;
(define (fn-for-anode node)
  (...(anode-quantity node) ;; Natural
      (anode-name node) ;; String
      (fn-for-alon (anode-children node)))) ;; ListOfNode

#;
(define (fn-for-alon alon)
  (cond
    [(empty? alon) (...)]
    [else
     (...
      (fn-for-anode (first alon)) ;; node
      (fn-for-alon (rest alon)))])) ;;ListOfNode




(define node1 (make-anode "1" 1 empty))
(define node2 (make-anode "2" 2 empty))
(define node3 (make-anode "3" 3 (list node1)))
(define node4 (make-anode "4" 4 (list node2 node3)))


;;Takes anode -> ListOfString
;; Returns a list of all the names of the children of anode
(check-expect (names-of--anode node1) (cons "1" empty))
(check-expect (names-of--anode node3) (list "3" "1"))
(check-expect (names-of--anode node4) (list "4" "2" "3" "1"))

(define (names-of--anode node)
  (cons
   (anode-name node) ;; String
   (names-of--alon (anode-children node)))) ;; ListOfNode


;; ListOfNode -> String
(define (names-of--alon alon)
  (cond
    [(empty? alon) empty]
    [else
     (append
      (names-of--anode (first alon)) ;; node
      (names-of--alon (rest alon)))])) ;;ListOfNode


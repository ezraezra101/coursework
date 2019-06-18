;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname sujiken-units) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
(require racket/list) ;gets list-ref, take and drop

;; Natural Natural --> (listOf Number)
;; creates a list of given length + 1 starting at (+ 1 given-number)
(check-expect (make-row 55 5) (list 56 57 58 59 60 61))
(define (make-row start num)
  (local [(define (row-helper x)
            (+ start x ))]
    (map row-helper (build-list (add1 num) add1))))

;; Natural --> (ListOf (ListOf Number))
;; Creates all the rows for sujiken
(check-expect (make-rows 0) (list (list 0)))
(check-expect (make-rows 2) (list (list 3 4 5) (list 1 2) (list 0)))
(define (make-rows rows)
  (cond [(< rows 1) (list (list 0))]
        [else
         (local [(define prev-rows (make-rows (- rows 1)))]
           (cons (make-row (last (first prev-rows)) rows) prev-rows) 
           )]))



(define sujiken-rows (make-rows 8))

(define sujiken-columns '(
                          '(36 28 21 15 10 6 3 1 0)
                          '(37 29 22 16 11 7 4 2)
                          '(38 30 23 17 12 8 5)
                          '(39 31 24 18 13 9)
                          '(40 32 25 19 14)
                          '(41 33 26 20)
                          '(42 34 27)
                          '(43 35)
                          '(44)))

(define sujiken-diagonal-down-right '(
                               '(44 35 27 20 14 9 5 2 0)
                               '(43 34 26 19 13 8 4 1)
                               '(42 33 25 18 12 7 3)
                               '(41 32 24 17 11 6)
                               '(40 31 23 16 10)
                               '(39 30 22 15)
                               '(38 29 21)
                               '(37 28)
                               '(36)))
(define sujiken-diagonal-down-left '(
                               '(0)
                               '(1)
                               '(3 2)
                               '(6 4)
                               '(10 7 5)
                               '(15 11 8)
                               '(21 16 12 9)
                               '(28 22 17 13)
                               '(36 29 23 18 14)
                               '(37 30 24 19)
                               '(38 31 25 20)
                               '(39 32 26)
                               '(40 33 27)
                               '(41 34)
                               '(42 35)
                               '(43)
                               '(44)))
(define sujiken-units (append sujiken-rows sujiken-columns sujiken-diagonal-down-right sujiken-diagonal-down-left))
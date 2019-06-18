;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-advanced-reader.ss" "lang")((modname star-sudoku-units) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #t #t none #f ())))
;;Star Sudoku

;;Example:

(define B 'blank)

(list 'O 'T 'Tr 'F 'Fi 'Si 'S 'E 'N)

(define STARBD1
(list      'O
          
         B  B  B  B  B  B  B  'F
         
      B 'S  B  B  B 'T  B  B  'O
      
'O B  B  B  B           B 'Si B 'S

   B  B 'Tr B           B  B  B  B  'F
   
      B  B 'Si B  B  B  B 'E  B
      
  B 'E  B  B  'F  'Fi  B  'Tr
                       B))




(define star-rows (list
     ;   (list       0) ;;Unnecessary
         
         (list     1  2  3  4  5  6  7  8)
         
         (list   9  10 11 12  13 14 15 16 17)
         
      (list 18 19 20 21 22        23 24 25 26)
      
      (list  27 28 29 30        31 32 33 34 35)
      
      (list   36 37 38 39 40 41 42 43 44)
      
      (list  45 46 47 48 49 50 51 52)
      
   ;  (list                    53) ;;Unnecessary
      ))
(define star-diagonal-down-left (list
         ;(list 18)
         (list 0    1 2    9 10   19 20   27)
         (list      3 4    11 12  21 22   28 29  36)
         (list      5 6    13 14          30     37 38  45 46)
         (list      7 8    15 16  23             39 40  47 48)
         (list             17     24 25   31 32  41 42  49 50)
         (list                    26 27   33 34  43 44  51 52)
   ;     (list 53) ;Unnecessary
         
         ))
(define star-diagonal-down-right (list
  ; (list 18) ;Unnecessary
  (list 0    1  2    9  10  19 20  27)
  (list      3  4    11 12  21 22  28 29  36)
  (list      5  6    13 14         30     37 38  45 46)
  (list      7  8    15 16  23            39 40  47 48)
  (list              17     24 25  31 32  41 42  49 50)
  (list                     26     33 34  43 44  51 52   53)
 ;(list                            35) ;Unnecessary
        ))

(define star-boxes (list
    (list 0  1 2 3      9 10 11 12 13)
    (list    4 5 6 7 8  14 15 16       23)
    (list                              18 19 20 21 22  27 28 29        36)
    (list               17             24 25 26        31 32 33 34 35)
    ;;    Should start out by 36, but moved over to preserve sanity
    (list 30  37 38 39           45 46 47 48 49)
    (list     40 41 42 43 44          50 51 52      53)
    ))



(check-expect (length (foldr append empty star-rows)) (- 54 2))
(check-expect (length (foldr append empty star-diagonal-down-left)) (- 54 2))
(check-expect (length (foldr append empty star-diagonal-down-right)) (- 54 2))
(check-expect (length (foldr append empty star-boxes)) 54)

(define STAR-UNITS (append star-rows star-diagonal-down-left star-diagonal-down-right star-boxes)
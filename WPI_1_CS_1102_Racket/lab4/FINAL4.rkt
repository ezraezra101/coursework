;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-intermediate-reader.ss" "lang")((modname FINAL4) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
(require 2htdp/image)
(require 2htdp/universe)
;; Connect 4 program - Ezra Davis and Ivan Melnikov
;; do not modify the constants, except for experimentation with ROWS and COLUMNS
(define RED 1) 
(define BLACK 2)
(define BLANK 0)
(define ROWS 9)    ;; can vary
(define COLUMNS 8) ;; can vary
(define DEPTH 3)

;; ListOfMove state Move Number--> Move
;; This function examines each move in the list, and returns
;; the move that gets the lowest value on evaluation-function.
;; Number is best move value - is infinity until replacement.
(check-expect (find-best-move (legal-next-moves (make-world-state
                                                 (list
                                                  (list 0 0 0 0 0 0 0 0 0)
                                                  (list 0 0 0 0 0 0 0 0 2)
                                                  (list 0 0 0 0 0 0 0 0 1)
                                                  (list 0 0 0 0 0 0 0 0 1)
                                                  (list 0 0 0 0 0 0 0 0 1)
                                                  (list 0 0 0 0 0 0 0 0 0)
                                                  (list 0 0 0 0 0 0 0 0 0)
                                                  (list 0 0 0 0 0 0 0 0 0))
                                                 2
                                                 5
                                                 empty))
                              (make-world-state
                               (list
                                (list 0 0 0 0 0 0 0 0 0)
                                (list 0 0 0 0 0 0 0 0 2)
                                (list 0 0 0 0 0 0 0 0 1)
                                (list 0 0 0 0 0 0 0 0 1)
                                (list 0 0 0 0 0 0 0 0 1)
                                (list 0 0 0 0 0 0 0 0 0)
                                (list 0 0 0 0 0 0 0 0 0)
                                (list 0 0 0 0 0 0 0 0 0))
                               2
                               5
                               empty)
                              (list 0 0)
                              +inf.0)
              (list 5 8))
;; Function definition
(define (find-best-move lom state best-move best-move-val)
  (cond
    [(empty? lom) (error "No Legal Moves")]
    [(empty? (rest lom))(local [(define first-move-val (min-max-state 0 (make-move state (first lom))))]
                          (if (< first-move-val best-move-val)
                                 (first lom)
                                 best-move))
                                 ]
    [else
     (local [(define first-move-val (min-max-state 0 (make-move state (first lom))))]
                       (if (< first-move-val
                              best-move-val)
                           (find-best-move (rest lom) state (first lom) first-move-val)
                           (find-best-move (rest lom) state best-move best-move-val))
                       )]))


;; State --> State
;; This takes the world state, and uses it
;; to create another world state in which the computer player made an intelligent move
(define (computer-moves state)
  (local
    [(define moves (legal-next-moves state))]
    (make-move state (find-best-move moves state (list 0 0) +inf.0))))
;;                                           bad move ^       ^ Undesirable evaluation number




(check-expect (min-max-state DEPTH start-state) (evaluation-function start-state))
;; Mutual recursion buddies.
;; min-max-state is called with world state and depth,
;; min-max-list-of-move is then called from former function with
;; depth, list of possible moves and state.
;; determinate is comparison function, called from min-max-list-of-move with
;; state and two values of the moves, evaluated with evaluation function.
;; Min or max is determined based on current turn - (Computer|Human).


;; Number State --> Number
;; Function returns value of board state from the evaluation function. 
;; It looks at the current state and the next legal moves that can be made.
(define (min-max-state depth state)
  (if (>= depth DEPTH)
      (evaluation-function state)
      (min-max-list-of-move depth (legal-next-moves state) state)))

;; Number ListOfMove State --> Number
;; Functions examines list of legal moves and passes single moves into the above function
;; for evaluation or further processing. Values are later on passed to the determinate function.
(define (min-max-list-of-move depth moves state)
  (cond [(empty? moves) "No legal moves"]
        [(empty? (rest moves)) (min-max-state (+ depth 1) (make-move state (first moves)))]
        [else (determinate state
                           (min-max-state (+ depth 1) (make-move state (first moves)))
                           (min-max-list-of-move depth (rest moves) state))]))





;; Function returns either maximum or minimum of value1 and value2
;; based on the current turn in the game - decided by state structure comparison to RED.
;; helper function for min-max-list-of-move
(check-expect (determinate start-state 5 10) 10)

(define (determinate state value1 value2)
  (if (= (world-state-whose-turn state) RED)
      (max value1 value2)
      (min value1 value2)))
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(define (red? num)
  (= num RED))

(define (black? num)
  (= num BLACK))



;; takes a column of checkers, and returns the value of that column
;; big numbers are good for the human, small numbers are good for AI
;; finds how valuable this column is, and finds who has more checkers 
;; there.  
;; 
;; alop Natural --> Natural
(define (checker-column alop current-col)
  (local
    ;; checkers played towards the center are worth more points than 
    ;; those played toward the edge.  
    ;; yes, 3 is a constant, but is connect *four*.  playing with fewer
    ;; than 4 columns would be a bit stilly
    [(define points 
       (min current-col 3 (- (- COLUMNS 1) current-col)))
     (define num-red
       (length (filter red? alop)))
     (define num-black
       (length (filter black? alop)))]
    (* points
       ;; want to spread checkers out as stacking them vertically is not 
       ;; a great idea.  therefore, putting a checker in a new
       ;; column counts as 2 checkers for evaluation purposes
       (- (+ num-red (min num-red 1))
          (+ num-black (min num-black 1))))))

;; recursively walks through the columns on the board, and 
;; sums up the point value of each column
(define (checker-value board current-col)
  (cond
    ([= current-col (- COLUMNS 1)] 0)
    (else
     (+ (checker-column (first board) current-col)
        (checker-value (rest board) (+ 1 current-col))))))

;; state --> Number.  
;; evaluates how good a state is.  larger values are better for red (human) 
;; while smaller values are better for black (computer)

(define (evaluation-function state)
  (local
    [(define red_turn? ( = (world-state-whose-turn state) RED))]
    ;; add a small amount of random noise to have more interesting tie breaking
    ;; otherwise the computer will tend to pick moves on the left side of the board
    (+ (/ 1 (+ 1 (random 10000)))
       (cond
         [(check-win? state)
          ;; we need to look backwards, since if it's red's turn and the game was just
          ;; won, that means that black won on its move (thus -999)
          (if red_turn? -999 999)] 
         [else 0])
       (checker-value (world-state-position state) 0))))


;; you should not modify code below this point

;; the representation of the world.  you should not be modifying this structure or its contents
;; to conduct the search, you can create instances of this structure as your representation

;; *** the settings part of world-state is now defined as time (a Number > 0)
;; *** as well as other-info, which is user-defined and can include anything you like
;; *** you can define a structure and add it to the state representation, or
;; *** just store a number.  the provided code will not make use of it
(define-struct world-state (position whose-turn settings other-info))
;;                                                         ^^^^


;; returns the checker color (RED, BLACK, or BLANK) of the specified position
;; on the board
(define (piece-at board row column)
  (get-nth row (get-nth column board)))


;; Natural List --> Element
;; returns the nth element of a list
(define (get-nth n alist)
  (local [(define (nth-helper n current alist)
            (cond
              [(= n current)(first alist)]
              [else
               (nth-helper n (+ 1 current) (rest alist))]))]
    (nth-helper n 0 alist)))

(define (main state)
  (local 
    [(define board 
       (make-list COLUMNS
                  (make-list ROWS 0)))
     
     (define PIECE-SIZE 30)
     
     (define RED-CHECKER (circle PIECE-SIZE "solid" "red"))
     (define BLACK-CHECKER (circle PIECE-SIZE "solid" "black"))
     (define BLANK-CHECKER (circle PIECE-SIZE "solid" "white"))
     
     (define OFFSET (/ PIECE-SIZE .66))
     (define WIDTH
       (+ (* COLUMNS 2.5 PIECE-SIZE) (* 0.5 PIECE-SIZE)))
     (define HEIGHT
       (+ (* ROWS 2.5 PIECE-SIZE) (* 0.5 PIECE-SIZE)))
     
     (define MTS 
       (rectangle WIDTH HEIGHT "solid" "yellow"))
     (define (place-checker state x y mouse-event)
       (local
         [(define move (map-coordinates x y))
          (define next-state (make-move state move))]
         (cond
           [(and (string=? mouse-event "button-down")
                 (member move (legal-next-moves state)))
            (if (check-win? next-state)  
                (cond
                  [(= (world-state-whose-turn state) RED)
                   "RED WINS"]
                  [(= (world-state-whose-turn state) BLACK)
                   "BLACK WINS"])
                (local [(define result (computer-moves next-state))]
                  (if (check-win? result)
                      (cond
                        [(= (world-state-whose-turn next-state) RED)
                         "RED WINS"]
                        [(= (world-state-whose-turn next-state) BLACK)
                         "BLACK WINS"])
                      result)))]
           [else state])))
     (define (display-column2 column x-offset y-offset image)
       x-offset)
     (define (display-column column x-offset y-offset image)
       (cond
         [(empty? column) image]
         [else
          (place-image
           (cond 
             [(= (first column) RED) RED-CHECKER]
             [(= (first column) BLACK) BLACK-CHECKER]
             [(= (first column) BLANK) BLANK-CHECKER])
           x-offset y-offset 
           (display-column (rest column) x-offset (+ y-offset (* 2.5 PIECE-SIZE)) image))]))
     
     (define (display-board-helper position x-offset image)
       (cond 
         [(empty? position) image]
         [else
          (display-board-helper
           (rest position)
           (+ x-offset (* 2.5 PIECE-SIZE))
           (display-column (first position)
                           x-offset
                           OFFSET image))]))
     
     (define (display-board position)
       (display-board-helper position OFFSET MTS))
     (define (render state)
       (display-board (world-state-position state)))
     
     (define (map-coordinate lower upper click pos)
       (cond
         [(and (> click lower) (< click upper)) pos]
         [(> pos (max ROWS COLUMNS)) -1]
         [else
          (map-coordinate (+ lower (* 2.5 PIECE-SIZE)) (+ upper (* 2.5 PIECE-SIZE)) click (+ 1 pos))]))
     
     (define (map-coordinates x y) 
       (list (map-coordinate (/ PIECE-SIZE 2) (+  (/ PIECE-SIZE 2) (* 2 PIECE-SIZE)) x 0)
             (map-coordinate (/ PIECE-SIZE 2) (+  (/ PIECE-SIZE 2) (* 2 PIECE-SIZE)) y 0)))]
    
    (big-bang state 
              (on-mouse place-checker) 
              (to-draw render))))

;; *** this function permits you to make both legal and illegal moves
;; *** you do not need to use this function and probably should not.  someone thought of a reason
;; *** for it to exist and so i included it.  to be clear, your program is only permitted to 
;; *** make legal moves.
(define (make-hypothetical-move state move)
  (local [(define (update-column turn column current move)
            (cond
              [(empty? column) empty]
              [else
               (cons
                (cond
                  [(= current move)
                   turn]
                  [else (first column)])
                (update-column turn (rest column) (+ 1 current) move))]))
          
          (define (do-move board turn move-x move-y current-x)
            (cond
              [(empty? board) empty]
              [else
               (cons
                (cond
                  [(= move-x current-x) (update-column turn (first board) 0 move-y)]
                  [else (first board)])
                (do-move (rest board) turn move-x move-y (+ 1 current-x)))]))]
    (make-world-state
     (do-move (world-state-position state)
              (world-state-whose-turn state) 
              (first move) (second move) 0)
     (cond
       [(= (world-state-whose-turn state) RED) BLACK]
       [(= (world-state-whose-turn state) BLACK) RED])
     (world-state-settings state)
     (world-state-other-info state))))

;; you will use this function.  it takes as input the move you will make, represented as a list of X Y coordinates
(define (make-move state move)
  (if (member move (legal-next-moves state))
      (make-hypothetical-move state move)
      state))

;; world-state --> list
;; returns all of the legal moves for the current position
(define (legal-next-moves state)
  (local [
          (define (first-blank column pos)
            (cond
              [(empty? column) (- pos 1)]
              [(not (= (first column) BLANK))
               (- pos 1)]
              [else (first-blank (rest column) (+ 1 pos))]))
          (define (get-moves board-state column)
            (cond
              [(empty? board-state) empty]
              [else
               (local [(define blank (first-blank (first board-state) 0))]
                 (if (< blank 0)
                     (get-moves (rest board-state) (+ 1 column))
                     (cons
                      (list column (first-blank (first board-state) 0))
                      (get-moves (rest board-state) (+ 1 column)))))]))]
    (get-moves (world-state-position state)
               0)))

;; check-win:  world-state --> boolean
;; determines whether the game has ended with a victory for whoever just moved
(define (check-win? state)
  (local [    
          (define (up-column board color x y)
            (if (< y 3)
                false
                (local [(define column (get-nth x board))]
                  (= (get-nth  (- y 1) column)
                     (get-nth  (- y 2) column)
                     (get-nth  (- y 3) column)
                     color))))
          
          (define (right-row board color x y)
            (if (>= x (- COLUMNS 3))
                false
                (= (get-nth y (get-nth (+ 1 x) board))
                   (get-nth y (get-nth (+ 2 x) board))
                   (get-nth y (get-nth (+ 3 x) board))
                   color)))
          
          (define (up-right board color x y)
            (if (or (< y 3)
                    (>= x (- COLUMNS 3)))
                false
                (= (get-nth (- y 1) (get-nth ( + x 1) board))
                   (get-nth (- y 2) (get-nth ( + x 2) board))
                   (get-nth (- y 3) (get-nth ( + x 3) board))
                   color)))
          
          (define (down-right board color x y)
            (if (or (>= y (- ROWS 3))
                    (>= x (- COLUMNS 3)))
                false
                (= (get-nth (+ y 1) (get-nth ( + x 1) board))
                   (get-nth (+ y 2) (get-nth ( + x 2) board))
                   (get-nth (+ y 3) (get-nth ( + x 3) board))
                   color)))
          
          (define (victory? board x y)
            (let
                ([color (get-nth y (get-nth x board))])
              (if (= color BLANK)
                  false
                  (or
                   (up-column board color x y)
                   (right-row board color x y)
                   (up-right board color x y)
                   (down-right board color x y)))))
          
          (define (walk-column board col row)
            (cond
              [(= row ROWS) false]
              [else
               (or
                (victory? board col row)
                (walk-column board col (+ 1 row)))]))
          
          (define (walk-board board col)
            (cond
              [(= col COLUMNS) false]
              [else
               (or (walk-column board col 0)
                   (walk-board board (+ 1 col)))]))]
    (walk-board (world-state-position state) 0)))


(define START-BOARD
  (make-list COLUMNS
             (make-list ROWS BLANK)))
(define start-state
  (make-world-state START-BOARD RED 5 empty))


;; UGH.  Ignore this test board.  It was created to help me debug.
;; the only valid values in a real board are 0 (BLANK), 1 (RED), and 
;; 2 (BLACK)
(define test-board
  (list
   (list 2 2 0 2 2 2 7 1 2)
   (list 2 12 13 14 15 16 17 18 19)
   (list 1 22 23 24 25 26 27 28 29)
   (list 2 32 33 34 35 36 37 38 39)
   (list 1 1 1 2 2 1 0 0 0)
   (list 2 2 2 1 0 0 0 0 0)
   (list 2 2 0 0 0 0 0 1 0)
   (list 1 2 1 0 0 0 0 0 0)))

(define test-state (make-world-state test-board RED 5 empty))


(main start-state)
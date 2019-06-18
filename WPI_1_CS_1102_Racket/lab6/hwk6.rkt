#lang racket
(require test-engine/racket-tests)

;; Nicholas Bradford & Ezra Davis


;; Implements a simple class system in racket
(define-syntax class
  (syntax-rules (method initvars tests)
    [(class
         (initvars var1 ...)
       (method name (arg1 ...) body)
       ...
       )
     
     (lambda (var1 ...)
       (lambda (message)
         (cond
           [(symbol=? 'name message)
            (lambda (arg1 ...) body)]
           ...
           [else (error "Undefined method for this \"class\"")]
           )))
     ]
    ))
;; Example class
(define make-cat-obj (class
                         (initvars whiskers dead?)
                       (method more-whiskers (x) (> whiskers x))
                       (method run-over () (make-cat-obj whiskers true))
                       (method dead? () dead?)
                       ))


;; Examples using the example class
(define tabby (make-cat-obj 6 false))
(define flat-cat ((tabby 'run-over)))

(check-expect ((tabby 'more-whiskers) 3) #t)
(check-expect ((flat-cat 'dead?)) #t)
(check-error ((flat-cat 'mouse?)))
;;===========================================================
;;Sends a message to a 'class'
(define-syntax send
  (syntax-rules ()
    [(send object method args ...)
     
     (let [(instance object) (message 'method)]
       (cond [(not (procedure? instance))
              (error "Send's first argument should be a object (procedure)")]
             [(not (symbol? message))
              (error "Send's second argument cannot be converted into a symbol")]
             [else  
              ((instance message) args ...)]
             ))]
    
    [(send stuff ...)
     (error "Send expects a \"object\", a message/method name, and its arguments")]))

;; Tests
(check-error (send tabby mouse?))
(check-error (send 'squirrel eat))
(check-error (send 3 floor))
(check-expect (send tabby more-whiskers 6) #f)

;; More tests of send and class
(define dillo-class
  (class (initvars length dead?)
         (method longer-than? (len) (> length len))
         (method run-over () (dillo-class (+ length 1)
true))))
(define d3 (dillo-class 5 false))
(check-expect (send d3 longer-than? 6) false)
(check-expect (send d3 longer-than? 5) false)
(define d4 (send d3 run-over))
(check-expect (send d4 longer-than? 5) true)
;;==========================================================

(define-syntax policy-checker
  (syntax-rules ()
    [(policy-checker
      (role (access-type ...) (file-type ...))
      ...)
     
     (lambda ( a-role a-access-type a-file-type)
       (cond
         [(and (symbol=? a-role 'role) (member a-access-type (list 'access-type ...)) (member a-file-type (list 'file-type ...)))
          #t]
         ...
         [(not (member a-role (list 'role ...))) (error "Undefined role")]
         [(not (member a-access-type (append (list 'access-type ...) ...))) (error "Undefined access-type")]
         [(not (member a-file-type (append (list 'file-type ...) ...))) (error "Undefined file-type")]
         [else #f]))]))


(define check-policy
  (policy-checker
   (programmer (read write) (code documentation))
   (tester (read) (code))
   (tester (write) (documentation))
   (manager (read write) (reports))
   (manager (read) (documentation))
   (ceo (read write) (code documentation reports))))

;;Tests for policy-checker
(check-expect (check-policy 'programmer 'write 'code) true)
(check-expect (check-policy 'programmer 'write 'reports) false)
(check-expect (check-policy 'ceo 'write 'documentation) true)
(check-expect (check-policy 'manager 'read 'documentation) true)
(check-expect (check-policy 'manager 'write 'documentation) false)
(check-error (check-policy 'programmer 'play 'game))
(check-error (check-policy 'armadillo 'write 'code))
(check-error (check-policy 'programmer 'hack 'code))
(check-error (check-policy 'programmer 'write 'essay))
;;=========================================================
(test)
;; The first three lines of this file were inserted by DrRacket. They record metadata
;; about the language level of this file in a form that our tools can easily process.
#reader(lib "htdp-beginner-reader.ss" "lang")((modname |fireworks starter|) (read-case-sensitive #t) (teachpacks ()) (htdp-settings #(#t constructor repeating-decimal #f #t none #f ())))
(require 2htdp/image)
(require 2htdp/universe)

;;  you should use a better name for the world state than "param" :-)
;; these are the function names you should use in your code
(define (main param)
  (big-bang param
            (on-tick move-fireworks)
            (to-draw render)
            (on-mouse handle-mouse)))
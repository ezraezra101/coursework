//////////////////////////
///    Question 1 A    ///
//////////////////////////

#define  MAX_PRESSES   50

....
unsigned char press=0, tmp, press_hist[MAX_PRESSES];

....
while(press < MAX_PRESSES) {
  press++;
  tmp = getTouchPad(); // returns symbol for touch pad press_hist[press] = tmp;

  switch(tmp) {
    case 'X':
      led_on(0);
      buzzerOn();
      swDelay(2);
      buzzerOff();
      led_off();
      break;
    case 'S': //Square
      buzzerOn();
      swDelay(1);
      buzzerOff();
      break;
    case '^':
    case 'P'://Polygon (Octagon)
    case 'O':
      led_on(1);
      break;
  }
}



//////////////////////////
///    Question 1 B    ///
//////////////////////////

////  Question 1 B i  ////
 The loop runs 255 times, because kk increments each loop, and when an
 unsigned char should be 256, it loops around and becomes 0.

Variable | 1st loop | last loop
kk       |  2       | 0
mm       |  65001   | 156
inVal    |  64900   | 39500

////  Question 1 B ii ////
This loop runs four times; we are bitshifting cnt over two bits each iteration,
and there are only 8 bits in cnt. 8/2 == 4. When the entirety of cnt has been
shifted, it will equal 0 and thus be falsy.

Value of cnt in all but the first loop:
0xF0
0xC0
0x00

//////////////////////////
///    Question 1 C    ///
//////////////////////////
Casting in C allows numbers to be converted between types,
often used when you want to calculate a expression to a different
precision than the variables involved.

A: -10240.0, bb: -4096, cc: 2048


//////////////////////////
///    Question 1 D    ///
//////////////////////////

#include <stdlib.h>

int VALUES[500];

int minimum();
int maximum();
int meanSquare();

int main() {
  int i;
  for(i=0; i<500; i++) {
    VALUES[i] = rand() % (250+300) - 250;
  }
  
  int min = minimum();
  int max = maximum();
  int meanSq = meanSquare();
  
  return 0;
}
// Returns the smallest element in VALUES
int minimum() {
  int i, min;
  min = VALUES[0];
  for(i=1; i<500; i++) {
    if(min > VALUES[i]) {
      min = VALUES[i];
    }
  }
  return min;
}
// Returns the largest element in VALUES
int maximum() {
  int i, max;
  max = VALUES[0];
  for(i=1; i<500; i++) {
    if(max < VALUES[i]) {
      max = VALUES[i];
    }
  }
  return max;
}
// Returns the average of the squares of each element in VALUES
int meanSquare() {
  int i;
  long int square_sum = 0;
  for(i=0; i<500; i++) {
    square_sum += VALUES[i]*VALUES[i];
  }
  
  return square_sum / 500;
}

// We aren't using Pow because it takes and produces doubles, which requires at
// least four times as much memory than just using 16-bit integers. The doubles
// also have to be manipulated in parts - just one word at a time, whereas
// integers can be multiplied in just one command. This makes pow() quite slow
// and memory intensive.

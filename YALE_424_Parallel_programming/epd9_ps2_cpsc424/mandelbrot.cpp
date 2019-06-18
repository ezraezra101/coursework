#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "drand.c"
#include "timing.h"

// #define VERBOSE

#if __has_include ("omp.h")
#include <omp.h>
#else
#define omp_get_max_threads() (0)
#define omp_get_num_threads() (1)
#endif

// Default number of iterations to calculate mandelbrot set.
int max_iterations = 111;

// Determining some facts about the region of the set we're working with:
// It simplifies a lot if these are global.
const float xBounds[2] = {-2.0, 0.5};
const float iBounds[2] = {0.0, 1.25};
const float cellSize = 0.001;
const int xIterations = ceil((xBounds[1]-xBounds[0])/cellSize);
const int iIterations = ceil((iBounds[1]-iBounds[0])/cellSize);
const int cellsTotal = xIterations*iIterations;

// If cellSize doesn't fit perfectly into bounds, R may be a little larger than the bounds.
const float rArea = xIterations*cellSize * iIterations*cellSize;

typedef float (*function_p)( void );



// Perform `max` iterations of the mandelbrot set, and returns the number of iterations
// it takes to exceed 2 (and thus isn't part of the mandelbrot set).
// If it never exceeds 2, will return `max`
static inline int iterationsAtLocation(float x, float y, int max) {
  int iterations = 0;
  float i=0, j=0, r=0;
  while(j*j+i*i < 2 && ++iterations<max) {
    // while (abs(j+ i*imaginary) < 2)
    // real = lastReal^2-lastImaginary^2 + originalReal
    // imaginary = 2*real
    r=j;
    j=r*r-i*i+x;
    i=2*r*i+y;
  }
  return iterations;
}

// Is this point in the mandelbrot set?
static inline int inSet(float x, float y, int max) {
  return max == iterationsAtLocation(x,y,max);
}


// Draws the mandelbrot set to the terminal.
void prettyPrintMandelbrot(int max_iterations) {
  int width = 168; // width in characters of the terminal
  float charAspectRatio = 2.5; // Characters are ~ 2.5 times taller than they are wide.

  float xBounds[2] = {-2.0, 0.5};
  float iBounds[2] = {-1.25, 1.25};
  float resolution = (xBounds[1]-xBounds[0])/width;

  for(float y = iBounds[0]; y <= iBounds[1]; y+= resolution*charAspectRatio) {
    printf("\n");
    for(float x = xBounds[0]; x <= xBounds[1]; x+= resolution) {
      int iters = iterationsAtLocation(x,y,max_iterations);
      int indxToDisplay = iters&15;
      // More accurate/useful, but less visually interesting:
      //int indxToDisplay = 16 * (max_iterations-iters)/max_iterations;
      printf("%c"," .:-;!/>)|&IH%*#"[indxToDisplay]);
    }
  }
  printf("\n");

}

////////////////////////////////////////////
// Different methods for calculating area //

// Be serial!
float getAreaSerial() {
  int cellsIn = 0;
  dsrand(12345);
  for(int x = 0; x < xIterations; x++) {
    for(int i=0; i < iIterations; i++) {
      float xcoord = (x+drand()) * cellSize + xBounds[0];
      float icoord = (i+drand()) * cellSize + iBounds[0];

      cellsIn += inSet(xcoord, icoord,max_iterations);
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}

// Use parallel for to calculate area
float getAreaFor() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp for reduction(+:cellsIn)
    for(int x = 0; x < xIterations; x++) {
      for(int i=0; i < iIterations; i++) {
        float xcoord = (x+drand()) * cellSize + xBounds[0];
        float icoord = (i+drand()) * cellSize + iBounds[0];

        cellsIn += inSet(xcoord, icoord,max_iterations);
      }
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}

// Use parallel for to calculate area
float getAreaForStatic1() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp for reduction(+:cellsIn) schedule(static,1)
    for(int x = 0; x < xIterations; x++) {
      for(int i=0; i < iIterations; i++) {
        float xcoord = (x+drand()) * cellSize + xBounds[0];
        float icoord = (i+drand()) * cellSize + iBounds[0];

        cellsIn += inSet(xcoord, icoord,max_iterations);
      }
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}
// Use parallel for to calculate area
float getAreaForStatic10() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp for reduction(+:cellsIn) schedule(static,10)
    for(int x = 0; x < xIterations; x++) {
      for(int i=0; i < iIterations; i++) {
        float xcoord = (x+drand()) * cellSize + xBounds[0];
        float icoord = (i+drand()) * cellSize + iBounds[0];

        cellsIn += inSet(xcoord, icoord,max_iterations);
      }
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}
// Use parallel for to calculate area
float getAreaForDynamic() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp for reduction(+:cellsIn) schedule(dynamic)
    for(int x = 0; x < xIterations; x++) {
      for(int i=0; i < iIterations; i++) {
        float xcoord = (x+drand()) * cellSize + xBounds[0];
        float icoord = (i+drand()) * cellSize + iBounds[0];

        cellsIn += inSet(xcoord, icoord,max_iterations);
      }
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}

// Use parallel for to calculate area
float getAreaForDynamic10() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp for reduction(+:cellsIn) schedule(dynamic,10)
    for(int x = 0; x < xIterations; x++) {
      for(int i=0; i < iIterations; i++) {
        float xcoord = (x+drand()) * cellSize + xBounds[0];
        float icoord = (i+drand()) * cellSize + iBounds[0];

        cellsIn += inSet(xcoord, icoord,max_iterations);
      }
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}
// Use parallel for to calculate area
float getAreaForGuided() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp for reduction(+:cellsIn) schedule(guided)
    for(int x = 0; x < xIterations; x++) {
      for(int i=0; i < iIterations; i++) {
        float xcoord = (x+drand()) * cellSize + xBounds[0];
        float icoord = (i+drand()) * cellSize + iBounds[0];

        cellsIn += inSet(xcoord, icoord,max_iterations);
      }
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}

// Use parallel for to calculate area
float getAreaForCollapse() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp for reduction(+:cellsIn) collapse(2)
    for(int x = 0; x < xIterations; x++) {
      for(int i=0; i < iIterations; i++) {
        float xcoord = (x+drand()) * cellSize + xBounds[0];
        float icoord = (i+drand()) * cellSize + iBounds[0];

        cellsIn += inSet(xcoord, icoord,max_iterations);
      }
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}

// Use parallel for to calculate area
float getAreaForGuidedCollapse() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp for reduction(+:cellsIn) collapse(2)
    for(int x = 0; x < xIterations; x++) {
      for(int i=0; i < iIterations; i++) {
        float xcoord = (x+drand()) * cellSize + xBounds[0];
        float icoord = (i+drand()) * cellSize + iBounds[0];

        cellsIn += inSet(xcoord, icoord,max_iterations);
      }
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}

float getParallelTask() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp single
    {
      for(int x = 0; x < xIterations; x++) {
        for(int i=0; i < iIterations; i++) {
          #pragma omp task shared(cellsIn) firstprivate(x, i)
          {
            float xcoord = (x+drand()) * cellSize + xBounds[0];
            float icoord = (i+drand()) * cellSize + iBounds[0];
            bool in = inSet(xcoord, icoord,max_iterations);
            #pragma omp atomic update
            cellsIn += in;
          }
        }
      }
    }
    //TODO really really slow...
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}

float getParallelTaskRow() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp single
    {
      for(int x = 0; x < xIterations; x++) {
        #pragma omp task shared(cellsIn) firstprivate(x)
        for(int i=0; i < iIterations; i++) {
          float xcoord = (x+drand()) * cellSize + xBounds[0];
          float icoord = (i+drand()) * cellSize + iBounds[0];
          bool in = inSet(xcoord, icoord,max_iterations);
          #pragma omp atomic update
          cellsIn += in;
        }
      }
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}

float getParallelTaskRowSharedCreation() {
  int cellsIn = 0;
  #pragma omp parallel
  {
    dsrand(12345);
    #pragma omp for
    for(int x = 0; x < xIterations; x++) {
      #pragma omp task shared(cellsIn) firstprivate(x)
      for(int i=0; i < iIterations; i++) {
        float xcoord = (x+drand()) * cellSize + xBounds[0];
        float icoord = (i+drand()) * cellSize + iBounds[0];
        bool in = inSet(xcoord, icoord,max_iterations);
        #pragma omp atomic update
        cellsIn += in;
      }
    }
  }
  return 2*rArea*cellsIn/(float)cellsTotal;
}

int main(int argc, char **argv) {
  if (argc == 2) {
    max_iterations = atoi(argv[1]);
  }

  printf("Threads,\tIterations,\tTime,\t\tArea,\t\tParallel type\n");

  int num_funcs = 12;
  char *names[] = {
    "serial",
    "for",
    "for static 1",
    "for static 10",
    "for dynamic",
    "for dynamic 10",
    "for guided",
    "for collapsed",
    "for guided collapsed",
    "task per row",
    "task shared row creation",
    "task",
  };
  function_p function_pointers[] = {
    getAreaSerial,
    getAreaFor,
    getAreaForStatic1,
    getAreaForStatic10,
    getAreaForDynamic,
    getAreaForDynamic10,
    getAreaForGuided,
    getAreaForCollapse,
    getAreaForGuidedCollapse,
    getParallelTaskRow,
    getParallelTaskRowSharedCreation,
    getParallelTask,
  };
  for(int i=0; i<num_funcs;i++) {
    double startTime, startCPUTime, endTime,endCPUTime;
    timing(&startTime, &startCPUTime);

    float area = function_pointers[i]();
    
    timing(&endTime, &endCPUTime);

    printf(
      "%d,\t\t%d,\t\t %lf,\t %f,\t %s\n",
      omp_get_max_threads(),
      max_iterations,
      endTime-startTime,
      area,
      names[i]
    );
  }

#ifdef VERBOSE
  prettyPrintMandelbrot(max_iterations);
#endif
   return 0;
}

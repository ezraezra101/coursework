
ITERATIONS=500

for MAX_THREADS in 1 2 4 8 10
do
export OMP_NUM_THREADS=$MAX_THREADS
make mandelbrot >/dev/null
./mandelbrot $ITERATIONS
done
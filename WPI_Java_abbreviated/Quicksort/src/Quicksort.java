import java.util.Random;

public class Quicksort {
	
	static boolean debug = false; // See true if you want to print debug info.  

	/** Main program for demonstrating Quicksort
	 * @param args None
	 */
	public static void main(String[] args) {
		System.out.println("Testing quicksort's runtime with various array sizes\n");
		testRuntime();
	}

	
	/** 
	 * Tests quicksort a single time and returns the number of loops run.
	 * 
	 * @param size length of the array used in testing
	 * @author ezradavis
	 */
	public static int testQuicksort(int size)
	{
		SortTimes st = new SortTimes();
		
		int[]a = randArray(size, size * 2); //Creates an array with 'size' elements ranging from 0 to 'size' * 2
		
		quicksort(a,0,(a.length - 1), st);
		
		return st.numLoops;
	}
	
	/**
	 * Experimentally computes the mean and standard deviation of quicksort running on a number of arrays
	 * 
	 * @param sampleSize The number of times quicksort is run
	 * @param arraySize The length of the arrays that quicksort sorts
	 * 
	 * @author ezradavis
	 */
	public static void experimentalDistribution(int sampleSize, int arraySize)
	{
		int[] array = new int[sampleSize]; // Loops used in each test
		int sum = 0; // Sum of all loops run
		
		for(int i=0; i < sampleSize; i++)
		{
			array[i] = testQuicksort(arraySize);
			sum += array[i];
		}
		
		double mean = sum / (double) sampleSize; //Average
	
		double variance = 0;
		for(int i=0; i < sampleSize; i++)
		{
			double temp = array[i] - mean;
			variance += temp * temp;
		}
		// Variance is currently the actual variance * (n-1)
		variance /= sampleSize - 1;
			//Its sampleSize - 1 because we are using a sample, not population
		
		double stdev = Math.sqrt(variance);
		
		System.out.println(stdev + "\t" + mean);
	}
	
	/**
	 * Tests the runtime distributions of quicksort for different sizes of arrays
	 * and prints them out.
	 * 
	 * @author ezradavis
	 */
	static public void testRuntime()
	{
		int sampleSize = 100;
		System.out.println("All samples are of size " + sampleSize);
		
		System.out.println("Array Length:\tStandard Deviation:\tMean:");
		for(int i=1; i < 12; i++)
		{
			int arraySize = (int) Math.pow(2,i); //arraySize is: 2, 4, 8,...2048
			System.out.print(arraySize + "\t\t");
			experimentalDistribution(sampleSize, arraySize);
		}
	}
	
	/**
	 * Prints an array of integers
	 * @param a Array to print
	 */
	static void printArray(int[] a) {
		int i; // Loop counter
		
		for (i = 0; i < a.length; i++) 
			System.out.print(a[i] + " ");	
	}
	
	/**
	 * Instantiates an array of random integers
	 * @param size Size of array
	 * @param max All values will be from 0 to max, inclusive
	 * @return The newly-allocated array
	 */
	static int[] randArray(int size, int max) {
		int a[] = new int[size];
		int i; // Loop counter
		Random r = new Random(); // Generator
		
		for (i = 0; i < size; i++) {
			a[i] = r.nextInt(max + 1);
		}
		return a;
	}
	
	/**
	 * Implements the Quicksort algorithm in the CLRS book. Sorts array of integers into ascending order.
	 * Note: Java arrays count from 0, not 1.
	 * @param a Array being sorted. Only a section of it, elements p through r, will get sorted on each invocation.
	 * @param p Lowest index to be sorted
	 * @param r Highest index to be sorted.
	 * @param st SortTimes object that gets incremented with the operations executed
	 */
	static void quicksort(int[] a, int p, int r, SortTimes st) {
		st.numCalls++;
		if (Quicksort.debug) {
			System.out.print("\nArray: ");
			printArray(a);
			System.out.println();
			System.out.println("Sorting (and partitioning) elements " + p + " through " + r);
		}
		int q; // Index of pivot
		if (p < r) {
			
			// Divide array into two partitions. On return all elements p..(q - 1) will be < a[q],
			// and all elements (q + 1)..r will be > a[q].
			q = partition(a, p, r, st); 
			if (Quicksort.debug) System.out.println("  New q (index of pivot) = " + q);
			
			// Then sort each partition
			quicksort(a, p, q - 1, st);
			quicksort(a, q + 1, r, st);			
		}
	}
	
	/**
	 * Rearranges the subarray a[p..r] in place
	 * @param a Array to partition
	 * @param p Lowest index to start partitioning
	 * @param r Highest index to stop partitioning
	 * @param st SortTimes object that gets incremented with the operations executed
	 * @return Index of pivot. when done, all elements of a from p through (this index - 10
	 *   will be <= the pivot value, and all elements (this index + 1) through r will
	 *   be > the pivot value.
	 */
	static int partition(int[] a, int p, int r, SortTimes st) {

		int x; // Pivot
		int j; // Loop counter
		int i; // Index to element maybe being exchanged.
		int temp; // Used for exchanging

		st.numCalls++;
		
		x = a[r];
		i = p - 1;
		
		if (Quicksort.debug) System.out.println("  Pivot value = " + x);
		
		for (j = p; j <= (r - 1); j++) {
			st.numLoops++;
			st.numCompares++;
			if (a[j] <= x) {
				// a[j] belongs in the left partition.
				st.numSwaps++;
				i = i + 1;
				
				// Exchange a[i] with a[j]
				if (Quicksort.debug) System.out.println("    Swapping a[" + i+ "]:" + a[i] +
						" with a[" + j + "]:" + a[j]);
				
				temp = a[i];
				a[i] = a[j];
				a[j] = temp;
			}
		}
		
		// Exchange a[i + 1] with a[r]. This moves the pivot to the middle
		if (Quicksort.debug) System.out.println("    Finally swapping a[" + (i + 1) + "]:" + a[i + 1] +
				" with a[" + r + "]:" + a[r]);
		st.numSwaps++;
		temp = a[i + 1];
		a[i + 1] = a[r];
		a[r] = temp;
		
		return i + 1;
	}
}

/**
 * This class keeps track of the number of operations executed during sorting.
 * @author ciaraldi
 *
 */
class SortTimes {
	public int numCalls = 0; // number of times a function is called
	public int numLoops = 0; // number of times through a loop
	public int numCompares = 0; // number of comparisons made
	public int numSwaps = 0; // number of swaps (exchanges) made
	
	/** Adds the times in the parameter object to those in the current object.
	 * 
	 * @param t Object whose times will be added to the current ("this") object.
	 */
	public void increment(SortTimes t) {
		numCalls += t.numCalls;
		numLoops+= t.numLoops;
		numCompares += t.numCompares;
		numSwaps += t.numSwaps;
	}
	
	/**
	 * @return Printable version of the object
	 */
	public String toString() {
		return "numCalls = " + numCalls + ", numLoops = " + numLoops + ", numCompares = " + numCompares + ", numSwaps = " + numSwaps;
	}
}

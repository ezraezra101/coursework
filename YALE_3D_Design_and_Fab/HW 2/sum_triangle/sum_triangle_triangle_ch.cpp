//! \file examples/Minkowski_sum_2/sum_triangle_square.cpp
// Computing the Minkowski sum of a number of pairs of triangles

#include <CGAL/basic.h>
#include <CGAL/minkowski_sum_2.h>

#include <list>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Boolean_set_operations_2.h>
#include <CGAL/ch_graham_andrew.h>

typedef CGAL::Exact_predicates_exact_constructions_kernel Kernel;
typedef Kernel::Point_2                            Point_2;
typedef CGAL::Polygon_2<Kernel>                    Polygon_2;
typedef CGAL::Polygon_with_holes_2<Kernel>         Polygon_with_holes_2;
typedef std::list<Polygon_with_holes_2>            Pgn_with_holes_2_container;


#include <fstream>
#include <iostream>
#include <string>


#define OUTFILE_PREFIX "ch_"
// Creates and opens the correct output file based on argv[1]
void open_out_file(char *argv1, std::ofstream &out) {
    //Filename wrangling
  std::string inFileName(argv1);
  if(inFileName.find_last_of('/') != std::string::npos) {
    inFileName = inFileName.substr(inFileName.find_last_of('/') + 1);
  }
  std::string outFilename = OUTFILE_PREFIX;
  outFilename += inFileName;

  out.open(outFilename.c_str());
}
// Opens the correct input file based on argv[1]
void open_in_file(char *argv1, std::ifstream &file) {
  file.open(argv1);
   if(!file.good()) {
    std::cout << "Invalid file." << std::endl;
  }
}

// Check for incorrect usage, and correct it if not.
void usage_message(int argc, char **argv) {
  if(argc != 2) {
    std::cout << "Usage:\n\t" << argv[0] << " <filename>" << std::endl;
    exit(1);
  }
}

// Read in a triangle from the input file
Polygon_2 read_triangle(std::ifstream &file) {
    float fs[2];
    Polygon_2 triangle;

    for(int i=0; i<3; i++) {
      if(!file.good()) {
        throw "Error: No more integers";
      }
      file >> fs[0];
      if(!file.good()) {
        throw "Error: No more integers";
      }
       file >> fs[1];

       triangle.push_back(Point_2(fs[0], fs[1]));
    }
    return triangle;
}

int main(int argc, char **argv)
{
  usage_message(argc, argv);

  std::ofstream out; open_out_file(argv[1], out);
  std::ifstream in; open_in_file(argv[1], in);

  while(true) {
    try {
      Polygon_2
        t_1 = read_triangle(in),
        t_2 = read_triangle(in);
      Polygon_with_holes_2 sum = CGAL::minkowski_sum_2(t_1, t_2);
      std::ostream_iterator< Point_2 >  out_iter( out, " " );

      CGAL_assertion(sum.number_of_holes() == 0);
      CGAL::ch_graham_andrew( sum.outer_boundary().vertices_begin(), sum.outer_boundary().vertices_end(), out_iter );

      out << std::endl;

    } catch(const char *e) {
      break;
    }
  }

  in.close();
  out.close();
  return 0;
}

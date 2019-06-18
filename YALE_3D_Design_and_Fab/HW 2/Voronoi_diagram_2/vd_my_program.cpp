// standard includes
#include <iostream>
#include <fstream>
#include <cassert>

// includes for defining the Voronoi diagram adaptor
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Voronoi_diagram_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_traits_2.h>
#include <CGAL/Delaunay_triangulation_adaptation_policies_2.h>

// typedefs for defining the adaptor
typedef CGAL::Exact_predicates_inexact_constructions_kernel                  K;
typedef CGAL::Delaunay_triangulation_2<K>                                    DT;
typedef CGAL::Delaunay_triangulation_adaptation_traits_2<DT>                 AT;
typedef CGAL::Delaunay_triangulation_caching_degeneracy_removal_policy_2<DT> AP;
typedef CGAL::Voronoi_diagram_2<DT,AT,AP>                                    VD;

// typedef for the result type of the point location
typedef AT::Site_2                    Site_2;
typedef AT::Point_2                   Point_2;

typedef VD::Locate_result             Locate_result;
typedef VD::Vertex_handle             Vertex_handle;
typedef VD::Face_handle               Face_handle;
typedef VD::Halfedge_handle           Halfedge_handle;
typedef VD::Ccb_halfedge_circulator   Ccb_halfedge_circulator;
typedef VD::Halfedge_around_vertex_circulator Halfedge_around_vertex_circulator;
void print_endpoint(Halfedge_handle e, bool is_src) {
  std::cout << "(";
  if ( is_src ) {
    if ( e->has_source() )  std::cout << e->source()->point();
    else  std::cout << "inf";
  } else {
    if ( e->has_target() )  std::cout << e->target()->point();
    else  std::cout << "inf";
  }
  std::cout << "), " << std::flush;
}

void print_face(Face_handle f) {
	if(f->is_unbounded()) {
		std::cout << "An unbounded face with points: " << std::flush;
	} else {
		std::cout << "A face with points: " << std::flush;
	}
	Ccb_halfedge_circulator ec_start = f->ccb();
	Ccb_halfedge_circulator ec = ec_start;

	do {
	  print_endpoint(ec, false);
	} while ( ++ec != ec_start );		

	std::cout << std::endl;
}

int main()
{
  std::ifstream ifs("data/data1.dt.cin");
  assert( ifs );

  VD vd;

  Site_2 t;
  while ( ifs >> t ) { vd.insert(t); }
  ifs.close();

  assert( vd.is_valid() );

  std::ifstream ifq("data/queries1.dt.cin");
  assert( ifq );

  Point_2 p;
  while ( ifq >> p ) {
  	std::cout << "Point (" << p.x() << "," << p.y() << ") lies " << std::flush;

    Locate_result lr = vd.locate(p);
    if ( Vertex_handle* v = boost::get<Vertex_handle>(&lr) ) {
      std::cout << "between several faces:" << std::endl;

      // It's between the following faces:
      Halfedge_around_vertex_circulator c_start = (*v)->incident_halfedges();
      Halfedge_around_vertex_circulator c = c_start;
      do {
      	//We've got an edge!
      	print_face(((Halfedge_handle) c)->face());
      } while (++c != c_start);

    } else if ( Halfedge_handle* e = boost::get<Halfedge_handle>(&lr) ) {
      std::cout << "on the boundary between two faces:" << std::endl;

      // Do something with these:
      print_face((*e)->face()); // Face_handle
      print_face((*e)->twin()->face());

    } else if ( Face_handle* f = boost::get<Face_handle>(&lr) ) {
      std::cout << "in a face:" << std::endl;
      print_face(*f);
  	}
  }
  ifq.close();

  return 0;
}

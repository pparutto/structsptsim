#include <boost/polygon/polygon.hpp>
#include <cassert>
namespace gtl = boost::polygon;
using namespace boost::polygon::operators;

int main()
{
    //lets construct a 10x10 rectangle shaped polygon
    typedef gtl::polygon_data<double> Polygon;
    typedef gtl::polygon_traits<Polygon>::point_type Point;
    Point pts[] = {gtl::construct<Point>(0, 0),
		   gtl::construct<Point>(10, 0),
		   gtl::construct<Point>(10, 10),
		   gtl::construct<Point>(0, 10) };
    Polygon poly;
    gtl::set_points(poly, pts, pts+4);

    //now lets see what we can do with this polygon
    assert(gtl::area(poly) == 100.0f);
    assert(gtl::contains(poly, gtl::construct<Point>(5, 5)));
    assert(!gtl::contains(poly, gtl::construct<Point>(15, 5)));
    gtl::rectangle_data<double> rect;
    assert(gtl::extents(rect, poly)); //get bounding box of poly
    assert(gtl::equivalence(rect, poly)); //hey, that's slick
    assert(gtl::winding(poly) == gtl::COUNTERCLOCKWISE);
    assert(gtl::perimeter(poly) == 40.0f);

    //add 5 to all coords of poly
    gtl::convolve(poly, gtl::construct<Point>(5, 5));
    //multiply all coords of poly by 2
    gtl::scale_up(poly, 2);
    gtl::set_points(rect, gtl::point_data<double>(10, 10),
    gtl::point_data<double>(30, 30));
    assert(gtl::equivalence(poly, rect));


    Point hole_pts[] = {gtl::construct<Point>(2, 2),
			gtl::construct<Point>(8, 2),
			gtl::construct<Point>(8, 8),
			gtl::construct<Point>(2, 8)};
    Polygon hole_polys[] = {Polygon()};
    gtl::set_points(hole_polys[0], hole_pts, hole_pts+4);

    typedef gtl::polygon_with_holes_data<double> CompoundPolygon;
    CompoundPolygon cpoly(pts, pts+4, hole_polys, hole_polys+1);

    assert(gtl::contains(cpoly, gtl::construct<Point>(1, 1)));
    assert(!gtl::contains(cpoly, gtl::construct<Point>(4, 4)));

    std::cout << "DONE" << std::endl;
    return 0;
}

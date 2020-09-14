#include <iostream>
#include <iomanip>
#include <cassert>

#include <vector>

#include "shape.hh"
#include "segment.hh"
#include "collider.hh"

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

  std::cout << std::setprecision(9);

  std::cout << "Colinear:" << std::endl;
  {
    Point p1 = {1, 1};
    Point p2 = {10, 1};
    Point p3 = {1, 2};

    std::cout << " [Test 1]";
    assert(!colinear(p1, p2, p3));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 2]";
    p3 = {5, 1};
    assert(colinear(p1, p2, p3));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 3]";
    p1 = {0, 0};
    p2 = {5, 5};
    p3 = {3, 3};
    assert(colinear(p1, p2, p3));
    std::cout << " OK" << std::endl;
  }

  std::cout << "Orientation:" << std::endl;
  {
    Point p1 = {1, 1};
    Point q1 = {10, 1};
    Point p2 = {1, 2};

    std::cout << " [Test 1]";
    assert(orientation(p1, q1, p2) == 2);
    std::cout << " OK" << std::endl;

    std::cout << " [Test 2]";
    p2 = {5, 1};
    assert(orientation(p1, q1, p2) == 0);
    std::cout << " OK" << std::endl;
  }

  std::cout << "Segments:" << std::endl;
  {
    Segment s1({1, 1}, {10, 1});
    Segment s2({1, 2}, {10, 2});

    std::cout << " [Test 1]";
    assert(!s1.on_segment(s2.p1()));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 2]";
    assert(!s1.on_segment(s2.p2()));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 3]";
    assert(!Segment::intersect(s1, s2));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 4]";
    s1 = Segment({10, 0}, {0, 10});
    s2 = Segment({0, 0}, {10, 10});
    assert(Segment::intersect(s1, s2));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 5]";
    s1 = Segment({-5, 5}, {0, 0});
    s2 = Segment({1, 1}, {10, 10});
    assert(!Segment::intersect(s1, s2));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    s1 = Segment({0, 0}, {5, 5});
    s2 = Segment({2, 1}, {10000.0, 1});
    assert(!Segment::intersect(s1, s2));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 7]";
    s1 = Segment({0, 0}, {5, 5});
    assert(s1.on_segment({3, 3}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 8]";
    s1 = Segment({0, 0}, {5, 5});
    s2 = Segment({3, 3}, {10000.0, 3});
    assert(Segment::intersect(s1, s2));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 9]";
    s1 = Segment({10, 10}, {0, 10});
    assert(!s1.on_segment({-1, 10}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 10]";
    s1 = Segment({10, 10}, {10, 0});
    assert(Segment::intersect(s1, Segment({5, 5}, {15, 5})));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 11]";
    s1 = Segment({5, 5}, {5, -5});
    s2 = Segment({0, 0}, {10, 0});
    Point res = Segment::intersection_point(s1, s2);
    assert((res == (Point) {5.0, 0.0}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 12]";
    s1 = Segment({5, 0}, {5, 10});

    assert((s1.normal()  == (Vec) {-1, 0}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 13]";
    s1 = Segment({2, 2}, {6, 6});
    s2 = Segment({5, 0}, {5, 10});
    res = Segment::reflect(s1, s2);
    assert((res == (Point) {4, 6}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 14]";
    s1 = Segment({2, 2}, {8, 2});
    s2 = Segment({5, 0}, {5, 10});
    res = Segment::intersection_point(s1, s2);
    assert((res == (Point) {5, 2}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 15]";
    s1 = Segment({2, 2}, {8, 2});
    s2 = Segment({5, 0}, {5, 10});
    res = Segment::reflect(s1, s2);
    assert((res == (Point) {2, 2}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 16]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    s2 = Segment({50.6049, 129.478}, {50.4045, 120.423});
    assert(Segment::intersect(s1, s2));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 17]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    s2 = Segment({50.6049, 129.478}, {50.4045, 120.423});
    res = Segment::intersection_point(s1, s2);
    assert((res == (Point) {50.6012629, 129.31366}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 18]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    s2 = Segment({50.6049, 129.478}, {50.4045, 120.423});
    res = Segment::reflect(s1, s2);
    assert((res == (Point) {51.8634543, 130.054885}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 19]";
    s1 = Segment({50.6012629, 129.31366}, {51.8634543, 130.054885});
    s2 = Segment({102.054, 129.746}, {50.6049, 129.478});
    assert(Segment::intersect(s1, s2));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 20]";
    s1 = Segment({50.6012629, 129.31366}, {51.8634543, 130.054885});
    s2 = Segment({102.054, 129.746}, {50.6049, 129.478});
    res = Segment::intersection_point(s1, s2);
    assert((res == (Point) {50.8835804, 129.479452}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 21]";
    s1 = Segment({50.6012629, 129.31366}, {51.8634543, 130.054885});
    s2 = Segment({102.054, 129.746}, {50.6049, 129.478});
    res = Segment::reflect(s1, s2);
    assert((res == (Point) {51.8693959, 128.914258}));
    std::cout << " OK" << std::endl;

  }

  std::cout << "Polygons:" << std::endl;
  {
    std::cout << " [Test 1]";
    PointEnsemble pe;
    pe.push_back({0, 0});
    pe.push_back({10, 0});
    pe.push_back({10, 10});
    pe.push_back({0, 10});

    Polygon poly(pe);
    Point p = {20, 20};

    assert(!poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 2]";
    p = {5, 5};
    assert(poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 3]";
    p = {-1, 10};
    assert(!poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 4]";
    Segment res = poly.intersect_with(Segment({5, 5}, {15, 5}));
    assert((res.p1() == (Point) {10, 0}));
    assert((res.p2() == (Point) {10, 10}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 5]";
    res = poly.intersect_with(Segment({5, 5}, {5, 15}));
    assert((res.p1() == (Point) {10, 10}));
    assert((res.p2() == (Point) {0, 10}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    res = poly.intersect_with(Segment({-5, 5}, {5, 5}));
    assert((res.p1() == (Point) {0, 10}));
    assert((res.p2() == (Point) {0, 0}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 7]";
    res = poly.intersect_with(Segment({5, -5}, {5, 5}));
    assert((res.p1() == (Point) {0, 0}));
    assert((res.p2() == (Point) {10, 0}));
    std::cout << " OK" << std::endl;
  }

  {
    PointEnsemble pe;
    pe.push_back({0, 0});
    pe.push_back({5, 5});
    pe.push_back({5, 0});

    std::cout << " [Test 8]";
    Polygon poly(pe);
    Point p = {2, 2};
    assert(poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 9]";
    p = {5, 1};
    assert(poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 10]";
    p = {8, 1};
    assert(!poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 11]";
    Segment res = poly.intersect_with(Segment({3.1, 3}, {-2, 8}));
    assert((res.p1() == (Point) {0, 0}));
    assert((res.p2() == (Point) {5, 5}));
    std::cout << " OK" << std::endl;
  }

  std::cout << "U polygon" << std::endl;
  {
    PointEnsemble pe;
    pe.push_back({116.984, 93.7433});
    pe.push_back({115.518, 117.399});
    pe.push_back({102.054, 129.746});
    pe.push_back({50.6049, 129.478});
    pe.push_back({50.4045, 120.423});
    pe.push_back({100.731, 120.958});
    pe.push_back({107.592, 112.292});
    pe.push_back({108.323, 96.5429});
    pe.push_back({96.8922, 86.4158});
    pe.push_back({51.965, 86.8721});
    pe.push_back({53.1677, 78.5918});
    pe.push_back({99.3132, 78.0573});
    pe.push_back({116.984, 93.7433});
    Polygon u_poly(pe);
    PolygonCollider poly_collider(u_poly);

    std::cout << " [Test 1]";
    Point p = {107.65, 109.702};
    assert(!u_poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 2]";
    Segment s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    Segment s2 = u_poly.intersect_with(s1);
    assert((s2.p1() == (Point) {107.592, 112.292})
	   && (s2.p2() == (Point) {108.323, 96.5429}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 3]";
    s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    s2 = u_poly.intersect_with(s1);
    p = Segment::intersection_point(s1, s2);
    assert((p == (Point) {107.709932, 109.751202}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 4]";
    s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    p = poly_collider.collide(s1.p1(), s1.p2());
    assert((p == (Point) {107.774349, 109.705772}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 5]";
    s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    s2 = Segment({107.592, 112.292}, {108.323, 96.5429});
    Point p1 = poly_collider.collide(s1.p1(), s1.p2());
    Point p2 = Segment::reflect(s1, s2);
    assert((p1 == p2));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    p1 = poly_collider.collide(s1.p1(), s1.p2());
    assert((p1 == (Point) {51.8693959, 128.914258}));
    std::cout << " OK" << std::endl;
  }

  std::cout << "Compound polygon" << std::endl;
  {
    PointEnsemble pe;
    pe.push_back({5, 5});
    pe.push_back({10, 5});
    pe.push_back({10, 10});
    pe.push_back({5, 10});
    Polygon base_poly(pe);

    pe.clear();
    pe.push_back({6, 6});
    pe.push_back({9, 6});
    pe.push_back({9, 9});
    pe.push_back({6, 9});
    std::vector<Polygon> diff_polys;
    diff_polys.push_back(Polygon(pe));

    CompoundPolygon poly(base_poly, diff_polys);
    PolygonCollider poly_collider(poly);
    (void) poly_collider;

    std::cout << " [Test 1]";
    assert(poly.inside({5.5, 5.5}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 2]";
    assert(!poly.inside({20, 5.5}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 3]";
    assert(!poly.inside({7, 7}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 4]";
    assert(diff_polys[0].inside({7, 7}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 5]";
    Segment res = poly.intersect_with(Segment({9.5, 5.5}, {12, 5.5}));
    assert((res.p1() == (Point) {10, 5}) && (res.p2() == (Point) {10, 10}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    res = poly.intersect_with(Segment({5.8, 7.1}, {6.1, 7.1}));
    assert((res.p1() == (Point) {6, 6}) && (res.p2() == (Point) {6, 9}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 7]";
    Point p = Segment::reflect(Segment({5.8, 7.1}, {6.1, 7.1}),
			       Segment({6, 6}, {6, 9}));
    assert(poly.inside(p));
    assert((p == (Point) {5.9, 7.1}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 8]";
    p = poly_collider.collide({5.8, 7.1}, {6.1, 7.1});
    assert(poly.inside(p));
    assert((p == (Point) {5.9, 7.1}));
    std::cout << " OK" << std::endl;

    // std::cout << " [Test 2]";
    // Segment s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    // Segment s2 = u_poly.intersect_with(s1);
    // assert((s2.p1() == (Point) {107.592, 112.292})
    // 	   && (s2.p2() == (Point) {108.323, 96.5429}));
    // std::cout << " OK" << std::endl;

    // std::cout << " [Test 3]";
    // s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    // s2 = u_poly.intersect_with(s1);
    // p = Segment::intersection_point(s1, s2);
    // assert((p == (Point) {107.709932, 109.751202}));
    // std::cout << " OK" << std::endl;

    // std::cout << " [Test 4]";
    // s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    // p = poly_collider.collide(s1.p1(), s1.p2());
    // assert((p == (Point) {107.774349, 109.705772}));
    // std::cout << " OK" << std::endl;

    // std::cout << " [Test 5]";
    // s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    // s2 = Segment({107.592, 112.292}, {108.323, 96.5429});
    // Point p1 = poly_collider.collide(s1.p1(), s1.p2());
    // Point p2 = Segment::reflect(s1, s2);
    // assert((p1 == p2));
    // std::cout << " OK" << std::endl;

    // std::cout << " [Test 6]";
    // s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    // p1 = poly_collider.collide(s1.p1(), s1.p2());
    // assert((p1 == (Point) {51.8693959, 128.914258}));
    // std::cout << " OK" << std::endl;
  }
}

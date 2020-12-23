#include <iostream>
#include <iomanip>
#include <cassert>

#include <vector>
#include <cfloat>

#include "shape.hh"
#include "segment.hh"
#include "collider.hh"
#include "io.hh"

double PRECISION = 1e-6;

bool pts_eq(const Point& p1, const Point& p2, double prec)
{
  return std::abs(p1[0] - p2[0]) < prec && std::abs(p1[1] - p2[1]) < prec;
}

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

  std::cout << std::setprecision(15);

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
    assert(pts_eq(res, {50.6012629, 129.31366}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 18]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    s2 = Segment({50.6049, 129.478}, {50.4045, 120.423});
    res = Segment::reflect(s1, s2);
    assert(pts_eq(res, {51.8634543, 130.054885}, PRECISION));
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
    assert(pts_eq(res, {50.8835804, 129.479452}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 21]";
    s1 = Segment({50.6012629, 129.31366}, {51.8634543, 130.054885});
    s2 = Segment({102.054, 129.746}, {50.6049, 129.478});
    res = Segment::reflect(s1, s2);
    assert(pts_eq(res, {51.8693959, 128.914258}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 22]";
    s1 = Segment({14.1584, 10.0646}, {14.1983, 10.0911});
    s2 = Segment({14.0766, 10.1441}, {14.2188, 10.0059});
    res = Segment::intersection_point(s1, s2);
    assert(pts_eq(res, {14.1584006, 10.0646004}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 23]";
    s1 = Segment({14.1584, 10.0646}, {14.1983, 10.0911});
    s2 = Segment({14.0766, 10.1441}, {14.2188, 10.0059});
    res = Segment::reflect(s1, s2);
    assert(pts_eq(res, {14.1330499, 10.0239613}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 24]";
    s1 = Segment({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment({8.69375, 0.954687}, {8.775, 0.979062});
    assert(Segment::intersect(s1, s2));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 25]";
    s1 = Segment({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment({8.69375, 0.954687}, {8.775, 0.979062});
    res = Segment::intersection_point(s1, s2);
    assert(pts_eq(res, {8.76027862, 0.974645586}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 26]";
    s1 = Segment({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment({8.69375, 0.954687}, {8.775, 0.979062});
    assert(dot(s1.vector(), s2.normal()) < 0);
    std::cout << " OK" << std::endl;

    std::cout << " [Test 27]";
    s1 = Segment({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment({8.69375, 0.954687}, {8.775, 0.979062});
    res = Segment::reflect(s1, s2);
    assert(pts_eq(res, {8.72978972, 0.965553917}, PRECISION));
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

    pe.clear();
    pe.push_back({8.01938, 9.62406});
    pe.push_back({8.03969, 9.65656});
    pe.push_back({7.95438, 9.86781});
    pe.push_back({7.865, 10.0791});
    pe.push_back({7.73906, 10.0669});
    pe.push_back({7.61719, 10.0547});
    pe.push_back({7.62531, 9.95312});
    pe.push_back({7.6375, 9.85156});
    pe.push_back({7.78781, 9.7175});
    pe.push_back({7.93406, 9.5875});
    pe.push_back({7.96656, 9.5875});
    pe.push_back({7.99906, 9.5875});
    pe.push_back({8.01938, 9.62406});
    poly = Polygon(pe);

    std::cout << poly << std::endl;

    std::cout << "colin= " << colinear({8.01938, 9.62406}, {8.01938, 9.62406}, {8.03189, 9.64409}) << std::endl;
    Segment s1 = Segment({8.01938, 9.62406}, {8.01938, 9.62406});
    std::cout << "on= " << s1.on_segment({8.03189, 9.64409}) << std::endl;
    std::cout << " [Test 8]";
    assert(poly.inside({8.03189, 9.64409}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 9]";
    assert(!poly.inside({8.03204, 9.6443}));
    std::cout << " OK" << std::endl;

    pe.clear();
    pe.push_back({6.72344, 0.154375});
    pe.push_back({6.64219, 7.21645e-17});
    pe.push_back({6.51219, 7.21645e-17});
    poly = Polygon(pe);

    std::cout << " [Test 10]";
    assert(poly.inside({6.64591, 0.00707282}));
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

    for (Point p: pe)
      std::cout << p[0] << ", " << p[1] << ";" << std::endl;

    std::cout << " [Test 1]";
    Point p = {107.65, 109.702};
    assert(!u_poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 2]";
    Segment s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    Segment s2 = u_poly.intersect_with(s1);
    assert(pts_eq(s2.p1(), {107.592, 112.292}, PRECISION) &&
	   pts_eq(s2.p2(), {108.323, 96.5429}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 3]";
    s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    s2 = u_poly.intersect_with(s1);
    p = Segment::intersection_point(s1, s2);
    assert(pts_eq(p, {107.709932, 109.751202}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 4]";
    s1 = Segment({108.0070, 110.005}, {107.65, 109.7});
    p = poly_collider.collide(s1.p1(), s1.p2());
    assert(pts_eq(p, {107.774349, 109.705772}, PRECISION));
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
    assert(u_poly.inside(s1.p1()) && !u_poly.inside(s1.p2()));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 7]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    s2 = u_poly.intersect_with(s1);
    assert(pts_eq(s2.p1(), {50.6049, 129.478}, PRECISION) &&
	   pts_eq(s2.p2(), {50.4045, 120.423}, PRECISION));
    std::cout << " OK" << std::endl;


    std::cout << " [Test 8]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    s2 = u_poly.intersect_with(s1);
    p1 = Segment::intersection_point(s1, s2);
    assert(s2.on_segment(p1));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 8]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    s2 = u_poly.intersect_with(s1);
    p1 = Segment::intersection_point(s1, s2);
    assert(pts_eq(p1, {50.6012629, 129.31366}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 9]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    s2 = u_poly.intersect_with(s1);
    p1 = Segment::intersection_point(s1, s2);
    assert(u_poly.inside(p1));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 10]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    s2 = u_poly.intersect_with(s1);
    p1 = Segment::reflect(s1, s2);
    assert(pts_eq(p1, {51.8634543, 130.054885}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 11]";
    assert(u_poly.inside({50.738, 129.225}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 12]";
    s1 = Segment({50.738, 129.225}, {51.8634543, 130.054885});
    s2 = u_poly.intersect_with(s1);
    assert(pts_eq(s2.p1(), {102.054, 129.746}, PRECISION) &&
	   pts_eq(s2.p2(), {50.6049, 129.478}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 13]";
    s1 = Segment({50.738, 129.225}, {51.8634543, 130.054885});
    s2 = Segment({102.054, 129.746}, {50.6049, 129.478});
    p1 = Segment::intersection_point(s1, s2);
    std::cout << p1 << std::endl;
    assert(pts_eq(p1, {51.0844957, 129.480498}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 14]";
    s1 = Segment({50.738, 129.225}, {51.8634543, 130.054885});
    s2 = Segment({102.054, 129.746}, {50.6049, 129.478});
    p1 = Segment::reflect(s1, s2);
    assert(pts_eq(p1, {51.8693959, 128.914258}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 15]";
    s1 = Segment({50.738, 129.225}, {51.8634543, 130.054885});
    s2 = Segment({102.054, 129.746}, {50.6049, 129.478});
    p1 = Segment::reflect(s1, s2);
    assert(u_poly.inside(p1));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 16]";
    s1 = Segment({50.738, 129.225}, {49.3731, 130.11});
    p1 = poly_collider.collide(s1.p1(), s1.p2());
    assert(pts_eq(p1, {51.8693959, 128.914258}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 17]";
    s1 = Segment({0.251875, 0}, {0.377813, 0});
    assert(s1.on_segment({0.347154, 0}));
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
  }

  std::cout << "Complex polygon" << std::endl;
  {
    std::vector<CompoundPolygon> polys =
      polys_from_inkscape_path("../resources/ER_net1.path");
    CompoundPolygon poly = polys[0];
    poly.apply_pxsize(0.0406250);

    //3.9040625 5.3828125;
    //3.9 5.41125;
    //3.9 5.44375;

    std::cout << " [Test 9]";
    assert(poly.inside({1.18408, 5.41125}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 10]";
    assert(poly.inside({7.10528, 2.98698}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 11]";
    assert(poly.inside({7.15607, 2.96563}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 12]";
    assert(poly.inside({3.86121, 0.73125}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 13]";
    assert(poly.inside({6.84829, 7.4425}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 13]";
    assert(poly.inside({1.86335, 2.31562}));
    std::cout << " OK" << std::endl;
  }
}

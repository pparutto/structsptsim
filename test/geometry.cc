#include <iostream>
#include <iomanip>
#include <cassert>

#include <vector>
#include <cfloat>

#include "shape.hh"
#include "segment.hh"
#include "collider.hh"
#include "io.hh"

bool pts_eq(const Point<2>& p1, const Point<2>& p2, double prec)
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
    Point<2> p1 = {1, 1};
    Point<2> p2 = {10, 1};
    Point<2> p3 = {1, 2};

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
    Point<2> p1 = {1, 1};
    Point<2> q1 = {10, 1};
    Point<2> p2 = {1, 2};

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
    Segment<2> s1({1, 1}, {10, 1});
    Segment<2> s2({1, 2}, {10, 2});
    Point<2> inter_p;
    Point<2> res;

    std::cout << " [Test 1]";
    assert(!s1.on_segment(s2.p1()));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 2]";
    assert(!s1.on_segment(s2.p2()));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 3]";
    assert(!s1.intersect(s2, inter_p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 4]";
    s1 = Segment<2>({10, 0}, {0, 10});
    s2 = Segment<2>({0, 0}, {10, 10});
    assert(s1.intersect(s2, inter_p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 5]";
    s1 = Segment<2>({-5, 5}, {0, 0});
    s2 = Segment<2>({1, 1}, {10, 10});
    assert(!s1.intersect(s2, inter_p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    s1 = Segment<2>({0, 0}, {5, 5});
    s2 = Segment<2>({2, 1}, {10000.0, 1});
    assert(!s1.intersect(s2, inter_p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 7]";
    s1 = Segment<2>({0, 0}, {5, 5});
    assert(s1.on_segment({3, 3}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 8]";
    s1 = Segment<2>({0, 0}, {5, 5});
    s2 = Segment<2>({3, 3}, {10000.0, 3});
    assert(s1.intersect(s2, inter_p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 9]";
    s1 = Segment<2>({10, 10}, {0, 10});
    assert(!s1.on_segment({-1, 10}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 10]";
    s1 = Segment<2>({10, 10}, {10, 0});
    assert(s1.intersect(Segment<2>({5, 5}, {15, 5}), inter_p));
    std::cout << " OK" << std::endl;

    Point<2> p = {20, 20};
    Point<2> p2 = {20, 20};
    std::cout << (p == p2) << std::endl;

    std::cout << " [Test 11]";
    s1 = Segment<2>({5, 5}, {5, -5});
    s2 = Segment<2>({0, 0}, {10, 0});
    s1.intersect(s2, inter_p);
    assert((inter_p == (Point<2>) {5.0, 0.0}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 12]";
    s1 = Segment<2>({5, 0}, {5, 10});

    assert((s1.normal()  == (Vec<2>) {-1, 0}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 13]";
    s1 = Segment<2>({2, 2}, {6, 6});
    s2 = Segment<2>({5, 0}, {5, 10});
    s1.intersect(s2, inter_p);
    res = Segment<2>::reflect(s1, s2, inter_p);
    assert((res == (Point<2>) {4, 6}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 14]";
    s1 = Segment<2>({2, 2}, {8, 2});
    s2 = Segment<2>({5, 0}, {5, 10});
    s1.intersect(s2, inter_p);
    assert((inter_p == (Point<2>) {5, 2}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 15]";
    s1 = Segment<2>({2, 2}, {8, 2});
    s2 = Segment<2>({5, 0}, {5, 10});
    s1.intersect(s2, inter_p);
    res = Segment<2>::reflect(s1, s2, inter_p);
    assert((res == (Point<2>) {2, 2}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 16]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    s2 = Segment<2>({50.6049, 129.478}, {50.4045, 120.423});
    assert(s1.intersect(s2, inter_p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 17]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    s2 = Segment<2>({50.6049, 129.478}, {50.4045, 120.423});
    s1.intersect(s2, inter_p);
    assert(pts_eq(inter_p, {50.6012629, 129.31366}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 18]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    s2 = Segment<2>({50.6049, 129.478}, {50.4045, 120.423});
    s1.intersect(s2, inter_p);
    res = Segment<2>::reflect(s1, s2, inter_p);
    assert(pts_eq(res, {51.8634543, 130.054885}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 19]";
    s1 = Segment<2>({50.6012629, 129.31366}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    assert(s1.intersect(s2, inter_p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 20]";
    s1 = Segment<2>({50.6012629, 129.31366}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    s1.intersect(s2, inter_p);
    assert(pts_eq(inter_p, {50.8835804, 129.479452}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 21]";
    s1 = Segment<2>({50.6012629, 129.31366}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    s1.intersect(s2, inter_p);
    res = Segment<2>::reflect(s1, s2, inter_p);
    assert(pts_eq(res, {51.8693959, 128.914258}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 22]";
    s1 = Segment<2>({14.1584, 10.0646}, {14.1983, 10.0911});
    s2 = Segment<2>({14.0766, 10.1441}, {14.2188, 10.0059});
    s1.intersect(s2, inter_p);
    assert(pts_eq(inter_p, {14.1584006, 10.0646004}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 23]";
    s1 = Segment<2>({14.1584, 10.0646}, {14.1983, 10.0911});
    s2 = Segment<2>({14.0766, 10.1441}, {14.2188, 10.0059});
    s1.intersect(s2, inter_p);
    res = Segment<2>::reflect(s1, s2, inter_p);
    assert(pts_eq(res, {14.1330499, 10.0239613}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 24]";
    s1 = Segment<2>({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment<2>({8.69375, 0.954687}, {8.775, 0.979062});
    assert(s1.intersect(s2, inter_p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 25]";
    s1 = Segment<2>({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment<2>({8.69375, 0.954687}, {8.775, 0.979062});
    s1.intersect(s2, inter_p);
    assert(pts_eq(inter_p, {8.76027862, 0.974645586}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 26]";
    s1 = Segment<2>({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment<2>({8.69375, 0.954687}, {8.775, 0.979062});
    assert(dot(s1.vector(), s2.normal()) < 0);
    std::cout << " OK" << std::endl;

    std::cout << " [Test 27]";
    s1 = Segment<2>({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment<2>({8.69375, 0.954687}, {8.775, 0.979062});
    s1.intersect(s2, inter_p);
    res = Segment<2>::reflect(s1, s2, inter_p);
    assert(pts_eq(res, {8.72978972, 0.965553917}, PRECISION));
    std::cout << " OK" << std::endl;
  }

  std::cout << "Polygons:" << std::endl;
  {
    std::cout << " [Test 1]";
    Point<2> inter_p;
    Segment<2> inter_s;

    PointEnsemble<2> pe;
    pe.push_back({0, 0});
    pe.push_back({10, 0});
    pe.push_back({10, 10});
    pe.push_back({0, 10});

    Polygon poly(pe);
    Point<2> p = {20, 20};

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
    poly.intersect(Segment<2>({5, 5}, {15, 5}), inter_p, inter_s);
    assert((inter_s.p1() == (Point<2>) {10, 0}));
    assert((inter_s.p2() == (Point<2>) {10, 10}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 5]";
    poly.intersect(Segment<2>({5, 5}, {5, 15}), inter_p, inter_s);
    assert((inter_s.p1() == (Point<2>) {10, 10}));
    assert((inter_s.p2() == (Point<2>) {0, 10}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    poly.intersect(Segment<2>({-5, 5}, {5, 5}), inter_p, inter_s);
    assert((inter_s.p1() == (Point<2>) {0, 10}));
    assert((inter_s.p2() == (Point<2>) {0, 0}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 7]";
    poly.intersect(Segment<2>({5, -5}, {5, 5}), inter_p, inter_s);
    assert((inter_s.p1() == (Point<2>) {0, 0}));
    assert((inter_s.p2() == (Point<2>) {10, 0}));
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
    Point<2> inter_p;
    Segment<2> inter_s;

    PointEnsemble<2> pe;
    pe.push_back({0, 0});
    pe.push_back({5, 5});
    pe.push_back({5, 0});

    std::cout << " [Test 11]";
    Polygon poly(pe);
    Point<2> p = {2, 2};
    assert(poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 12]";
    p = {5, 1};
    assert(poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 13]";
    p = {8, 1};
    assert(!poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 14]";
    poly.intersect(Segment<2>({3.1, 3}, {-2, 8}), inter_p, inter_s);
    assert((inter_s.p1() == (Point<2>) {0, 0}));
    assert((inter_s.p2() == (Point<2>) {5, 5}));
    std::cout << " OK" << std::endl;
  }

  std::cout << "U polygon" << std::endl;
  {
    Point<2> inter_p;
    Segment<2> inter_s;
    Segment<2> s2;

    PointEnsemble<2> pe;
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
    Point<2> p = {107.65, 109.702};
    assert(!u_poly.inside(p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 2]";
    Segment<2> s1({108.0070, 110.005}, {107.65, 109.7});
    u_poly.intersect(s1, inter_p, inter_s);
    assert(pts_eq(inter_s.p1(), {107.592, 112.292}, PRECISION) &&
  	   pts_eq(inter_s.p2(), {108.323, 96.5429}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 3]";
    s1 = Segment<2>({108.0070, 110.005}, {107.65, 109.7});
    u_poly.intersect(s1, inter_p, inter_s);
    assert(pts_eq(inter_p, {107.709932, 109.751202}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 4]";
    s1 = Segment<2>({108.0070, 110.005}, {107.65, 109.7});
    p = poly_collider.collide(s1.p1(), s1.p2());
    std::cout << p << std::endl;
    assert(pts_eq(p, {107.774349, 109.705772}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 5]";
    s1 = Segment<2>({108.0070, 110.005}, {107.65, 109.7});
    s2 = Segment<2>({107.592, 112.292}, {108.323, 96.5429});
    Point<2> p1 = poly_collider.collide(s1.p1(), s1.p2());
    s1.intersect(s2, inter_p);
    Point<2> p2 = Segment<2>::reflect(s1, s2, inter_p);
    assert((p1 == p2));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    assert(u_poly.inside(s1.p1()) && !u_poly.inside(s1.p2()));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 7]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    u_poly.intersect(s1, inter_p, inter_s);
    assert(pts_eq(inter_s.p1(), {50.6049, 129.478}, PRECISION) &&
  	   pts_eq(inter_s.p2(), {50.4045, 120.423}, PRECISION));
    std::cout << " OK" << std::endl;


    std::cout << " [Test 8]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    u_poly.intersect(s1, inter_p, inter_s);
    assert(s1.on_segment(inter_p)); //not sure?
    std::cout << " OK" << std::endl;

    std::cout << " [Test 9]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    u_poly.intersect(s1, inter_p, inter_s);
    assert(pts_eq(inter_p, {50.6012629, 129.31366}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 10]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    u_poly.intersect(s1, inter_p, inter_s);
    assert(u_poly.inside(inter_p));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 11]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    u_poly.intersect(s1, inter_p, inter_s);
    p1 = Segment<2>::reflect(s1, s2, inter_p);
    assert(pts_eq(p1, {51.8634543, 130.054885}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 12]";
    assert(u_poly.inside({50.738, 129.225}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 13]";
    s1 = Segment<2>({50.738, 129.225}, {51.8634543, 130.054885});
    u_poly.intersect(s1, inter_p, inter_s);
    assert(pts_eq(inter_s.p1(), {102.054, 129.746}, PRECISION) &&
  	   pts_eq(inter_s.p2(), {50.6049, 129.478}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 14]";
    s1 = Segment<2>({50.738, 129.225}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    s1.intersect(s2, inter_p);
    assert(pts_eq(inter_p, {51.0844957, 129.480498}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 15]";
    s1 = Segment<2>({50.738, 129.225}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    s1.intersect(s2, inter_p);
    p1 = Segment<2>::reflect(s1, s2, inter_p);
    assert(pts_eq(p1, {51.8693959, 128.914258}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 16]";
    s1 = Segment<2>({50.738, 129.225}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    s1.intersect(s2, inter_p);
    p1 = Segment<2>::reflect(s1, s2, inter_p);
    assert(u_poly.inside(p1));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 17]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    p1 = poly_collider.collide(s1.p1(), s1.p2());
    assert(pts_eq(p1, {51.8693959, 128.914258}, PRECISION));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 18]";
    s1 = Segment<2>({0.251875, 0}, {0.377813, 0});
    assert(s1.on_segment({0.347154, 0}));
    std::cout << " OK" << std::endl;

  }

  std::cout << "Compound polygon" << std::endl;
  {
    Point<2> inter_p;
    Segment<2> inter_s;

    PointEnsemble<2> pe;
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
    poly.intersect(Segment<2>({9.5, 5.5}, {12, 5.5}), inter_p, inter_s);
    assert((inter_s.p1() == (Point<2>) {10, 5}) && (inter_s.p2() == (Point<2>) {10, 10}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    poly.intersect(Segment<2>({5.8, 7.1}, {6.1, 7.1}), inter_p, inter_s);
    assert((inter_s.p1() == (Point<2>) {6, 9}) && (inter_s.p2() == (Point<2>) {6, 6}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 7]";
    Segment<2> s1 = Segment<2>({5.8, 7.1}, {6.1, 7.1});
    Segment<2> s2 = Segment<2>({6, 6}, {6, 9});
    s1.intersect(s2, inter_p);
    Point<2> p = Segment<2>::reflect(s1, s2, inter_p);
    assert(poly.inside(p));
    assert((p == (Point<2>) {5.9, 7.1}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 8]";
    p = poly_collider.collide({5.8, 7.1}, {6.1, 7.1});
    assert(poly.inside(p));
    assert((p == (Point<2>) {5.9, 7.1}));
    std::cout << " OK" << std::endl;
  }

  std::cout << "Compound polygon 2" << std::endl;
  {
    Point<2> inter_p;
    Segment<2> inter_s;

    PointEnsemble<2> pe;
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
    poly.intersect(Segment<2>({9.5, 5.5}, {12, 5.5}), inter_p, inter_s);
    assert((inter_s.p1() == (Point<2>) {10, 5}) && (inter_s.p2() == (Point<2>) {10, 10}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    poly.intersect(Segment<2>({5.8, 7.1}, {6.1, 7.1}), inter_p, inter_s);
    assert((inter_s.p1() == (Point<2>) {6, 9}) && (inter_s.p2() == (Point<2>) {6, 6}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 7]";
    Segment<2> s1({5.8, 7.1}, {6.1, 7.1});
    Segment<2> s2({6, 6}, {6, 9});
    s1.intersect(s2, inter_p);
    Point<2> p = Segment<2>::reflect(s1, s2, inter_p);
    assert(poly.inside(p));
    assert((p == (Point<2>) {5.9, 7.1}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 8]";
    p = poly_collider.collide({5.8, 7.1}, {6.1, 7.1});
    assert(poly.inside(p));
    assert((p == (Point<2>) {5.9, 7.1}));
    std::cout << " OK" << std::endl;
  }

  std::cout << "Complex polygon" << std::endl;
  {
    MultiplePolygon* polys =
      polys_from_inkscape_path("../resources/ER_net1.path");
    CompoundPolygon poly = polys->polys()[0];
    poly.apply_pxsize(0.0406250);

    Segment<2> s2({177, 277.1}, {177, 280.3});
    std::cout << " [Test 1]";
    assert(colinear(s2.p1(), s2.p2(), {177, 177.3}));
    std::cout << " OK" << std::endl;
    assert(colinear(s2.p1(), s2.p2(), {177, 10000}));

    std::cout << " [Test 2]";
    assert(polys->polys().size() == 1);
    std::cout << " OK" << std::endl;

    std::cout << " [Test 3]";
    assert(poly.inside({1.18408, 5.41125}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 4]";
    assert(poly.inside({7.10528, 2.98698}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 5]";
    assert(poly.inside({7.15607, 2.96563}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    assert(poly.inside({3.86121, 0.73125}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 7]";
    assert(poly.inside({6.84829, 7.4425}));
    std::cout << " OK" << std::endl;

    std::cout << " [Test 8]";
    assert(poly.inside({1.86335, 2.31562}));
    std::cout << " OK" << std::endl;
  }

  std::cout << "Touch inside" << std::endl;
  {
    MultiplePolygon* polys =
      polys_from_inkscape_path("../resources/poly_touch.poly");
    CompoundPolygon poly = polys->polys()[0];

    std::cout << " [Test 1]";
    assert(poly.inside((Point<2>) {481.9, 160}));
    std::cout << " OK" << std::endl;

    delete polys;
  }

  std::cout << "Complex polygon 2" << std::endl;
  {
    MultiplePolygon* polys =
      polys_from_inkscape_path("../resources/C2-_2019_10_02__16_23_00_mask_5073.poly");

    std::cout << " [Test 1]";
    assert(polys->polys().size() == 1);
    std::cout << " OK" << std::endl;

    delete polys;
  }

  std::cout << "Complex collide" << std::endl;
  {
    Segment<2> border({12.3784000000000, 9.35883000000000},
		      {12.4075000000000, 9.87177000000000});
    Segment<2> traj({12.4030, 9.8718}, {12.4108, 9.8685});

    Point<2> inter_p;
    border.intersect(traj, inter_p);
    Point<2> p = Segment<2>::reflect(traj, border, inter_p);
    (void) p; //NEED assert here
  }

  std::cout << "Poly L" << std::endl;
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/L2_poly.poly");

    std::cout << "[TEST1] ";
    assert(poly->polys()[0].inside((Point<2>) {71.059519, 105}));
    std::cout << "OK"  << std::endl;
  }

  std::cout << "Poly moon" << std::endl;
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/moon_poly.poly");

    std::cout << "[TEST1] ";
    assert(poly->polys()[0].inside((Point<2>) {90.60437, 41.0}));
    std::cout << "OK"  << std::endl;

    std::cout << "[TEST2] ";
    assert(poly->polys()[0].inside((Point<2>) {141.38558, 41.0}));
    std::cout << "OK"  << std::endl;
  }

  std::cout << "Poly colin in" << std::endl;
  {
    PointEnsemble<2> pe;
    pe.push_back({102, 130});
    pe.push_back({102, 119});
    pe.push_back({102, 108});
    pe.push_back({117, 104});
    pe.push_back({117, 84});
    pe.push_back({78, 84});
    pe.push_back({78, 141});
    pe.push_back({102, 130});
    std::vector<Polygon> diffs;
    CompoundPolygon poly(Polygon(pe), diffs);

    std::cout << "[Test 1]";
    assert(poly.inside((Point<2>) {93, 95}));
    std::cout << " OK" << std::endl;
  }

  std::cout << "Poly colin in left" << std::endl;
  {
    PointEnsemble<2> pe;
    pe.push_back({102, 130});
    pe.push_back({102, 119});
    pe.push_back({102, 108});
    pe.push_back({117, 104});
    pe.push_back({117, 84});
    pe.push_back({78, 84});
    pe.push_back({78, 141});
    std::vector<Polygon> diffs;
    CompoundPolygon poly(Polygon(pe), diffs);

    std::cout << "[Test 1]";
    assert(poly.inside((Point<2>) {102, 95}));
    std::cout << " OK" << std::endl;
  }

  std::cout << "Poly colin in 2 left" << std::endl;
  {
    PointEnsemble<2> pe;
    pe.push_back({102, 130});
    pe.push_back({102, 119});
    pe.push_back({102, 108});
    pe.push_back({93, 104});
    pe.push_back({117, 100});
    pe.push_back({117, 84});
    pe.push_back({78, 84});
    pe.push_back({78, 141});
    std::vector<Polygon> diffs;
    CompoundPolygon poly(Polygon(pe), diffs);

    std::cout << "[Test 1]";
    assert(poly.inside((Point<2>) {102, 95}));
    std::cout << " OK" << std::endl;
  }

    std::cout << "Poly Fail 1" << std::endl;
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/poly_fail.poly");

    std::cout << "[Test 1]";
    assert(poly->polys()[0].inside((Point<2>) {1194, 207.3}));
    std::cout << " OK" << std::endl;
    std::cout << "[Test 2]";
    assert(poly->polys().size() == 1);
    std::cout << " OK" << std::endl;
  }

  std::cout << "Poly Fail 2" << std::endl;
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/poly_fail2.poly");

    std::cout << "[Test 1]";
    assert(poly->polys()[0].inside((Point<2>) {1155, 545.3}));
    std::cout << " OK" << std::endl;
    std::cout << "[Test 2]";
    assert(poly->polys().size() == 1);
    std::cout << " OK" << std::endl;
  }

  std::cout << "ER complex poly" << std::endl;
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/ER9_VERYNICE_MMStack_Pos0.ome_Simple Segmentation_cleaned_binary_poly.poly");

    std::cout << "[Test 1]";
    assert(poly->polys()[0].inside((Point<2>) {1155, 545.3}));
    std::cout << " OK" << std::endl;
    std::cout << "[Test 2]";
    assert(poly->polys().size() == 1);
    std::cout << " OK" << std::endl;
  }
}

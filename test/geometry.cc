#include <iostream>
#include <cassert>

#include "shape.hh"
#include "segment.hh"

int main(int argc, char** argv)
{
  (void) argc;
  (void) argv;

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
    assert((res == (Point) {5, 0}));
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
    Segment res = poly.intersect_with(Segment({3, 3}, {-2, 8}));
    assert((res.p1() == (Point) {0, 0}));
    assert((res.p2() == (Point) {5, 5}));
    std::cout << " OK" << std::endl;
  }
}

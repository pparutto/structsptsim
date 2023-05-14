#include <iostream>
#include <iomanip>
#include <cassert>

#include <vector>
#include <cfloat>

#include <unordered_map>

#include "shape.hh"
#include "segment.hh"
#include "collider.hh"
#include "io.hh"

bool pts_eq(const Point<2>& p1, const Point<2>& p2, double prec)
{
  return std::abs(p1[0] - p2[0]) < prec && std::abs(p1[1] - p2[1]) < prec;
}

class Test
{
  const std::string name_;

public:
  Test(const std::string& name)
    : name_(name)
  {
  }

  virtual ~Test() = default;

  virtual bool check() const = 0;

  virtual const std::string& name() const
  {
    return this->name_;
  }
};

class Test1: public Test
{
public:
  Test1() : Test("QuadTree 1") {}

  virtual bool check() const
  {
    QuadTree qt = QuadTree(Box<2> ({0, 0},  {87.333, 81.399}));

    std::vector<Segment<2> > segs;
    segs.push_back(Segment<2>({19.1076, 48.7332}, {19.0873, 48.7135}));
    qt.insert_segments(segs, 5);

    Segment<2> s({19.092, 48.98775}, {19.092, 48.7104});
    Point<2> inter_p;
    Segment<2> inter_s;

    return qt.intersect(s, inter_p, inter_s);
  }
};

class Test2: public Test
{
public:
  Test2() : Test("ER complex poly") {}

  virtual bool check() const
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/ER9_VERYNICE_MMStack_Pos0.ome_Simple Segmentation_cleaned_binary_poly.poly");

    poly->apply_pxsize(0.0645);
    poly->round_poly_pts();
    bool res = poly->polys()[0].inside({20.3233146, 55.128996});
    delete poly;
    return res;
  }
};

class Test3: public Test
{
public:
  Test3() : Test("InPoly1") {}

  virtual bool check() const
  {
    PointEnsemble<2> pe;
    pe.push_back({72, 30});
    pe.push_back({72, 25.8});
    pe.push_back({71.21445, 25.8});
    pe.push_back({71.0532, 25.8});
    pe.push_back({70.89195, 25.8});
    pe.push_back({70, 25.8});
    pe.push_back({70, 30});
    Polygon poly(pe);

    std::cout << poly << std::endl;
    return !poly.inside({71.0532001, 30.894131});
  }
};

class Test4: public Test
{
public:
  Test4() : Test("InPoly2") {}

  virtual bool check() const
  {
    PointEnsemble<2> pe;
    pe.push_back({72, 30});
    pe.push_back({72, 25.8});
    pe.push_back({71.21445, 25.8});
    pe.push_back({71.0532, 25.8});
    pe.push_back({70.89195, 25.8});
    pe.push_back({70, 25.8});
    pe.push_back({70, 30});
    Polygon poly(pe);

    std::cout << poly << std::endl;
    return !poly.inside({71.0532001, 30.894131});
  }
};

class Test5: public Test
{
public:
  Test5() : Test("Poly inside 0") {}

  virtual bool check() const
  {
    PointEnsemble<2> pe;
    pe.push_back({1004.1, 246});
    pe.push_back({997.3, 246.1});
    pe.push_back({990.5, 246.1});
    pe.push_back({986, 247.6});
    pe.push_back({981.5, 249});
    pe.push_back({979.3, 249});
    pe.push_back({977.1, 249});
    pe.push_back({974.6, 251.2});
    pe.push_back({972.1, 253.5});
    pe.push_back({971.4, 256.2});
    pe.push_back({970.8, 258.9});
    pe.push_back({973.4, 261.4});
    pe.push_back({975.9, 264});
    pe.push_back({977, 264});
    pe.push_back({978, 264});
    pe.push_back({979.9, 262.1});
    pe.push_back({981.8, 260.2});
    pe.push_back({984.4, 260.7});
    pe.push_back({987, 261.2});
    pe.push_back({988.5, 260});
    pe.push_back({990, 258.8});
    pe.push_back({989.4, 256.4});
    pe.push_back({988.8, 254});
    pe.push_back({990, 254});
    pe.push_back({991.2, 254});
    pe.push_back({991.7, 256});
    pe.push_back({992.2, 258});
    pe.push_back({993.4, 258});
    pe.push_back({994.5, 258});
    pe.push_back({1000, 255.2});
    pe.push_back({1005.5, 252.5});
    pe.push_back({1005.2, 250.5});
    pe.push_back({1004.9, 248.5});
    pe.push_back({1004.5, 247.2});
    pe.push_back({1004.1, 246});
    Polygon poly(pe);

    return !poly.inside({989.4, 254.005});
  }
};

class Test6: public Test
{
public:
  Test6() : Test("Poly inside 1") {}

  virtual bool check() const
  {
    PointEnsemble<2> pe;
    pe.push_back({977.2, 185.6});
    pe.push_back({978, 188.5});
    pe.push_back({977.5, 189.7});
    pe.push_back({977.1, 191});
    pe.push_back({973.7, 191});
    pe.push_back({970.2, 191});
    pe.push_back({969.1, 189.6});
    pe.push_back({967.9, 188.1});
    pe.push_back({969, 186});
    pe.push_back({970.1, 183.9});
    pe.push_back({972.3, 183});
    pe.push_back({974.5, 182.1});
    pe.push_back({975.4, 182.4});
    pe.push_back({976.4, 182.6});
    Polygon poly(pe);

    bool res = !Polygon::check_normals(poly, false);

    if (!res)
      return false;

    poly = Polygon::reverse(poly);

    std::cout << poly << std::endl;

    Segment<2> seg(poly.pts()[9], poly.pts()[10]);
    Vec<2> norm = seg.normal();
    Point<2> p = seg.p1() + seg.vector() / 2 + norm * 0.005;
    std::cout << "TRUE POINT: " << p << " -> " << poly.inside(p) << std::endl;

    std::cout << poly.inside({975.4, 191.005}) << std::endl;

    res = !poly.inside({975.4, 191.005});
    if (!res)
      return false;

    return Polygon::check_normals(poly, false);
  }
};

class Test7: public Test
{
public:
  Test7() : Test("Poly inside 2") {}

  virtual bool check() const
  {
    PointEnsemble<2> pe;
    pe.push_back({110.5, 62.8});
    pe.push_back({105, 62.4});
    pe.push_back({99.4, 62});
    pe.push_back({97.8, 63.6});
    pe.push_back({96.2, 65.3});
    pe.push_back({97.2, 67});
    pe.push_back({98.3, 68.7});
    pe.push_back({95.7, 72.2});
    pe.push_back({93, 75.7});
    pe.push_back({93, 77.4});
    pe.push_back({93, 79.1});
    pe.push_back({94.5, 81.9});
    pe.push_back({95.9, 84.8});
    pe.push_back({94.2, 86.2});
    pe.push_back({92.5, 87.7});
    pe.push_back({90.8, 88.3});
    pe.push_back({89.2, 88.9});
    pe.push_back({88.6, 88.3});
    pe.push_back({88, 87.7});
    pe.push_back({89, 86.5});
    pe.push_back({89.9, 85.4});
    pe.push_back({89.3, 81.6});
    pe.push_back({88.7, 77.9});
    pe.push_back({86.3, 82.1});
    pe.push_back({83.9, 86.3});
    pe.push_back({85.1, 87});
    pe.push_back({86.2, 87.8});
    pe.push_back({85, 91.4});
    pe.push_back({83.8, 95});
    pe.push_back({81.9, 96});
    pe.push_back({80, 97.1});
    pe.push_back({80, 102.5});
    pe.push_back({80, 107.9});
    pe.push_back({81.1, 108.5});
    pe.push_back({82.1, 109.2});
    pe.push_back({83.9, 107.6});
    pe.push_back({85.7, 106});
    pe.push_back({87.3, 106});
    pe.push_back({89, 106});
    pe.push_back({89, 105});
    pe.push_back({89, 104});
    pe.push_back({90.8, 104});
    pe.push_back({92.6, 104});
    pe.push_back({93.5, 104.9});
    pe.push_back({94.5, 105.8});
    pe.push_back({96.8, 106.5});
    pe.push_back({99, 107.1});
    pe.push_back({99, 105});
    pe.push_back({99, 102.9});
    pe.push_back({98, 101});
    pe.push_back({97, 99.2});
    pe.push_back({93.8, 98.5});
    pe.push_back({90.5, 97.8});
    pe.push_back({89.8, 97.4});
    pe.push_back({89.2, 97});
    pe.push_back({90.3, 93.7});
    pe.push_back({91.5, 90.5});
    pe.push_back({91.6, 90.2});
    pe.push_back({91.7, 90});
    pe.push_back({94.8, 91.6});
    pe.push_back({98, 93.1});
    pe.push_back({99.2, 94.9});
    pe.push_back({100.5, 96.7});
    pe.push_back({102.7, 97.8});
    pe.push_back({104.9, 99});
    pe.push_back({108.3, 99});
    pe.push_back({111.7, 99});
    pe.push_back({116.3, 97});
    pe.push_back({120.8, 95});
    pe.push_back({121.4, 93.4});
    pe.push_back({122, 91.9});
    pe.push_back({119.9, 87.8});
    pe.push_back({117.9, 83.8});
    pe.push_back({118.4, 82.5});
    pe.push_back({118.9, 81.2});
    pe.push_back({116.9, 79.4});
    pe.push_back({114.9, 77.6});
    pe.push_back({112.8, 77.5});
    pe.push_back({110.7, 77.5});
    pe.push_back({110.3, 76.3});
    pe.push_back({109.9, 75.1});
    pe.push_back({112.4, 74.4});
    pe.push_back({115, 73.8});
    pe.push_back({115, 72.8});
    pe.push_back({115, 71.8});
    pe.push_back({113, 69});
    pe.push_back({111, 66.2});
    pe.push_back({111.6, 64.6});
    pe.push_back({112.2, 63});
    pe.push_back({111.4, 62.9});

    Polygon poly(pe);
    std::cout << "===================" << std::endl;
    std::cout << poly << std::endl;

    bool res = poly.inside({91.7, 104.005}) == false;

    if (!res)
      return false;

    poly = Polygon::reverse(poly);
    return poly.inside({91.7, 104.005}) == false;
  }
};

class Test8: public Test
{
public:
  Test8() : Test("Poly inside 2") {}

  virtual bool check() const
  {
    PointEnsemble<2> pe;
    pe.push_back({1083.7, 659});
    pe.push_back({1082.3, 659});
    pe.push_back({1081, 659});
    pe.push_back({1081, 660.9});
    pe.push_back({1081, 662.8});
    pe.push_back({1084.2, 668.4});
    pe.push_back({1087.4, 673.9});
    pe.push_back({1088.2, 673.4});
    pe.push_back({1089, 672.9});
    pe.push_back({1089, 673.9});
    pe.push_back({1089, 674.9});
    pe.push_back({1092.8, 676});
    pe.push_back({1096.7, 677});
    pe.push_back({1098.5, 681});
    pe.push_back({1100.3, 685});
    pe.push_back({1102.9, 684.6});
    pe.push_back({1105.5, 684.2});
    pe.push_back({1106.4, 684.1});
    pe.push_back({1107.2, 684});
    pe.push_back({1106.6, 681.4});
    pe.push_back({1105.9, 678.7});
    pe.push_back({1103.3, 675.3});
    pe.push_back({1100.7, 671.9});
    pe.push_back({1097.6, 672.2});
    pe.push_back({1094.5, 672.5});
    pe.push_back({1094.2, 670});
    pe.push_back({1093.8, 667.5});
    pe.push_back({1092.7, 666.1});
    pe.push_back({1091.5, 664.7});
    pe.push_back({1087.6, 661.8});
    pe.push_back({1083.7, 659});
    Polygon poly(pe);

    return !poly.inside({1088.995, 673.4});
  }
};

class Test9: public Test
{
public:
  Test9() : Test("Colinear") {}

  virtual bool check() const
  {
    Point<2> p1 = {1, 1};
    Point<2> p2 = {10, 1};
    Point<2> p3 = {1, 2};

    std::cout << " [Test 1]";
    bool res = !colinear(p1, p2, p3);
    if (!res)
      return false;

    std::cout << " [Test 2]";
    p3 = {5, 1};
    res = colinear(p1, p2, p3);
    if (!res)
      return false;

    std::cout << " [Test 3]";
    p1 = {0, 0};
    p2 = {5, 5};
    p3 = {3, 3};
    return colinear(p1, p2, p3);
  }
};

class Test10: public Test
{
public:
  Test10() : Test("Orientation") {}

  virtual bool check() const
  {
    Point<2> p1 = {1, 1};
    Point<2> q1 = {10, 1};
    Point<2> p2 = {1, 2};

    std::cout << " [Test 1]";
    bool res = orientation(p1, q1, p2) == 2;
    if (!res)
      return false;

    std::cout << " [Test 2]";
    p2 = {5, 1};
    return orientation(p1, q1, p2) == 0;
  }
};

class Test11: public Test
{
public:
  Test11() : Test("Segments") {}

  virtual bool check() const
  {
    Segment<2> s1({1, 1}, {10, 1});
    Segment<2> s2({1, 2}, {10, 2});
    Point<2> inter_p;
    Point<2> res_p;

    std::cout << " [Test 1]";

    bool res = !s1.on_segment(s2.p1());
    if (!res)
      return false;

    std::cout << " [Test 2]";
    res = !s1.on_segment(s2.p2());
    if (!res)
      return false;

    std::cout << " [Test 3]";
    res = !s1.intersect(s2, inter_p);
    if (!res)
      return false;

    std::cout << " [Test 4]";
    s1 = Segment<2>({10, 0}, {0, 10});
    s2 = Segment<2>({0, 0}, {10, 10});
    res = s1.intersect(s2, inter_p);
    if (!res)
      return false;

    std::cout << " [Test 5]";
    s1 = Segment<2>({-5, 5}, {0, 0});
    s2 = Segment<2>({1, 1}, {10, 10});
    res = !s1.intersect(s2, inter_p);
    if (!res)
      return false;

    std::cout << " [Test 6]";
    s1 = Segment<2>({0, 0}, {5, 5});
    s2 = Segment<2>({2, 1}, {10000.0, 1});
    res = !s1.intersect(s2, inter_p);
    if (!res)
      return false;

    std::cout << " [Test 7]";
    s1 = Segment<2>({0, 0}, {5, 5});
    res = s1.on_segment({3, 3});
    if (!res)
      return false;

    std::cout << " [Test 8]";
    s1 = Segment<2>({0, 0}, {5, 5});
    s2 = Segment<2>({3, 3}, {10000.0, 3});
    res = s1.intersect(s2, inter_p);
    if (!res)
      return false;

    std::cout << " [Test 9]";
    s1 = Segment<2>({10, 10}, {0, 10});
    res = !s1.on_segment({-1, 10});
    if (!res)
      return false;

    std::cout << " [Test 10]";
    s1 = Segment<2>({10, 10}, {10, 0});
    res = s1.intersect(Segment<2>({5, 5}, {15, 5}), inter_p);
    if (!res)
      return false;

    std::cout << " [Test 11]";
    s1 = Segment<2>({5, 5}, {5, -5});
    s2 = Segment<2>({0, 0}, {10, 0});
    s1.intersect(s2, inter_p);
    res = (inter_p == (Point<2>) {5.0, 0.0});
    if (!res)
      return false;

    std::cout << " [Test 12]";
    s1 = Segment<2>({5, 0}, {5, 10});
    res = (s1.normal()  == (Vec<2>) {-1, 0});
    if (!res)
      return false;

    std::cout << " [Test 13]";
    s1 = Segment<2>({2, 2}, {6, 6});
    s2 = Segment<2>({5, 0}, {5, 10});
    s1.intersect(s2, inter_p);
    res_p = Segment<2>::reflect(s1, s2, inter_p);
    assert((res_p == (Point<2>) {4, 6}));
    if (!res)
      return false;

    std::cout << " [Test 14]";
    s1 = Segment<2>({2, 2}, {8, 2});
    s2 = Segment<2>({5, 0}, {5, 10});
    s1.intersect(s2, inter_p);
    res = (inter_p == (Point<2>) {5, 2});
    if (!res)
      return false;

    std::cout << " [Test 15]";
    s1 = Segment<2>({2, 2}, {8, 2});
    s2 = Segment<2>({5, 0}, {5, 10});
    s1.intersect(s2, inter_p);
    res_p = Segment<2>::reflect(s1, s2, inter_p);
    res = (res_p == (Point<2>) {2, 2});
    if (!res)
      return false;

    std::cout << " [Test 16]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    s2 = Segment<2>({50.6049, 129.478}, {50.4045, 120.423});
    res = s1.intersect(s2, inter_p);
    if (!res)
      return false;

    std::cout << " [Test 17]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    s2 = Segment<2>({50.6049, 129.478}, {50.4045, 120.423});
    s1.intersect(s2, inter_p);
    res = pts_eq(inter_p, {50.6012629, 129.31366}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 18]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    s2 = Segment<2>({50.6049, 129.478}, {50.4045, 120.423});
    s1.intersect(s2, inter_p);
    res_p = Segment<2>::reflect(s1, s2, inter_p);
    res = pts_eq(res_p, {51.8634543, 130.054885}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 19]";
    s1 = Segment<2>({50.6012629, 129.31366}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    res = s1.intersect(s2, inter_p);
    if (!res)
      return false;

    std::cout << " [Test 20]";
    s1 = Segment<2>({50.6012629, 129.31366}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    s1.intersect(s2, inter_p);
    res = pts_eq(inter_p, {50.8835804, 129.479452}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 21]";
    s1 = Segment<2>({50.6012629, 129.31366}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    s1.intersect(s2, inter_p);
    res_p = Segment<2>::reflect(s1, s2, inter_p);
    res = pts_eq(res_p, {51.8693959, 128.914258}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 22]";
    s1 = Segment<2>({14.1584, 10.0646}, {14.1983, 10.0911});
    s2 = Segment<2>({14.0766, 10.1441}, {14.2188, 10.0059});
    s1.intersect(s2, inter_p);
    res = pts_eq(inter_p, {14.1584006, 10.0646004}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 23]";
    s1 = Segment<2>({14.1584, 10.0646}, {14.1983, 10.0911});
    s2 = Segment<2>({14.0766, 10.1441}, {14.2188, 10.0059});
    s1.intersect(s2, inter_p);
    res_p = Segment<2>::reflect(s1, s2, inter_p);
    res = pts_eq(res_p, {14.1330499, 10.0239613}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 24]";
    s1 = Segment<2>({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment<2>({8.69375, 0.954687}, {8.775, 0.979062});
    res = s1.intersect(s2, inter_p);
    if (!res)
      return false;

    std::cout << " [Test 25]";
    s1 = Segment<2>({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment<2>({8.69375, 0.954687}, {8.775, 0.979062});
    s1.intersect(s2, inter_p);
    res = pts_eq(inter_p, {8.76027862, 0.974645586}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 26]";
    s1 = Segment<2>({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment<2>({8.69375, 0.954687}, {8.775, 0.979062});
    res = dot(s1.vector(), s2.normal()) < 0;
    if (!res)
      return false;

    std::cout << " [Test 27]";
    s1 = Segment<2>({8.76194, 0.975147}, {8.72982, 0.965453});
    s2 = Segment<2>({8.69375, 0.954687}, {8.775, 0.979062});
    s1.intersect(s2, inter_p);
    res_p = Segment<2>::reflect(s1, s2, inter_p);
    res = pts_eq(res_p, {8.72978972, 0.965553917}, PRECISION);
    if (!res)
      return false;

    s1 = Segment<2> ({19.092, 48.98775}, {19.092, 48.7104});
    s2 = Segment<2> ({19.1076, 48.7332}, {19.0873, 48.7135});
    return s1.intersect(s2, inter_p);
  }
};

class Test12: public Test
{
public:
  Test12() : Test("Polygons") {}

  virtual bool check() const
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

    bool res = !poly.inside(p);
    if (!res)
      return false;

    std::cout << " [Test 2]";
    p = {5, 5};
    res = poly.inside(p);
    if (!res)
      return false;

    std::cout << " [Test 3]";
    p = {-1, 10};
    res = !poly.inside(p);
    if (!res)
      return false;

    std::cout << " [Test 4]";
    poly.intersect(Segment<2>({5, 5}, {15, 5}), inter_p, inter_s);
    res = (inter_s.p1() == (Point<2>) {10, 0});
    if (!res)
      return false;
    res = (inter_s.p2() == (Point<2>) {10, 10});
    if (!res)
      return false;

    std::cout << " [Test 5]";
    poly.intersect(Segment<2>({5, 5}, {5, 15}), inter_p, inter_s);
    res = (inter_s.p1() == (Point<2>) {10, 10});
    if (!res)
      return false;
    res = (inter_s.p2() == (Point<2>) {0, 10});
    if (!res)
      return false;

    std::cout << " [Test 6]";
    poly.intersect(Segment<2>({-5, 5}, {5, 5}), inter_p, inter_s);
    res = (inter_s.p1() == (Point<2>) {0, 10});
    if (!res)
      return false;
    res = (inter_s.p2() == (Point<2>) {0, 0});
    if (!res)
      return false;

    std::cout << " [Test 7]";
    poly.intersect(Segment<2>({5, -5}, {5, 5}), inter_p, inter_s);
    res = (inter_s.p1() == (Point<2>) {0, 0});
    if (!res)
      return false;
    res = (inter_s.p2() == (Point<2>) {10, 0});
    if (!res)
      return false;

    pe.clear();
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
    res = poly.inside({8.03189, 9.64409});
    if (!res)
      return false;

    std::cout << " [Test 9]";
    res = !poly.inside({8.03204, 9.6443});
    if (!res)
      return false;

    pe.clear();
    pe.push_back({6.72344, 0.154375});
    pe.push_back({6.64219, 7.21645e-17});
    pe.push_back({6.51219, 7.21645e-17});
    poly = Polygon(pe);

    std::cout << " [Test 10]";
    res = poly.inside({6.64591, 0.00707282});
    if (!res)
      return false;

    pe.clear();
    pe.push_back({0, 0});
    pe.push_back({5, 5});
    pe.push_back({5, 0});

    std::cout << " [Test 11]";
    poly = Polygon(pe);
    p = Point<2> ({2, 2});
    res = poly.inside(p);
    if (!res)
      return false;

    std::cout << " [Test 12]";
    p = {5, 1};
    res = poly.inside(p);
    if (!res)
      return false;

    std::cout << " [Test 13]";
    p = {8, 1};
    res = !poly.inside(p);
    if (!res)
      return false;

    std::cout << " [Test 14]";
    poly.intersect(Segment<2>({3.1, 3}, {-2, 8}), inter_p, inter_s);
    res = (inter_s.p1() == (Point<2>) {0, 0});
    if (!res)
      return false;

    return (inter_s.p2() == (Point<2>) {5, 5});
  }
};

class Test13: public Test
{
public:
  Test13() : Test("U polygon") {}

  virtual bool check() const
  {
    Point<2> inter_p;
    Segment<2> inter_s;
    Segment<2> s2;
    bool collided;

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
    Polygon u_poly(pe);
    if (!Polygon::check_normals(u_poly, true))
      u_poly = Polygon::reverse(u_poly);
    QuadTree qt(u_poly.bounding_box());
    qt.insert_segments(u_poly.segments(), 2);
    std::cout << pe.size() << " ; " << qt.size() << std::endl;
    PolygonCollider poly_collider(u_poly, &qt);

    std::cout << " [Test 1]";
    Point<2> p = {107.65, 109.702};
    bool res = !u_poly.inside(p);
    if (!res)
      return false;

    std::cout << " [Test 2]";
    Segment<2> s1({108.0070, 110.005}, {107.65, 109.7});
    u_poly.intersect(s1, inter_p, inter_s);
    res = (pts_eq(inter_s.p1(), {107.592, 112.292}, PRECISION) &&
	   pts_eq(inter_s.p2(), {108.323, 96.5429}, PRECISION));
    if (!res)
      return false;

    std::cout << " [Test 3]";
    s1 = Segment<2>({108.0070, 110.005}, {107.65, 109.7});
    u_poly.intersect(s1, inter_p, inter_s);
    res = pts_eq(inter_p, {107.709932, 109.751202}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 4]";
    s1 = Segment<2>({108.0070, 110.005}, {107.65, 109.7});
    collided = poly_collider.collide(s1.p1(), s1.p2(), p);
    res = collided == true;
    if (!res)
      return false;
    res = pts_eq(p, {107.774349, 109.705772}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 5]";
    s1 = Segment<2>({108.0070, 110.005}, {107.65, 109.7});
    s2 = Segment<2>({107.592, 112.292}, {108.323, 96.5429});
    Point<2> p1;
    collided = poly_collider.collide(s1.p1(), s1.p2(), p1);
    res = collided == true;
    if (!res)
      return false;
    s1.intersect(s2, inter_p);
    Point<2> p2 = Segment<2>::reflect(s1, s2, inter_p);
    res = p1 == p2;
    if (!res)
      return false;

    std::cout << " [Test 6]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    res = u_poly.inside(s1.p1()) && !u_poly.inside(s1.p2());
    if (!res)
      return false;

    std::cout << " [Test 7]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    u_poly.intersect(s1, inter_p, inter_s);
    res = (pts_eq(inter_s.p1(), {50.6049, 129.478}, PRECISION) &&
	   pts_eq(inter_s.p2(), {50.4045, 120.423}, PRECISION));
    if (!res)
      return false;

    std::cout << " [Test 8]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    u_poly.intersect(s1, inter_p, inter_s);
    res = s1.on_segment(inter_p); //not sure?
    if (!res)
      return false;

    std::cout << " [Test 9]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    u_poly.intersect(s1, inter_p, inter_s);
    res = pts_eq(inter_p, {50.6012629, 129.31366}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 10]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    u_poly.intersect(s1, inter_p, inter_s);
    std::cout << inter_p << std::endl;
    res = u_poly.inside(inter_p);
    if (!res)
      return false;

    std::cout << " [Test 11]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    u_poly.intersect(s1, inter_p, inter_s);
    p1 = Segment<2>::reflect(s1, s2, inter_p);
    res = pts_eq(p1, {51.8634543, 130.054885}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 12]";
    res = u_poly.inside({50.738, 129.225});
    if (!res)
      return false;

    std::cout << " [Test 13]";
    s1 = Segment<2>({50.738, 129.225}, {51.8634543, 130.054885});
    u_poly.intersect(s1, inter_p, inter_s);
    res = (pts_eq(inter_s.p1(), {102.054, 129.746}, PRECISION) &&
  	   pts_eq(inter_s.p2(), {50.6049, 129.478}, PRECISION));
    if (!res)
      return false;

    std::cout << " [Test 14]";
    s1 = Segment<2>({50.738, 129.225}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    s1.intersect(s2, inter_p);
    res = pts_eq(inter_p, {51.0844957, 129.480498}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 15]";
    s1 = Segment<2>({50.738, 129.225}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    s1.intersect(s2, inter_p);
    p1 = Segment<2>::reflect(s1, s2, inter_p);
    res = pts_eq(p1, {51.8693959, 128.914258}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 16]";
    s1 = Segment<2>({50.738, 129.225}, {51.8634543, 130.054885});
    s2 = Segment<2>({102.054, 129.746}, {50.6049, 129.478});
    s1.intersect(s2, inter_p);
    p1 = Segment<2>::reflect(s1, s2, inter_p);
    res = u_poly.inside(p1);
    if (!res)
      return false;

    std::cout << " [Test 17]";
    s1 = Segment<2>({50.738, 129.225}, {49.3731, 130.11});
    p1 = null_point<2>();
    collided = poly_collider.collide(s1.p1(), s1.p2(), p1);
    res = collided == true;
    if (!res)
      return false;
    res = pts_eq(p1, {51.8693959, 128.914258}, PRECISION);
    if (!res)
      return false;

    std::cout << " [Test 18]";
    s1 = Segment<2>({0.251875, 0}, {0.377813, 0});
    return s1.on_segment({0.347154, 0});
  }
};

class Test14: public Test
{
public:
  Test14() : Test("Compound polygon") {}

  virtual bool check() const
  {
    Point<2> inter_p;
    Segment<2> inter_s;
    bool collided;

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
    QuadTree qt(poly.bounding_box());
    qt.insert_segments(poly.segments(), 2);
    PolygonCollider poly_collider(poly, &qt);
    (void) poly_collider;

    std::cout << " [Test 1]";
    bool res = poly.inside({5.5, 5.5});
    if (!res)
      return false;

    std::cout << " [Test 2]";
    res = !poly.inside({20, 5.5});
    if (!res)
      return false;

    std::cout << " [Test 3]";
    res = !poly.inside({7, 7});
    if (!res)
      return false;

    std::cout << " [Test 4]";
    res = diff_polys[0].inside({7, 7});
    if (!res)
      return false;

    std::cout << " [Test 5]";
    poly.intersect(Segment<2>({9.5, 5.5}, {12, 5.5}), inter_p, inter_s);
    res = (inter_s.p1() == (Point<2>) {10, 5}) && (inter_s.p2() == (Point<2>) {10, 10});
    if (!res)
      return false;

    std::cout << " [Test 6]";
    poly.intersect(Segment<2>({5.8, 7.1}, {6.1, 7.1}), inter_p, inter_s);
    res = (inter_s.p1() == (Point<2>) {6, 9}) && (inter_s.p2() == (Point<2>) {6, 6});
    if (!res)
      return false;

    std::cout << " [Test 7]";
    Segment<2> s1 = Segment<2>({5.8, 7.1}, {6.1, 7.1});
    Segment<2> s2 = Segment<2>({6, 6}, {6, 9});
    s1.intersect(s2, inter_p);
    Point<2> p = Segment<2>::reflect(s1, s2, inter_p);
    res = poly.inside(p);
    if (!res)
      return false;
    res = (p == (Point<2>) {5.9, 7.1});
    if (!res)
      return false;

    std::cout << " [Test 8]";
    collided = poly_collider.collide({5.8, 7.1}, {6.1, 7.1}, p);
    res = collided == true;
    if (!res)
      return false;
    res = poly.inside(p);
    if (!res)
      return false;
    return (p == (Point<2>) {5.9, 7.1});
  }
};

class Test15: public Test
{
public:
  Test15() : Test("Compound polygon 2") {}

  virtual bool check() const
  {
    Point<2> inter_p;
    Segment<2> inter_s;
    bool collided;

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
    QuadTree qt(poly.bounding_box());
    qt.insert_segments(poly.segments(), 2);
    PolygonCollider poly_collider(poly, &qt);
    (void) poly_collider;

    std::cout << " [Test 1]";
    bool res = poly.inside({5.5, 5.5});
    if (!res)
      return false;

    std::cout << " [Test 2]";
    res = !poly.inside({20, 5.5});
    if (!res)
      return false;

    std::cout << " [Test 3]";
    res = !poly.inside({7, 7});
    if (!res)
      return false;

    std::cout << " [Test 4]";
    res = diff_polys[0].inside({7, 7});
    if (!res)
      return false;

    std::cout << " [Test 5]";
    poly.intersect(Segment<2>({9.5, 5.5}, {12, 5.5}), inter_p, inter_s);
    res = (inter_s.p1() == (Point<2>) {10, 5}) && (inter_s.p2() == (Point<2>) {10, 10});
    if (!res)
      return false;

    std::cout << " [Test 6]";
    poly.intersect(Segment<2>({5.8, 7.1}, {6.1, 7.1}), inter_p, inter_s);
    res = (inter_s.p1() == (Point<2>) {6, 9}) && (inter_s.p2() == (Point<2>) {6, 6});
    if (!res)
      return false;

    std::cout << " [Test 7]";
    Segment<2> s1({5.8, 7.1}, {6.1, 7.1});
    Segment<2> s2({6, 6}, {6, 9});
    s1.intersect(s2, inter_p);
    Point<2> p = Segment<2>::reflect(s1, s2, inter_p);
    res = poly.inside(p);
    if (!res)
      return false;
    res = (p == (Point<2>) {5.9, 7.1});
    if (!res)
      return false;

    std::cout << " [Test 8]";
    collided = poly_collider.collide({5.8, 7.1}, {6.1, 7.1}, p);
    res = collided == true;
    if (!res)
      return false;
    res = poly.inside(p);
    if (!res)
      return false;
    return (p == (Point<2>) {5.9, 7.1});
  }
};

class Test16: public Test
{
public:
  Test16() : Test("Complex polygon") {}

  virtual bool check() const
  {
    MultiplePolygon* polys =
      polys_from_inkscape_path("../resources/ER_net1.path");
    //save_polys_matlab(*polys, "/tmp/polys.m");

    bool res = polys->polys()[0].base().inside({203, 128});
    if (!res)
      return false;
    res = polys->polys()[0].base().inside({123, 176.2});
    if (!res)
      return false;
    res = polys->polys()[0].base().inside({195.4, 22.1});
    if (!res)
      return false;
    res = polys->polys()[0].base().inside({152.1, 61});
    if (!res)
      return false;

    CompoundPolygon poly = polys->polys()[0];
    poly.apply_pxsize(0.0406250);

    res = polys->polys().size() == 1;
    if (!res)
      return false;

    Segment<2> s2({177, 277.1}, {177, 280.3});
    std::cout << " [Test 1]";
    res = colinear(s2.p1(), s2.p2(), {177, 177.3});
    if (!res)
      return false;

    std::cout << " OK" << std::endl;
    assert(colinear(s2.p1(), s2.p2(), {177, 10000}));
    if (!res)
      return false;

    std::cout << " [Test 2]";
    res = polys->polys().size() == 1;
    if (!res)
      return false;

    std::cout << " [Test 3]";
    res = poly.inside({1.18408, 5.41125});
    if (!res)
      return false;

    std::cout << " [Test 4]";
    res = poly.inside({7.10528, 2.98698});
    if (!res)
      return false;

    std::cout << " [Test 5]";
    res = poly.inside({7.15607, 2.96563});
    if (!res)
      return false;

    std::cout << " OK" << std::endl;

    std::cout << " [Test 6]";
    res = poly.inside({3.86121, 0.73125});
    if (!res)
      return false;

    std::cout << " [Test 7]";
    res = poly.inside({6.84829, 7.4425});
    if (!res)
      return false;

    std::cout << " [Test 8]";
    res = poly.inside({1.86335, 2.31562});

    delete polys;
    return res;
  }
};

class Test17: public Test
{
public:
  Test17() : Test("Touch inside") {}

  virtual bool check() const
  {
    MultiplePolygon* polys =
      polys_from_inkscape_path("../resources/poly_touch.poly");
    CompoundPolygon poly = polys->polys()[0];

    std::cout << " [Test 1]";
    bool res = poly.inside((Point<2>) {481.9, 160});
    delete polys;
    return res;
  }
};

class Test18: public Test
{
public:
  Test18() : Test("Complex polygon 2") {}

  virtual bool check() const
  {
    MultiplePolygon* polys =
      polys_from_inkscape_path("../resources/C2-_2019_10_02__16_23_00_mask_5073.poly");
    save_polys_matlab(*polys, "/tmp/polys.m");

    std::cout << " [Test 1]";
    bool res = polys->polys().size() == 1;

    delete polys;
    return res;
  }
};

class Test19: public Test
{
public:
  Test19() : Test("Complex collide") {}

  virtual bool check() const
  {

    Segment<2> border({12.3784000000000, 9.35883000000000},
		      {12.4075000000000, 9.87177000000000});
    Segment<2> traj({12.4030, 9.8718}, {12.4108, 9.8685});

    Point<2> inter_p;
    border.intersect(traj, inter_p);
    Point<2> p = Segment<2>::reflect(traj, border, inter_p);
    (void) p; //NEED assert here
    return true;
  }
};

class Test20: public Test
{
public:
  Test20() : Test("Poly L") {}

  virtual bool check() const
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/L2_poly.poly");

    std::cout << "[TEST1] ";
    bool res = poly->polys()[0].inside((Point<2>) {71.059519, 105});

    delete poly;
    return res;
  }
};

class Test21: public Test
{
public:
  Test21() : Test("Poly moon") {}

  virtual bool check() const
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/moon_poly.poly");

    std::cout << "[TEST1] ";
    bool res = poly->polys()[0].inside((Point<2>) {90.60437, 41.0});
    if (!res)
      return false;

    std::cout << "[TEST2] ";
    res = poly->polys()[0].inside((Point<2>) {141.38558, 41.0});

    delete poly;
    return res;
  }
};

class Test22: public Test
{
public:
  Test22() : Test("Poly colin in") {}

  virtual bool check() const
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
    return poly.inside((Point<2>) {93, 95});
  }
};

class Test23: public Test
{
public:
  Test23() : Test("Poly colin in left") {}

  virtual bool check() const
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
    return poly.inside((Point<2>) {102, 95});
  }
};

class Test24: public Test
{
public:
  Test24() : Test("Poly colin in 2 left") {}

  virtual bool check() const
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
    return poly.inside((Point<2>) {102, 95});
  }
};

class Test25: public Test
{
public:
  Test25() : Test("Poly Fail 1") {}

  virtual bool check() const
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/poly_fail.poly");

    std::cout << "[Test 1]";
    bool res = poly->polys()[0].inside((Point<2>) {1194, 207.3});
    if (!res)
      return false;
    std::cout << "[Test 2]";
    res = poly->polys().size() == 1;

    delete poly;
    return res;
  }
};

class Test26: public Test
{
public:
  Test26() : Test("Poly Fail 2") {}

  virtual bool check() const
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/poly_fail2.poly");

    std::cout << "[Test 1]";
    bool res = poly->polys()[0].inside((Point<2>) {1155, 545.3});
    if (!res)
      return false;

    std::cout << "[Test 2]";
    res = poly->polys().size() == 1;

    delete poly;
    return res;
  }
};

class Test27: public Test
{
public:
  Test27() : Test("ER complex poly") {}

  virtual bool check() const
  {
    MultiplePolygon* poly =
      polys_from_inkscape_path("../resources/ER9_VERYNICE_MMStack_Pos0.ome_Simple Segmentation_cleaned_binary_poly.poly");

    bool res = poly->polys()[0].base().inside({633.5, 1064.2});
    if (!res)
      return false;

    std::cout << "[Test 1]";
    res = poly->polys()[0].base().inside({1155, 545.3});
    if (!res)
      return false;

    std::cout << "[Test 2]";
    res = poly->polys().size() == 1;
    if (!res)
      return false;

    poly->apply_pxsize(0.0645);
    poly->round_poly_pts();
    res = poly->inside({67.6094063, 21.2413533});
    if (!res)
      return false;

    res = poly->inside({44.3165149, 57.9196315});
    if (!res)
      return false;
    res = poly->inside({44.3179501, 57.9195661});

    delete poly;
    return res;
  }
};


// class TestX: public Test
// {
// public:
//   TestX() : Test("") {}

//   virtual bool check() const
//   {
//   }
// };


int main(int argc, char** argv)
{
  std::string todo;
  if (argc == 1)
    todo = "all";
  else if (argc == 2)
    todo = argv[1];

  std::cout << std::setprecision(15);

  Test* tests[] = {new Test1(), new Test2(), new Test3(), new Test4(),
    new Test5(), new Test6(), new Test7(), new Test8(), new Test9(),
    new Test10(), new Test11(), new Test12(), new Test13(), new Test14(),
    new Test15(), new Test16(), new Test17(), new Test18(), new Test19(),
    new Test20(), new Test21(), new Test22(), new Test23(), new Test24(),
    new Test25(), new Test26(), new Test27()};

  std::unordered_map<std::string, Test*> tests_map;
  for (Test* t: tests)
    tests_map.insert({t->name(), t});

  if (todo != "all" && tests_map.find(todo) == tests_map.end())
  {
    std::cerr << "ERROR test not found: " << todo << std::endl;
    return 1;
  }

  if (todo != "all")
  {
    Test* t = tests_map.at(todo);
    std::cout << "[1] " << t->name() << ":";

    if (!t->check())
      std::cout << " FAILED!" << std::endl;
    else
      std::cout << " OK" << std::endl;
  }
  else
  {
    int cnt = 1;
    for (const Test* test: tests)
      {
	std::cout << "[" << cnt << "] " << test->name() << ":";

	std::cout.clear();
	std::streambuf *old = std::cout.rdbuf();
	std::stringstream ss;
	std::cout.rdbuf(ss.rdbuf());

	bool status = test->check();

	std::cout.rdbuf(old);
	if (!status)
	  std::cout << " FAILED!" << std::endl;
	else
	  std::cout << " OK" << std::endl;

	++cnt;
      }
  }

  for (const Test* test: tests)
    delete test;

  return 0;
}

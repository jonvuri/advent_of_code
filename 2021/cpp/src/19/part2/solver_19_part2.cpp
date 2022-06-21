#include <algorithm>
#include <cstddef>
#include <functional>
#include <iostream>
#include <iterator>
#include <queue>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <doctest/doctest.h>

#include "solver_19_part2.h"


namespace {

struct Beacon
{
  int x;
  int y;
  int z;

  Beacon &operator+=(const Beacon &rhs)
  {
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    return *this;
  }
  friend Beacon operator+(Beacon lhs, const Beacon &rhs)
  {
    lhs += rhs;
    return lhs;
  }
  Beacon &operator-=(const Beacon &rhs)
  {
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    return *this;
  }
  friend Beacon operator-(Beacon lhs, const Beacon &rhs)
  {
    lhs -= rhs;
    return lhs;
  }
  Beacon operator-() const
  {
    return Beacon{ -x, -y, -z };
  }

  friend bool operator==(const Beacon &lhs, const Beacon &rhs)
  {
    return lhs.x == rhs.x and lhs.y == rhs.y and lhs.z == rhs.z;
  }
  friend bool operator!=(const Beacon &lhs, const Beacon &rhs)
  {
    return !(lhs == rhs);
  }

  struct Hasher
  {
    size_t operator()(const Beacon &beacon) const
    {
      size_t result = 17;
      result = result * 31 + std::hash<int>()(beacon.x);
      result = result * 31 + std::hash<int>()(beacon.y);
      result = result * 31 + std::hash<int>()(beacon.z);
      return result;
    }
  };
};

// Direction-less edge between two beacons
struct Edge
{
  Beacon first;
  Beacon second;

  // Half-computed distance between beacons, without the sqrt operation
  long magnitude;

  friend bool operator<(const Edge &lhs, const Edge &rhs)
  {
    return lhs.magnitude < rhs.magnitude;
  }

  friend bool operator==(const Edge &lhs, const Edge &rhs)
  {
    return lhs.magnitude == rhs.magnitude;
  }
};

constexpr long
  magnitude(const Beacon &lhs, const Beacon &rhs)
{
  // Take deltas between points
  long du = rhs.x - lhs.x;
  long dv = rhs.y - lhs.y;
  long dw = rhs.z - lhs.z;

  // Half-compute distance between points - no need to do the final sqrt since we only need to check
  // equality and that wouldn't affect the result. In fact it introduces floating-point issues there!
  long magnitude = du * du + dv * dv + dw * dw;

  return magnitude;
}

// Right-angle only sine fn
constexpr int sin_r(int deg)
{
  switch (deg) {
  case 0:
    return 0;
  case 90:
    return 1;
  case 180:
    return 0;
  case 270:
    return -1;
  default:
    throw std::out_of_range("");
  }
}

// Right-angle only cosine fn
constexpr int cos_r(int deg)
{
  switch (deg) {
  case 0:
    return 1;
  case 90:
    return 0;
  case 180:
    return -1;
  case 270:
    return 0;
  default:
    throw std::out_of_range("");
  }
}

// Helpers for a given rotation matrix:
// | xu, yu, zu |
// | xv, yv, zv |
// | xw, yw, zw |

constexpr int xu(int a, int b)
{
  return cos_r(a) * cos_r(b);
}

constexpr int yu(int a, int b, int y)
{
  return cos_r(a) * sin_r(b) * sin_r(y) - sin_r(a) * cos_r(y);
}

constexpr int zu(int a, int b, int y)
{
  return cos_r(a) * sin_r(b) * cos_r(y) + sin_r(a) * sin_r(y);
}

constexpr int xv(int a, int b)
{
  return sin_r(a) * cos_r(b);
}

constexpr int yv(int a, int b, int y)
{
  return sin_r(a) * sin_r(b) * sin_r(y) + cos_r(a) * cos_r(y);
}

constexpr int zv(int a, int b, int y)
{
  return sin_r(a) * sin_r(b) * cos_r(y) - cos_r(a) * sin_r(y);
}

constexpr int xw(int b)
{
  return -1 * sin_r(b);
}

constexpr int yw(int b, int y)
{
  return cos_r(b) * sin_r(y);
}

constexpr int zw(int b, int y)
{
  return cos_r(b) * cos_r(y);
}


template<int a, int b, int y>
constexpr Beacon rotate(Beacon p)
{
  return {
    p.x * xu(a, b) + p.y * yu(a, b, y) + p.z * zu(a, b, y),
    p.x * xv(a, b) + p.y * yv(a, b, y) + p.z * zv(a, b, y),
    p.x * xw(b) + p.y * yw(b, y) + p.z * zw(b, y),
  };
}

inline bool is_correct_rotation(const std::function<Beacon(Beacon)> &rotate_fn, const std::vector<std::pair<Beacon, Beacon>> &beacon_pairs)
{
  auto it = beacon_pairs.begin();

  // Check to see if this rotation causes rays to and from _all_ beacons to lead to the same point, using first as the measuring stick
  Beacon first_rotated = it->first - rotate_fn(it->second);

  for (++it; it != beacon_pairs.end(); ++it) {
    Beacon rotated = it->first + rotate_fn(-it->second);
    if (rotated != first_rotated) {
      return false;
    }
  }

  return true;
}

// The set of rotations chosen here is covering and exclusive over possible rotations, so we assume first found is also the only one
std::function<Beacon(Beacon)> first_matching_rotation(const std::vector<std::pair<Beacon, Beacon>> &beacon_pairs)
{
  // Facing +y
  if (is_correct_rotation(rotate<0, 0, 0>, beacon_pairs)) {
    return rotate<0, 0, 0>;
  } else if (is_correct_rotation(rotate<0, 0, 90>, beacon_pairs)) {
    return rotate<0, 0, 90>;
  } else if (is_correct_rotation(rotate<0, 0, 180>, beacon_pairs)) {
    return rotate<0, 0, 180>;
  } else if (is_correct_rotation(rotate<0, 0, 270>, beacon_pairs)) {
    return rotate<0, 0, 270>;
  }

  // Facing -y
  else if (is_correct_rotation(rotate<180, 0, 0>, beacon_pairs)) {
    return rotate<180, 0, 0>;
  } else if (is_correct_rotation(rotate<180, 0, 90>, beacon_pairs)) {
    return rotate<180, 0, 90>;
  } else if (is_correct_rotation(rotate<180, 0, 180>, beacon_pairs)) {
    return rotate<180, 0, 180>;
  } else if (is_correct_rotation(rotate<180, 0, 270>, beacon_pairs)) {
    return rotate<180, 0, 270>;
  }

  // Facing +x
  else if (is_correct_rotation(rotate<90, 0, 0>, beacon_pairs)) {
    return rotate<90, 0, 0>;
  } else if (is_correct_rotation(rotate<90, 0, 90>, beacon_pairs)) {
    return rotate<90, 0, 90>;
  } else if (is_correct_rotation(rotate<90, 0, 180>, beacon_pairs)) {
    return rotate<90, 0, 180>;
  } else if (is_correct_rotation(rotate<90, 0, 270>, beacon_pairs)) {
    return rotate<90, 0, 270>;
  }

  // Facing -x
  else if (is_correct_rotation(rotate<270, 0, 0>, beacon_pairs)) {
    return rotate<270, 0, 0>;
  } else if (is_correct_rotation(rotate<270, 0, 90>, beacon_pairs)) {
    return rotate<270, 0, 90>;
  } else if (is_correct_rotation(rotate<270, 0, 180>, beacon_pairs)) {
    return rotate<270, 0, 180>;
  } else if (is_correct_rotation(rotate<270, 0, 270>, beacon_pairs)) {
    return rotate<270, 0, 270>;
  }

  // Note that due to gimbal lock, either yaw or roll can be used for z
  // Facing +z
  else if (is_correct_rotation(rotate<0, 90, 0>, beacon_pairs)) {
    return rotate<0, 90, 0>;
  } else if (is_correct_rotation(rotate<90, 90, 0>, beacon_pairs)) {
    return rotate<90, 90, 0>;
  } else if (is_correct_rotation(rotate<180, 90, 0>, beacon_pairs)) {
    return rotate<180, 90, 0>;
  } else if (is_correct_rotation(rotate<270, 90, 0>, beacon_pairs)) {
    return rotate<270, 90, 0>;
  }

  // Facing -z
  else if (is_correct_rotation(rotate<0, 270, 0>, beacon_pairs)) {
    return rotate<0, 270, 0>;
  } else if (is_correct_rotation(rotate<90, 270, 0>, beacon_pairs)) {
    return rotate<90, 270, 0>;
  } else if (is_correct_rotation(rotate<180, 270, 0>, beacon_pairs)) {
    return rotate<180, 270, 0>;
  } else {
    if (!is_correct_rotation(rotate<270, 270, 0>, beacon_pairs)) {
      throw solver_runtime_error("Could not find a rotation");
    }

    return rotate<270, 270, 0>;
  }
}

std::vector<Edge> get_scanner_edges(const std::vector<Beacon> &scanner)
{
  std::vector<Edge> result;

  for (auto it = scanner.begin(); it != scanner.end(); ++it) {
    for (auto next = std::next(it); next != scanner.end(); ++next) {
      result.push_back(Edge{ *it, *next, magnitude(*it, *next) });
    }
  }

  std::sort(result.begin(), result.end());

  return result;
}

std::vector<std::pair<Edge, Edge>> get_edge_intersection(const std::vector<Edge> &lhs, const std::vector<Edge> &rhs)
{
  std::vector<std::pair<Edge, Edge>> result = {};

  auto rhs_it = rhs.begin();

  for (const auto &l_edge : lhs) {
    const auto r_bound = std::lower_bound(rhs_it, rhs.end(), l_edge);

    if (r_bound != rhs.end()) {
      if (*r_bound == l_edge) {
        result.emplace_back(std::make_pair(l_edge, *r_bound));
      }

      // We can continue searching from the lower bound on for the next l_edge
      rhs_it = r_bound;
    }
  }

  return result;
}

// UGLY: Overloading Beacon in these mappings just to represent a vector offset to the scanners, for the mapping functions.

// One-way mapping from any scanner to another (direction matters)
struct PairMapping
{
  size_t from_scanner;
  size_t to_scanner;
  Beacon offset;
  std::function<Beacon(Beacon)> rotation;
};

// Mapping from origin (zero scanner) to another scanner
struct ZeroMapping
{
  Beacon offset = {};
  std::function<Beacon(Beacon)> rotation;
};

}// namespace


std::string Solver_19_part2::solve(std::istream &is)
{
  std::vector<std::vector<Beacon>> scanners;

  {
    std::string line;
    size_t scanner_num = 0;
    std::vector<Beacon> *scanner = nullptr;

    while (std::getline(is, line)) {
      if (line.find('s') != std::string::npos) {
        scanners.emplace_back(std::vector<Beacon>());
        scanner = &scanners.at(scanner_num);
        scanner_num += 1;
      } else if (scanner != nullptr) {
        std::stringstream ss(line);
        Beacon pv{};

        ss >> std::ws;

        if (ss >> pv.x) {
          ss.ignore(1);
          ss >> pv.y;
          ss.ignore(1);
          ss >> pv.z;
          scanner->push_back(pv);
        }
      }
    }
  }

  std::vector<std::vector<Edge>> scanner_edges;
  scanner_edges.reserve(scanners.size());
  std::transform(scanners.begin(), scanners.end(), std::back_inserter(scanner_edges), get_scanner_edges);

  std::vector<PairMapping> scanner_pair_mappings = {};

  for (size_t i = 0; i < scanner_edges.size(); ++i) {
    for (size_t j = i + 1; j < scanner_edges.size(); ++j) {
      std::vector<std::pair<Edge, Edge>> edge_intersection = get_edge_intersection(scanner_edges.at(i), scanner_edges.at(j));

      // Only proceed if size = 12 choose 2 = 66. The puzzle graciously guarantees exactly this amount of common beacons, not more or less.
      if (edge_intersection.size() != 66) {
        continue;
      }

      // Collect map of left/right beacons, add magnitude to value of map
      // Zip maps together into pairs
      std::unordered_map<Beacon, long, Beacon::Hasher> left_total_magnitudes;
      std::unordered_map<Beacon, long, Beacon::Hasher> right_total_magnitudes;

      for (const auto &edge_pair : edge_intersection) {
        const auto magnitude = edge_pair.first.magnitude;
        left_total_magnitudes[edge_pair.first.first] += magnitude;
        left_total_magnitudes[edge_pair.first.second] += magnitude;
        right_total_magnitudes[edge_pair.second.first] += magnitude;
        right_total_magnitudes[edge_pair.second.second] += magnitude;
      }

      std::vector<std::pair<Beacon, Beacon>> beacon_pairs;

      for (const auto &left_total : left_total_magnitudes) {
        const auto &right_total = std::find_if(right_total_magnitudes.begin(), right_total_magnitudes.end(), [&](const auto &r) { return r.second == left_total.second; });

        if (right_total == right_total_magnitudes.end()) {
          throw solver_runtime_error("No match found for a left and right total magnitude pair");
        }

        beacon_pairs.emplace_back(std::make_pair(left_total.first, right_total->first));
      }

      if (beacon_pairs.size() != 12) {
        throw solver_runtime_error("Could not find all beacon pairs");
      }

      const auto rotation = first_matching_rotation(beacon_pairs);
      const auto offset = beacon_pairs[0].first + rotation(-beacon_pairs[0].second);

      scanner_pair_mappings.emplace_back(PairMapping{ i, j, offset, rotation });


      // Also add backwards mapping
      std::vector<std::pair<Beacon, Beacon>> back_beacon_pairs;
      back_beacon_pairs.reserve(beacon_pairs.size());
      std::transform(beacon_pairs.begin(), beacon_pairs.end(), std::back_inserter(back_beacon_pairs), [](const auto &pair) { return std::make_pair(pair.second, pair.first); });

      const auto back_rotation = first_matching_rotation(back_beacon_pairs);
      const auto back_offset = back_beacon_pairs[0].first + back_rotation(-back_beacon_pairs[0].second);

      scanner_pair_mappings.emplace_back(PairMapping{ j, i, back_offset, back_rotation });
    }
  }


  std::unordered_map<size_t, ZeroMapping> scanner_zero_mappings = {};

  scanner_zero_mappings[0] = ZeroMapping{ Beacon{ 0, 0, 0 }, rotate<0, 0, 0> };

  // Initialize with all mappings coming from scanner 0
  for (const auto &pair_mapping : scanner_pair_mappings) {
    if (pair_mapping.from_scanner == 0) {
      const auto offset = pair_mapping.offset;
      const auto rotation = pair_mapping.rotation;

      if (!rotation) {
        throw solver_runtime_error("No rotation");
      }

      scanner_zero_mappings[pair_mapping.to_scanner] = ZeroMapping{ offset, rotation };
    }
  }

  std::queue<size_t> unmapped_scanners = {};

  for (size_t i = 1; i < scanners.size(); ++i) {
    if (scanner_zero_mappings.find(i) == scanner_zero_mappings.end()) {
      unmapped_scanners.push(i);
    }
  }

  while (!unmapped_scanners.empty()) {
    const size_t i = unmapped_scanners.front();
    unmapped_scanners.pop();

    // There could be more than one mapping, and if so, we need them all in case they are parents for other mappings.
    // But if we don't find any yet, we need to push this to the back of the queue again until we get THIS scanner's parent.
    bool found_mapping = false;

    for (const auto &pair_mapping : scanner_pair_mappings) {
      if (pair_mapping.to_scanner == i) {
        const auto parent_mapping = scanner_zero_mappings.find(pair_mapping.from_scanner);
        if (parent_mapping != scanner_zero_mappings.end()) {
          const auto pair_mapper = pair_mapping.rotation;
          const std::function<Beacon(Beacon)> parent_mapper = parent_mapping->second.rotation;

          // Follow forward vector to parent first, then parent-mapped forward vector to this scanner
          const auto offset = parent_mapping->second.offset + parent_mapper(pair_mapping.offset);
          const auto rotation = [pair_mapper, parent_mapper](Beacon b) -> Beacon { return parent_mapper(pair_mapper(b)); };

          scanner_zero_mappings[i] = ZeroMapping{ offset, rotation };
          found_mapping = true;
        }
      }
    }

    if (!found_mapping) {
      unmapped_scanners.push(i);
    }
  }

  long max_manhattan_distance = 0;
  for (size_t i = 0; i < scanner_zero_mappings.size(); ++i) {
    for (size_t j = i + 1; j < scanner_zero_mappings.size(); ++j) {
      const auto &oi = scanner_zero_mappings[i].offset;
      const auto &oj = scanner_zero_mappings[j].offset;

      const long manhattan_distance = std::abs(oi.x - oj.x) + std::abs(oi.y - oj.y) + std::abs(oi.z - oj.z);

      if (manhattan_distance > max_manhattan_distance) {
        max_manhattan_distance = manhattan_distance;
      }
    }
  }

  return std::to_string(max_manhattan_distance);
}

TEST_CASE("testing solver for day 19 part 1 - scanner positioning, max manhattan distance")
{
  Solver_19_part2 solver;

  std::istringstream is(std::string{ R"(
      --- scanner 0 ---
      404,-588,-901
      528,-643,409
      -838,591,734
      390,-675,-793
      -537,-823,-458
      -485,-357,347
      -345,-311,381
      -661,-816,-575
      -876,649,763
      -618,-824,-621
      553,345,-567
      474,580,667
      -447,-329,318
      -584,868,-557
      544,-627,-890
      564,392,-477
      455,729,728
      -892,524,684
      -689,845,-530
      423,-701,434
      7,-33,-71
      630,319,-379
      443,580,662
      -789,900,-551
      459,-707,401

      --- scanner 1 ---
      686,422,578
      605,423,415
      515,917,-361
      -336,658,858
      95,138,22
      -476,619,847
      -340,-569,-846
      567,-361,727
      -460,603,-452
      669,-402,600
      729,430,532
      -500,-761,534
      -322,571,750
      -466,-666,-811
      -429,-592,574
      -355,545,-477
      703,-491,-529
      -328,-685,520
      413,935,-424
      -391,539,-444
      586,-435,557
      -364,-763,-893
      807,-499,-711
      755,-354,-619
      553,889,-390

      --- scanner 2 ---
      649,640,665
      682,-795,504
      -784,533,-524
      -644,584,-595
      -588,-843,648
      -30,6,44
      -674,560,763
      500,723,-460
      609,671,-379
      -555,-800,653
      -675,-892,-343
      697,-426,-610
      578,704,681
      493,664,-388
      -671,-858,530
      -667,343,800
      571,-461,-707
      -138,-166,112
      -889,563,-600
      646,-828,498
      640,759,510
      -630,509,768
      -681,-892,-333
      673,-379,-804
      -742,-814,-386
      577,-820,562

      --- scanner 3 ---
      -589,542,597
      605,-692,669
      -500,565,-823
      -660,373,557
      -458,-679,-417
      -488,449,543
      -626,468,-788
      338,-750,-386
      528,-832,-391
      562,-778,733
      -938,-730,414
      543,643,-506
      -524,371,-870
      407,773,750
      -104,29,83
      378,-903,-323
      -778,-728,485
      426,699,580
      -438,-605,-362
      -469,-447,-387
      509,732,623
      647,635,-688
      -868,-804,481
      614,-800,639
      595,780,-596

      --- scanner 4 ---
      727,592,562
      -293,-554,779
      441,611,-461
      -714,465,-776
      -743,427,-804
      -660,-479,-426
      832,-632,460
      927,-485,-438
      408,393,-506
      466,436,-512
      110,16,151
      -258,-428,682
      -393,719,612
      -211,-452,876
      808,-476,-593
      -575,615,604
      -485,667,467
      -680,325,-822
      -627,-443,-432
      872,-547,-609
      833,512,582
      807,604,487
      839,-516,451
      891,-625,532
      -652,-548,-490
      30,-46,-14
    )" });

  CHECK(solver.solve(is) == "3621");
}

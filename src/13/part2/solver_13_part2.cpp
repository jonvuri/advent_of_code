#include <algorithm>
#include <boost/functional/hash.hpp>
#include <cctype>
#include <limits>
#include <sstream>
#include <unordered_map>
#include <vector>

#include <doctest/doctest.h>

#include "solver_13_part2.h"

namespace {

std::string without_space(std::string str)
{
  str.erase(std::remove_if(str.begin(), str.end(), isspace), str.end());
  return str;
}

auto &Letter_B()
{
  static auto letter = without_space(std::string(R"(
    ###.
    #..#
    ###.
    #..#
    #..#
    ###.
  )"));

  return letter;
}

auto &Letter_F()
{
  static auto letter = without_space(std::string(R"(
    ####
    #...
    ###.
    #...
    #...
    #...
  )"));

  return letter;
}

auto &Letter_G()
{
  static auto letter = without_space(std::string(R"(
    .##.
    #..#
    #...
    #.##
    #..#
    .###
  )"));

  return letter;
}

auto &Letter_H()
{
  static auto letter = without_space(std::string(R"(
    #..#
    #..#
    ####
    #..#
    #..#
    #..#
  )"));

  return letter;
}

auto &Letter_L()
{
  static auto letter = without_space(std::string(R"(
    #...
    #...
    #...
    #...
    #...
    ####
  )"));

  return letter;
}

auto &Letter_R()
{
  static auto letter = without_space(std::string(R"(
    ###.
    #..#
    #..#
    ###.
    #.#.
    #..#
  )"));

  return letter;
}

auto &Letter_U()
{
  static auto letter = without_space(std::string(R"(
    #..#
    #..#
    #..#
    #..#
    #..#
    .##.
  )"));

  return letter;
}


const unsigned int LETTER_HEIGHT = 6;

using coord = std::pair<unsigned int, unsigned int>;

}// namespace


std::string Solver_13_part2::solve(std::istream &is)
{
  bool on_beta = false;
  std::unordered_map<coord, bool, boost::hash<coord>> point_map_alpha;
  std::unordered_map<coord, bool, boost::hash<coord>> point_map_beta;

  // Fold boundary during fold operation, right / bottom boundaries any other time
  unsigned int x_fold = 0;
  unsigned int y_fold = 0;

  {
    unsigned int coord_x = 0;
    unsigned int coord_y = 0;
    while (is >> std::ws && is.peek() != 'f' && is >> coord_x && is.ignore(1) && is >> coord_y) {
      point_map_alpha.emplace(std::make_pair(coord_x, coord_y), true);

      if (coord_x + 1 > x_fold) {
        x_fold = coord_x + 1;
      }

      if (coord_y + 1 > y_fold) {
        y_fold = coord_y + 1;
      }
    }
  }

  // Ingest and perform fold commands
  {
    std::string command;
    while (is >> std::ws && std::getline(is, command, '=')) {
      const auto &source_point_map = on_beta ? point_map_beta : point_map_alpha;
      auto &dest_point_map = on_beta ? point_map_alpha : point_map_beta;

      if (command == "fold along x") {
        is >> x_fold;

        for (const auto &point : source_point_map) {
          if (point.first.first > x_fold * 2 || point.first.second > y_fold) {
            // Point past fold boundaries (not including current fold operation)
            continue;
          }

          if (point.first.first > x_fold) {
            dest_point_map.emplace(std::make_pair(x_fold * 2 - point.first.first, point.first.second), true);
          } else {
            dest_point_map.emplace(std::make_pair(point.first.first, point.first.second), true);
          }
        }
      } else if (command == "fold along y") {
        is >> y_fold;

        for (const auto &point : source_point_map) {
          if (point.first.first > x_fold || point.first.second > y_fold * 2) {
            // Point past fold boundaries (not including current fold operation)
            continue;
          }

          if (point.first.second > y_fold) {
            dest_point_map.emplace(std::make_pair(point.first.first, y_fold * 2 - point.first.second), true);
          } else {
            dest_point_map.emplace(std::make_pair(point.first.first, point.first.second), true);
          }
        }
      } else {
        throw solver_runtime_error("Could not process fold command: " + command);
      }

      on_beta = !on_beta;
    }
  }


  const auto &point_map = on_beta ? point_map_beta : point_map_alpha;

  std::string result;

  for (unsigned int pos = 0; pos < x_fold; pos += 5) {
    std::string letter;

    for (unsigned int y = 0; y < LETTER_HEIGHT; ++y) {
      for (unsigned int x = pos; x < pos + 4; ++x) {
        if (point_map.find(std::make_pair(x, y)) != point_map.end()) {
          letter.push_back('#');
        } else {
          letter.push_back('.');
        }
      }
    }

    if (letter == Letter_B()) {
      result.push_back('B');
    } else if (letter == Letter_F()) {
      result.push_back('F');
    } else if (letter == Letter_G()) {
      result.push_back('G');
    } else if (letter == Letter_H()) {
      result.push_back('H');
    } else if (letter == Letter_L()) {
      result.push_back('L');
    } else if (letter == Letter_R()) {
      result.push_back('R');
    } else if (letter == Letter_U()) {
      result.push_back('U');
    } else {
      throw solver_runtime_error("Could not read letter in result: " + letter);
    }

    letter.clear();
  }

  return result;
}

TEST_CASE("testing solver for day 13 part 2 - dotted paper folding")
{
  Solver_13_part2 solver;

  std::istringstream is(std::string{ R"(
    381,143
    954,841
    1061,183
    654,247
    30,192
    72,215
    1223,680
    512,367
    150,747
    85,890
    495,127
    714,710
    31,238
    242,878
    1218,772
    862,654
    1190,710
    112,229
    276,679
    1225,240
    1104,654
    1120,175
    266,690
    142,79
    977,639
    626,203
    1134,147
    898,206
    438,731
    75,800
    731,576
    932,287
    283,236
    659,222
    241,751
    682,182
    305,612
    651,218
    152,491
    890,100
    788,823
    1198,229
    842,189
    1283,789
    887,113
    17,679
    92,122
    831,21
    894,637
    803,52
    1235,36
    373,669
    512,444
    864,376
    184,316
    104,822
    303,340
    82,289
    465,878
    890,794
    1041,29
    925,116
    847,553
    905,330
    296,382
    708,387
    1255,718
    530,669
    977,460
    1153,568
    30,30
    350,740
    872,781
    944,893
    1069,143
    438,427
    1222,425
    925,555
    1034,215
    547,379
    1233,567
    982,373
    1054,754
    110,508
    214,550
    291,505
    1036,163
    1089,834
    1223,214
    569,415
    1179,534
    529,578
    206,654
    1082,551
    495,655
    353,871
    528,838
    224,431
    380,283
    455,505
    1168,130
    626,483
    687,758
    3,672
    55,340
    907,599
    124,369
    328,73
    1123,396
    194,171
    903,892
    80,660
    277,184
    1233,631
    736,821
    13,750
    562,502
    1307,373
    447,65
    654,763
    43,130
    781,578
    711,610
    159,319
    323,890
    1164,679
    535,829
    888,178
    1190,240
    110,59
    370,800
    135,789
    1111,389
    27,789
    1283,229
    828,315
    1175,789
    736,597
    857,578
    1151,486
    1236,693
    1218,271
    1087,444
    741,206
    947,420
    803,207
    1178,7
    756,808
    507,394
    1178,439
    550,126
    1233,782
    1218,392
    13,669
    805,893
    1153,12
    58,236
    1094,337
    689,684
    793,464
    176,747
    338,102
    402,21
    1004,652
    1044,876
    1200,59
    900,458
    238,469
    1242,579
    1163,388
    142,415
    574,682
    684,203
    518,271
    359,126
    522,103
    20,541
    1168,344
    756,86
    117,673
    454,838
    763,379
    982,719
    132,119
    202,416
    623,880
    1297,750
    1297,669
    1101,730
    723,561
    1210,346
    579,576
    888,40
    135,658
    589,599
    944,445
    201,354
    1240,779
    175,626
    1005,612
    462,365
    1168,792
    897,143
    395,687
    311,239
    1233,711
    364,488
    266,876
    1225,767
    924,408
    602,785
    492,323
    618,591
    997,673
    728,763
    1168,450
    77,711
    782,838
    820,312
    780,221
    835,282
    682,600
    733,858
    1252,184
    326,424
    403,599
    320,425
    885,798
    693,876
    954,467
    586,315
    127,540
    282,334
    162,246
    58,122
    862,206
    946,854
    582,315
    579,688
    159,224
    1297,228
    691,551
    348,176
    49,184
    904,430
    940,352
    55,718
    525,98
    268,408
    1168,367
    512,311
    1178,190
    704,54
    982,315
    1052,635
    432,574
    940,542
    937,893
    792,175
    405,241
    577,782
    512,226
    313,673
    266,602
    474,231
    885,730
    554,86
    105,50
    594,689
    628,600
    522,375
    140,408
    576,766
    1134,747
    366,893
    1020,488
    1307,72
    878,320
    855,729
    773,92
    298,871
    85,767
    25,603
    862,10
    1282,19
    753,388
    311,500
    185,108
    277,374
    967,207
    699,289
    969,225
    328,358
    80,297
    733,782
    669,666
    815,127
    1049,599
    1206,822
    1280,330
    1285,695
    231,409
    925,778
    3,879
    726,660
    146,679
    651,73
    387,551
    52,11
    120,436
    1141,528
    1046,429
    95,561
    28,691
    549,60
    21,612
    744,201
    408,361
    304,826
    75,878
    1039,4
    897,222
    1235,654
    915,207
    1260,663
    387,343
    77,40
    542,229
    1119,117
    684,427
    924,684
    788,375
    651,676
    572,798
    741,448
    616,100
    464,679
    686,416
    132,551
    1094,557
    750,707
    579,206
    97,718
    748,271
    276,635
    651,374
    1022,557
    124,669
    229,164
    758,207
    693,764
    142,444
    92,772
    1210,824
    416,649
    266,428
    956,712
    1124,714
    224,483
    209,164
    1225,127
    1109,204
    420,516
    1086,154
    507,52
    999,394
    969,669
    1168,479
    88,245
    1141,814
    736,350
    1282,203
    792,600
    863,513
    1258,355
    94,187
    880,411
    1062,16
    768,229
    686,390
    894,245
    914,623
    157,12
    52,387
    378,623
    894,693
    1203,442
    94,35
    1218,563
    453,578
    551,86
    659,521
    1072,705
    254,91
    1186,525
    659,218
    627,789
    728,131
    888,707
    656,22
    224,17
    1109,652
    820,548
    932,831
    679,219
    309,455
    944,449
    1216,854
    1059,815
    962,176
    1068,16
    1069,751
    1260,792
    226,536
    1044,154
    60,226
    946,406
    393,14
    1086,226
    378,831
    890,516
    431,444
    853,868
    401,887
    631,675
    617,802
    194,723
    542,217
    813,882
    1275,119
    1232,318
    1034,565
    753,718
    932,259
    1242,271
    268,184
    460,275
    167,94
    1178,847
    599,340
    623,758
    1151,821
    377,798
    1068,878
    258,635
    1034,550
    462,81
    604,439
    1251,343
    731,688
    376,632
    1181,882
    1125,786
    999,500
    364,504
    50,834
    621,684
    396,44
    1245,151
    1161,414
    1021,466
    1088,822
    184,652
    334,425
    266,204
    758,707
    535,381
    464,623
    43,289
    224,226
    175,873
    416,245
    628,712
    1237,455
    1272,150
    1126,653
    1012,759
    68,271
    715,343
    350,154
    1235,800
    1250,226
    244,10
    944,1
    629,654
    157,326
    1001,7
    388,674
    589,606
    922,292
    517,430
    733,205
    410,605
    1280,504
    184,653
    1015,598
    238,189
    659,676
    1193,673
    748,772
    905,564
    907,634
    377,350
    572,884
    582,131
    626,243
    1034,679
    798,31
    120,240
    28,243
    478,147
    284,38
    654,647
    792,719
    492,312
    189,654
    74,873
    708,539
    249,183
    602,539
    537,506
    628,47
    651,521
    296,512
    624,116
    604,565
    890,563
    1260,455
    785,658
    756,346
    574,43
    249,653
    1238,763
    522,280
    569,240
    1213,611
    487,710
    200,751
    1280,414
    729,215
    616,673
    403,260
    228,551
    820,582
    584,115
    432,434
    900,10
    1160,747
    888,803
    815,575
    624,390
    1026,542
    1260,887
    1205,834
    410,458
    1020,87
    780,413
    850,275
    1009,240
    721,295
    1272,744
    569,448
    846,719
    706,792
    1007,554
    676,886
    586,719
    1190,688
    452,520
    200,56
    1190,206
    738,884
    267,103
    28,701
    626,53
    629,603
    88,425
    842,856
    1294,455
    602,508
    1059,257
    460,495
    492,771
    1183,354
    1255,340
    142,527
    1210,808
    716,689
    301,240
    944,72
    264,518
    422,406
    910,803
    979,442
    544,387
    400,215
    616,744
    222,822
    805,1
    13,144
    897,560
    309,7
    753,631
    604,117
    872,427
    681,647
    1235,240
    1061,856
    1190,654
    766,387
    35,801
    378,287
    879,198
    569,206
    490,312
    1168,764
    730,21
    551,24
    599,610
    512,653
    366,449
    88,201
    92,271
    865,730
    632,740
    631,219
    1240,115
    1123,274
    328,373
    574,821
    678,740
    1242,626
    1235,438
    612,352
    679,610
    959,554
    813,80
    721,158
    448,10
    1252,658
    433,589
    185,786
    1245,599
    880,292
    28,483
    301,871
    167,224
    903,450
    651,452
    416,201
    1022,374
    562,54
    1170,408
    281,225
    410,10
    1141,12
    884,551
    1043,343
    510,603
    519,236
    887,857
    1143,74
    132,774
    550,296
    724,315
    1143,261
    378,719
    1175,236
    788,519
    1124,705
    139,172
    413,591
    242,16
    341,669
    706,264
    594,241
    3,224
    773,221
    879,444
    291,474
    271,4
    378,679
    1089,813
    522,775
    1179,360
    965,215
    855,876
    855,781
    624,778
    847,341
    1185,802
    328,821
    877,81
    186,180
    1284,623
    676,606
    562,772
    1022,337
    30,116
    216,337
    749,362
    847,777
    363,57
    900,829
    984,521
    949,658
    74,469
    463,553
    791,105
    125,802
    331,390
    1232,128
    848,365
    907,260
    478,595
    321,367
    1109,578
    999,655
    201,242
    1072,649
    865,388
    1094,163
    1241,828
    187,274
    366,520
    569,446
    94,488
    652,184
    425,730
    26,600
    828,579
    502,403
    463,117
    447,513
    633,658
    932,271
    425,544
    298,471
    634,456
    788,614
    700,560
    1213,487
    741,672
    200,838
    509,455
    782,751
    264,205
    33,348
    1307,879
    525,546
    348,718
    845,878
    1268,682
    313,221
    1042,408
    479,21
    687,14
    587,561
    984,175
    1238,679
    623,14
    937,449
    760,126
    979,890
    28,53
    321,460
    862,884
    1026,38
    441,856
    72,679
    606,840
    592,429
    986,262
    223,696
    472,155
    577,406
    422,854
    882,360
    750,488
    572,515
    1135,268
    604,663
    888,406
    773,506
    567,886
    1184,591
    480,43
    1131,164
    724,402
    512,578
    201,204
    1289,684
    455,876
    1136,851
    52,81
    1044,204
    589,295
    741,890
    1072,469
    341,225
    1089,60
    572,96
    1260,24
    1307,676
    1034,774
    721,151
    681,240
    1004,204
    880,877
    1076,383
    363,420
    396,623
    706,117
    378,215
    1215,857
    1236,469
    1007,667
    232,707
    1289,282
    167,74
    651,222
    831,873
    497,814
    773,388
    85,127
    629,647
    1006,68
    736,544
    1109,354
    1063,444
    522,614
    512,792
    818,373
    487,185
    589,151
    415,662
    514,714
    1078,707
    1242,763
    806,299
    1079,409
    954,759
    1205,844
    52,59
    569,672
    741,479
    939,829
    455,389
    1051,207
    182,603
    1273,130
    813,814
    858,30
    234,63
    1218,740
    110,835
    1274,303
    915,687
    679,284
    729,7
    1210,70
    1151,408
    580,21
    856,751
    933,350
    711,564
    580,873
    748,47
    1044,690
    587,225
    602,109
    490,582
    356,759
    328,536
    649,316
    977,367
    745,663
    987,890
    517,16
    1252,236
    446,376
    73,679
    708,508
    1148,694
    1307,749
    1053,66
    288,333
    197,236
    984,73
    221,50
    258,259
    62,824
    1061,815
    1171,620
    801,455
    364,303
    1210,86
    431,198
    561,532
    358,512
    1166,739
    1216,488
    793,16
    706,663
    447,829
    922,220
    422,259
    1217,532
    1228,379
    400,803
    708,507
    80,682
    962,877
    787,744
    162,694
    733,112
    628,182
    1233,183
    1158,491
    16,455
    1233,260
    135,236
    385,865
    705,262
    863,65
    783,540
    1228,96
    303,284
    581,887
    77,634
    962,718
    552,207
    15,599
    1066,458
    1222,469
    594,242
    97,487
    100,86
    798,241
    879,450
    515,274
    780,669
    80,73
    1178,551
    1022,333
    1206,434
    159,486
    251,260
    316,102
    624,480
    1121,430
    445,730
    562,346
    1061,711
    271,130
    452,374
    1170,486
    199,876
    1261,632
    552,687
    888,259
    1001,679
    85,240
    745,567
    74,201
    537,316
    224,463
    261,295
    487,261
    1268,352
    582,763
    946,591
    232,187
    706,439
    1135,873
    144,739
    522,71
    597,477
    693,802
    798,792
    1225,336
    326,623
    1126,316

    fold along x=655
    fold along y=447
    fold along x=327
    fold along y=223
    fold along x=163
    fold along y=111
    fold along x=81
    fold along y=55
    fold along x=40
    fold along y=27
    fold along y=13
    fold along y=6
  )" });

  CHECK(solver.solve(is) == "HLBUBGFR");
}

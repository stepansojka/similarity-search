#ifndef LEVENSHTEIN_HPP
#define LEVENSHTEIN_HPP

#include <algorithm>

#include "matrix.hpp"

namespace levenshtein
{
  /*
    Computes Levenshtein distance of two strings using
    the Wagner-Fisher algorithm.

    Levenshtein distance is a string metric, i. e. a measure
    of similarity between two strings.

    see:
      https://en.wikipedia.org/wiki/Levenshtein_distance
      https://en.wikipedia.org/wiki/Wagner%E2%80%93Fischer_algorithm
  */
  template<typename T>
  unsigned levenshtein_distance(const T& lhs, const T& rhs)
  {
    matrix<unsigned, false> m(lhs.size()+1, rhs.size()+1);

    m.at(0, 0) = 0;

    for (auto i = 1U; i < m.rows; ++i)
      m.at(i, 0) = i;

    for (auto j = 1U; j < m.cols; ++j)
      m.at(0, j) = j;

    for (auto i = 1U; i < m.rows; ++i)
      for (auto j = 1U; j < m.cols; ++j)
        if (lhs[i-1] == rhs[j-1])
          m.at(i, j) = m.at(i-1, j-1);
        else
          m.at(i, j) =
            std::min({
                      m.at(i-1, j) + 1,
                      m.at(i, j-1) + 1,
                      m.at(i-1, j-1) + 1
              });

    return m.at(m.rows-1, m.cols-1);
  }
}

#endif

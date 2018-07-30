#include <algorithm>

#include "matrix.hpp"

namespace levenshtein
{
  template<typename T>
  auto levenshtein_distance(const T& lhs, const T& rhs)
  {
    matrix<typename T::size_type, false> m(lhs.size()+1, rhs.size()+1);

    m.at(0, 0) = 0;

    for (auto i = 1; i < m.rows; ++i)
      m.at(i, 0) = i;

    for (auto j = 1; j < m.cols; ++j)
      m.at(0, j) = j;

    for (auto i = 1; i < m.rows; ++i)
      for (auto j = 1; j < m.cols; ++j)
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

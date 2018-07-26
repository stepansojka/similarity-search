#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

using namespace std;

template<typename T, bool check_bounds=true>
class matrix
{
public:
  typedef vector<T> M;
  typedef typename M::size_type size_type;

  const size_type rows;
  const size_type cols;

  matrix(size_type rows, size_type cols):
    rows(rows), cols(cols),
    m(M(rows*cols))
  {}

  auto& at(size_type row, size_type col)
  {
    do_check_bounds(row, col);
    return m[row*cols + col];
  }

  auto at(size_type row, size_type col) const
  {
    do_check_bounds(row, col);
    return m[row*cols + col];
  }

private:
  M m;

  void do_check_bounds(size_type row, size_type col) const
  {
    if constexpr(check_bounds)
         if (row < 0 || row >= rows || col < 0 || col >= cols )
           throw out_of_range("matrix index out of range");
  }
};


template<typename T>
auto levenshtein(const T& lhs, const T& rhs)
{
  matrix<typename T::size_type, false> m(lhs.size()+1, rhs.size()+1);

  m.at(0, 0) = 0;

  for (auto i = 1; i < m.rows; ++i)
    {
      m.at(i, 0) = i;

      for (auto j = 1; j < m.cols; ++j)
        {
          m.at(0, j) = j;

          if (lhs[i-1] == rhs[j-1])
            m.at(i, j) = m.at(i-1, j-1);
          else
            m.at(i, j) =
                min({
                     m.at(i-1, j) + 1,
                     m.at(i, j-1) + 1,
                     m.at(i-1, j-1) + 1
                  });
        }
    }

  return m.at(m.rows-1, m.cols-1);
}


int main()
{
  cout << levenshtein("hello"s, "hell"s) << endl;
  cout << levenshtein("hello"s, "h"s) << endl;
  cout << levenshtein("hello"s, "helLo"s) << endl;

  return 0;
}

#include <string>
#include <vector>
#include <iostream>
#include <algorithm>
#include <stdexcept>

using namespace std;


struct strict_checking
{
  template<typename T>
  static void check_bounds(const T& m, typename T::size_type row, typename T::size_type col)
  {
    if (row >= m.rows || col >= m.cols)
      throw out_of_range("matrix index out of range");
  }
};


struct no_checking
{
  template<typename T>
  static void check_bounds(const T&, typename T::size_type, typename T::size_type)
  {}
};


template<typename T, typename C=no_checking>
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
    C::check_bounds(*this, row, col);
    return m[row*cols + col];
  }

  auto at(size_type row, size_type col) const
  {
    C::check_bounds(*this, row, col);
    return m[row*cols + col];
  }

private:
  M m;
};


template<typename T>
auto levenshtein(const T& lhs, const T& rhs)
{
  matrix<typename T::size_type> m(lhs.size()+1, rhs.size()+1);

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

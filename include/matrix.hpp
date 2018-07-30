#include <vector>
#include <stdexcept>

namespace levenshtein
{
  template<typename T, bool check_bounds=true>
  class matrix
  {
  public:
    typedef std::vector<T> M;
    typedef typename M::size_type size_type;

    const size_type rows, cols;

    matrix(size_type rows, size_type cols):
      rows(rows), cols(cols),
      m(M(rows*cols))
    {}

    auto& at(size_type row, size_type col)
    {
      return element_ref(row, col);
    }

    auto at(size_type row, size_type col) const
    {
      return element_ref(row, col);
    }

  private:
    M m;

    auto& element_ref(size_type row, size_type col)
    {
      if constexpr(check_bounds)
                    if (row < 0 || row >= rows || col < 0 || col >= cols )
                      throw std::out_of_range("matrix index out of range");

      return m[row*cols + col];
    }
  };
}

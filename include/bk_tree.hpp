#ifndef BK_TREE_HPP
#define BK_TREE_HPP

#include <map>
#include <functional>
#include <tuple>
#include <limits>

#include "levenshtein.hpp"

namespace bk
{
  /*
    Burkhard-Keller Tree implementation.

    The Burkhard-Keller Tree can be used for searching a set of keys
    to find a key which is closest to a given query key.

    Note that strict equality is not required. It is enough that
    the query key is similar to one of the keys present in the tree.
    
    see:
      https://signal-to-noise.xyz/post/bk-tree/
      http://blog.notdot.net/2007/4/Damn-Cool-Algorithms-Part-1-BK-Trees
   */
  template <typename T>
  struct bk_tree
  {
    typedef std::map<unsigned, bk_tree> children;
    typedef std::function<unsigned(const T&, const T&)> metric;

    bk_tree(T value, metric metric = levenshtein::levenshtein_distance<T>):
      m_value(value),
      m_metric(metric)
    {}

    void insert(T value)
    {
      auto d = m_metric(value, m_value);
      if (d == 0)
        return;

      auto i = m_children.find(d);
      if (i == m_children.end())
        m_children.insert({d, bk_tree(value, m_metric)});
      else
        i->second.insert(value);
    }

    auto search(T value, unsigned threshold = std::numeric_limits<unsigned>::max())
    {
      auto tau = threshold;
      T result;

      search_rec(value, tau, result);
      return std::tuple(result, tau);
    }

  private:
    children m_children;
    T m_value;
    metric m_metric;

    void search_rec(const T& value, unsigned& tau, T& result)
    {
      auto d = m_metric(value, m_value);
      if (d <= tau)
        {
          tau = d;
          result = m_value;
        }

      for (auto i = m_children.lower_bound(d - tau);
           i != m_children.end() && (i->first <= d + tau);
           ++i)
        {
          i->second.search_rec(value, tau, result);
        }
    }
  };
}

#endif

#ifndef BK_TREE_HPP
#define BK_TREE_HPP

#include <memory>
#include <map>
#include <functional>
#include <tuple>
#include <limits>

#include "levenshtein.hpp"

namespace bk
{
  template <typename T>
  struct bk_tree
  {
    typedef std::shared_ptr<bk_tree> shared_ptr;
    typedef std::map<unsigned, shared_ptr> children;
    typedef std::function<unsigned(const T&, const T&)> distance_fn;

    bk_tree(T value, distance_fn distance_fn = levenshtein::levenshtein_distance<T>):
      m_value(value),
      m_distance_fn(distance_fn)
    {}

    void insert(T value)
    {
      auto d = m_distance_fn(value, m_value);
      if (d == 0)
        return;

      auto i = m_children.find(d);
      if (i == m_children.end())
        m_children[d] = std::make_shared<bk_tree>(value, m_distance_fn);
      else
        i->second->insert(value);
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
    distance_fn m_distance_fn;

    void search_rec(const T& value, unsigned& tau, T& result)
    {
      auto d = m_distance_fn(value, m_value);
      if (d <= tau)
        {
          tau = d;
          result = m_value;
        }

      for (auto i = m_children.lower_bound(d - tau);
           i != m_children.end() && (i->first <= d + tau);
           ++i)
        {
          i->second->search_rec(value, tau, result);
        }
    }
  };
}

#endif

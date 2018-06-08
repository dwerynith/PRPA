#pragma once
#include "IDictionary.hpp"
#include "trie.hpp"
#include <set>
#include <mutex>
#include <vector>

class Dictionary : public IDictionary
{
public:
  Dictionary() = default;

  result_t      search(const std::string& w) const final;
  void          insert(const std::string& w) final;
  void          erase(const std::string& w) final;

private:
  Trie root_;
};



template <class Iterator>
naive_dictionary::naive_dictionary(Iterator begin, Iterator end)
  : m_set(begin, end)
{
}

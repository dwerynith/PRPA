#include "trie.hpp"

#include <memory>
#include <string>

Trie::Trie()
{
    is_end_ = false;
    for (int i = 0; i < 26; ++i)
    {
        childrens_[i] = nullptr;
    }
}

void Trie::add(const std::string& w, unsigned int i)
{
    if (i == w.length())
        is_end_ = true;
    else
    {
        if (!childrens_[w[i] - 'a'])
            childrens_[w[i] - 'a'] = std::make_shared<Trie>(Trie());
        childrens_[w[i] - 'a']->add(w, i + 1);
    }
}

void Trie::remove(const std::string& w, unsigned int i)
{
    if (i == w.length())
        is_end_ = false;
    else if (childrens_[w[i] - 'a'])
        childrens_[w[i] - 'a']->remove(w, i + 1);
}

bool Trie::find(const std::string& w, unsigned int i)
{
    if (i == w.length())
        return is_end_;

    if (childrens_[w[i] - 'a'])
        return childrens_[w[i] - 'a']->find(w, i + 1);
    
    return false;
}

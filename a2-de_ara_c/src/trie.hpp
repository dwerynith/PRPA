#pragma once

#include <memory>
#include <string>

class Trie 
{
public:
    Trie();

    void add(const std::string& w, unsigned int i = 0);
    void remove(const std::string& w, unsigned int i = 0);
    bool find(const std::string& w, unsigned int i = 0);

private:
    std::shared_ptr<Trie> childrens_[26];
    bool is_end_;
};

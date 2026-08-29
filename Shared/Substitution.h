#pragma once

#include <string>
#include <vector>

class Substitution
{
public:
    Substitution(std::vector<std::string> data) : data_(std::move(data)) {}

    const std::string &get(long idx) const { return data_.at(idx); }
    long count() const { return data_.size(); }
    const std::vector<std::string> &data() const { return data_; }

private:
    std::vector<std::string> data_;
};
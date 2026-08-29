#include "Utility.h"

#include <unordered_map>

using namespace std;

std::string Utility::str(const std::vector<long> &vector)
{
    std::string result = "[";

    for (size_t i = 0; i < vector.size(); ++i)
    {
        result += std::to_string(vector[i]);
        if (i + 1 < vector.size())
        {
            result += ", ";
        }
    }

    result += "]";
    return result;
}

std::string Utility::str(const std::vector<bool> &vector)
{
    std::string result = "[";

    for (size_t i = 0; i < vector.size(); ++i)
    {
        result += std::to_string(vector[i]);
        if (i + 1 < vector.size())
        {
            result += ", ";
        }
    }

    result += "]";
    return result;
}

std::string Utility::str(const std::vector<std::vector<long>> &matrix)
{
    std::string result = "[\n";

    for (size_t i = 0; i < matrix.size(); ++i)
    {
        result += "  " + str(matrix[i]);
        if (i + 1 < matrix.size())
        {
            result += ",\n";
        }
        else
        {
            result += "\n";
        }
    }

    result += "]";
    return result;
}

std::string Utility::str(const std::vector<std::vector<bool>> &matrix)
{
    std::string result = "[\n";

    for (size_t i = 0; i < matrix.size(); ++i)
    {
        result += "  [";
        for (size_t j = 0; j < matrix[i].size(); ++j)
        {
            result += (matrix[i][j] ? "true" : "false");
            if (j + 1 < matrix[i].size())
            {
                result += ", ";
            }
        }
        result += "]";

        if (i + 1 < matrix.size())
        {
            result += ",\n";
        }
        else
        {
            result += "\n";
        }
    }

    result += "]";
    return result;
}

string Utility::str(const unordered_map<long, long> &mp)
{
    string res = "";
    for (auto [key, value] : mp)
    {
        res += to_string(key) + " " + to_string(value) + "\n";
    }
    return res;
}

string Utility::str(const std::set<long> &st)
{
    string res = "";
    for (const auto &val : st)
    {
        res += std::to_string(val) + "\n";
    }
    return res;
}
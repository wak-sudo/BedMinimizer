#include <stdexcept>

/// @brief Thrown after many attempts fail to generate a randomly valid DAG or DFA
class RandomGeneratorExp : public std::exception
{
private:
    std::string msg_;

public:
    explicit RandomGeneratorExp(std::string msg) : msg_(std::move(msg)) {}

    const char *what() const noexcept override
    {
        return msg_.c_str();
    }
};
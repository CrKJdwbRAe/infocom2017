#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <cmath>

namespace cam {

struct Function {
    Function() = default; 

    Function(std::string const& name) {
        if (name == "dict") {
            mu_ = [] (double x, double y) -> double {
                return std::log(std::exp(x) + std::exp(y));
            };
        } else if (name == "max") {
            mu_ = [] (double x, double y) -> double {
                return std::max(x, y);
            };
        } else if (name == "min") {
            mu_ = [] (double x, double y) -> double {
                return std::min(x, y);
            };
        } else if (name == "euqlid") {
            mu_ = [] (double x, double y) -> double {
               return std::sqrt(x * x + y * y);
            };
        } else {
            abort();
        }
    }

    auto operator()(double x, double y) const -> double {
        return mu_(x, y);
    }

    auto name() const -> std::string const& {
        return name_;
    }

private:
    std::function<double(double, double)> mu_;
    std::string name_;
};

}

#endif // FUNCTIONS_H

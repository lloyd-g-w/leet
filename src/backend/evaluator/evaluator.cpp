#include <iomanip>
#include <iostream>
#include <sstream>

#include "../maths/maths.hpp"
#include "evaluator.hpp"

#define SF_LIMIT 11

namespace std_cells {
// Prototype

static str to_str(const double &value);
int count_decimal_places(double num);

// End prototype

str evaluate(const ast_node &ast, std::optional<grid> g) {
    // Literals
    if (ast->type == ast_struct::type::INT ||
        ast->type == ast_struct::type::FLOAT ||
        ast->type == ast_struct::type::STRING) {
        return ast->value;
    }

    // Functions
    if (ast->type == ast_struct::type::FUNCTION) {
        if (ast->value == "SUM") {
            if (ast->children.size() < 1)
                throw exception::cell_exception(
                    "SUM function requires at least 1 argument");

            std::vector<double> nums;
            for (const auto &child : ast->children) {
                nums.push_back(std::stod(evaluate(child)));
            }

            double res = lm::sum(nums);
            return to_str(res);
        }

        if (ast->value == "PRODUCT" || ast->value == "PROD") {
            if (ast->children.size() < 1)
                throw exception::cell_exception(
                    "PRODUCT function requires at least 1 argument");

            std::vector<double> nums;
            for (const auto &child : ast->children) {
                nums.push_back(std::stod(evaluate(child)));
            }

            double res = lm::product(nums);
            return to_str(res);
        }

        if (ast->value == "SQRT") {
            if (ast->children.size() != 1)
                throw exception::cell_exception(
                    "SQRT function requires 1 argument");

            str child = evaluate(ast->children[0]);

            double res = lm::sqrt(std::stod(child));
            return to_str(res);
        }

        str error_msg = "ERROR: Could not evaluate function " + ast->value;
        throw exception::cell_exception(error_msg);
    }

    // Operators
    if (ast->type == ast_struct::type::OPERATOR) {
        if (ast->value == "POS") {
            if (ast->children.size() != 1)
                throw exception::cell_exception(
                    "POS operator requires 1 argument");

            return evaluate(ast->children[0]);
        }

        if (ast->value == "NEG") {
            if (ast->children.size() != 1)
                throw exception::cell_exception(
                    "NEG operator requires 1 argument");

            str child = evaluate(ast->children[0]);

            double res = lm::mul(-1, std::stod(child));
            return to_str(res);
        }

        if (ast->value == "ADD") {
            if (ast->children.size() != 2)
                throw exception::cell_exception(
                    "ADD operator requires 2 arguments");

            str left = evaluate(ast->children[0]);
            str right = evaluate(ast->children[1]);

            double res = lm::add(std::stod(left), std::stod(right));
            return to_str(res);
        }

        if (ast->value == "SUB") {
            if (ast->children.size() != 2)
                throw exception::cell_exception(
                    "SUB operator requires 2 arguments");

            str left = evaluate(ast->children[0]);
            str right = evaluate(ast->children[1]);

            double res = lm::sub(std::stod(left), std::stod(right));
            return to_str(res);
        }

        if (ast->value == "MUL") {
            if (ast->children.size() != 2)
                throw exception::cell_exception(
                    "MUL operator requires 2 arguments");

            str left = evaluate(ast->children[0]);
            str right = evaluate(ast->children[1]);

            double res = lm::mul(std::stod(left), std::stod(right));
            return to_str(res);
        }

        if (ast->value == "DIV") {
            if (ast->children.size() != 2)
                throw exception::cell_exception(
                    "DIV operator requires 2 arguments");

            str left = evaluate(ast->children[0]);
            str right = evaluate(ast->children[1]);

            double res = lm::div(std::stod(left), std::stod(right));
            return to_str(res);
        }

        if (ast->value == "POW") {
            if (ast->children.size() != 2)
                throw exception::cell_exception(
                    "EXP operator requires 2 arguments");

            str left = evaluate(ast->children[0]);
            str right = evaluate(ast->children[1]);

            double res = lm::pow(std::stod(left), std::stod(right));
            return to_str(res);
        }
    }

    throw exception::cell_exception("ERROR: Could not evaluate AST node");
}

static str to_str(const double &value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

}  // namespace std_cells

#include <iomanip>
#include <iostream>
#include <sstream>

#include "../maths/maths.hpp"
#include "evaluator.hpp"

#define SF_LIMIT 11

namespace std_cells {
// Prototype

static eval_res evaluate_function(const ast_node &ast, std::optional<grid> g);
static eval_res evaluate_operator(const ast_node &ast, std::optional<grid> g);
static str to_str(const double &value);
static bool is_whole_num(const double &number);
static bool is_num(const eval_res &res);

// End prototype

eval_res evaluate(const ast_node &ast, std::optional<grid> g) {
    switch (ast->type) {
        // LITERALS
        case ast_struct::type::INT:
            return {cell::type::INT, std::stod(ast->value)};
        case ast_struct::type::DECIMAL:
            return {cell::type::DECIMAL, std::stod(ast->value)};
        case ast_struct::type::STRING:
            return {cell::type::STRING, INFINITY, ast->value};

        case ast_struct::type::FUNCTION: {
            return evaluate_function(ast, g);
        }

        case ast_struct::type::OPERATOR: {
            return evaluate_operator(ast, g);
        }

        default: break;
    }

    throw exception::cell_exception("ERROR: Could not evaluate AST node");
}

static eval_res evaluate_function(const ast_node &ast,
                                  std::optional<grid> g = std::nullopt) {
    if (ast->type != ast_struct::type::FUNCTION)
        throw exception::cell_exception("ERROR: AST node is not a function yet "
                                        "evaluate_function was called");

    if (ast->value == "SUM") {
        if (ast->children.size() < 1)
            throw exception::cell_exception(
                "SUM function requires at least 1 argument");

        std::vector<double> nums;
        for (const auto &child : ast->children) {
            auto res = evaluate(child);

            if (!is_num(res))
                throw exception::cell_exception(
                    "ERROR: SUM function cannot accept non-number "
                    "arguments");

            nums.push_back(res.number);
        }

        double res = lm::sum(nums);
        cell::type type = cell::type::DECIMAL;
        if (is_whole_num(res))
            type = cell::type::INT;
        return {type, res};
    }

    if (ast->value == "PRODUCT" || ast->value == "PROD") {
        if (ast->children.size() < 1)
            throw exception::cell_exception(
                "PRODUCT function requires at least 1 argument");

        std::vector<double> nums;
        for (const auto &child : ast->children) {
            auto res = evaluate(child);

            if (!is_num(res))
                throw exception::cell_exception(
                    "ERROR: PRODUCT function cannot accept non-number "
                    "arguments");

            nums.push_back(res.number);
        }

        double res = lm::product(nums);
        cell::type type = cell::type::DECIMAL;
        if (is_whole_num(res))
            type = cell::type::INT;
        return {type, res};
    }

    if (ast->value == "SQRT") {
        if (ast->children.size() != 1)
            throw exception::cell_exception(
                "SQRT function requires 1 argument");

        auto child = evaluate(ast->children[0]);

        if (!is_num(child))
            throw exception::cell_exception(
                "ERROR: SQRT function cannot accept non-number arguments");

        double res = lm::sqrt(child.number);
        return {is_whole_num(res) ? cell::type::INT : cell::type::DECIMAL, res};
    }

    str error_msg = "ERROR: Could not evaluate function " + ast->value;
    throw exception::cell_exception(error_msg);
}

static eval_res evaluate_operator(const ast_node &ast,
                                  std::optional<grid> g = std::nullopt) {
    if (ast->type != ast_struct::type::OPERATOR)
        throw exception::cell_exception(
            "ERROR: AST node is not an operator yet "
            "evaluate_operator was called");

    if (ast->value == "POS") {
        if (ast->children.size() != 1)
            throw exception::cell_exception("POS operator requires 1 argument");
        auto res = evaluate(ast->children[0]);

        if (!is_num(res))
            throw exception::cell_exception(
                "POS operator requires a number argument");

        return res;
    }

    if (ast->value == "NEG") {
        if (ast->children.size() != 1)
            throw exception::cell_exception("NEG operator requires 1 argument");

        auto child = evaluate(ast->children[0]);
        if (!is_num(child))
            throw exception::cell_exception(
                "NEG operator requires a number argument");

        double res = lm::mul(-1, child.number);
        return {child.type, res};
    }

    if (ast->value == "ADD") {
        if (ast->children.size() != 2)
            throw exception::cell_exception(
                "ADD operator requires 2 arguments");

        auto left = evaluate(ast->children[0]);
        auto right = evaluate(ast->children[1]);

        if (!is_num(left) || !is_num(right))
            throw exception::cell_exception(
                "ADD operator requires number arguments");

        double res = lm::add(left.number, right.number);
        return {is_whole_num(res) ? cell::type::INT : cell::type::DECIMAL, res};
    }

    if (ast->value == "SUB") {
        if (ast->children.size() != 2)
            throw exception::cell_exception(
                "SUB operator requires 2 arguments");

        auto left = evaluate(ast->children[0]);
        auto right = evaluate(ast->children[1]);

        if (!is_num(left) || !is_num(right))
            throw exception::cell_exception(
                "SUB operator requires number arguments");

        double res = lm::sub(left.number, right.number);
        return {is_whole_num(res) ? cell::type::INT : cell::type::DECIMAL, res};
    }

    if (ast->value == "MUL") {
        if (ast->children.size() != 2)
            throw exception::cell_exception(
                "MUL operator requires 2 arguments");

        auto left = evaluate(ast->children[0]);
        auto right = evaluate(ast->children[1]);

        if (!is_num(left) || !is_num(right))
            throw exception::cell_exception(
                "MUL operator requires number arguments");

        double res = lm::mul(left.number, right.number);
        return {is_whole_num(res) ? cell::type::INT : cell::type::DECIMAL, res};
    }

    if (ast->value == "DIV") {
        if (ast->children.size() != 2)
            throw exception::cell_exception(
                "DIV operator requires 2 arguments");

        auto left = evaluate(ast->children[0]);
        auto right = evaluate(ast->children[1]);

        if (!is_num(left) || !is_num(right))
            throw exception::cell_exception(
                "DIV operator requires number arguments");

        double res = lm::div(left.number, right.number);
        return {is_whole_num(res) ? cell::type::INT : cell::type::DECIMAL, res};
    }

    if (ast->value == "POW") {
        if (ast->children.size() != 2)
            throw exception::cell_exception(
                "POW operator requires 2 arguments");

        auto left = evaluate(ast->children[0]);
        auto right = evaluate(ast->children[1]);

        if (!is_num(left) || !is_num(right))
            throw exception::cell_exception(
                "POW operator requires number arguments");

        double res = lm::pow(left.number, right.number);
        return {is_whole_num(res) ? cell::type::INT : cell::type::DECIMAL, res};
    }

    throw exception::cell_exception("ERROR: Could not evaluate operator " +
                                    ast->value);
}

// ==================== HELPERS ==================== //
static str to_str(const double &value) {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << value;
    return oss.str();
}

static bool is_whole_num(const double &number) {
    return number == lm::floor(number);
}

static bool is_num(const eval_res &res) {
    return res.type == cell::type::INT || res.type == cell::type::DECIMAL;
}

}  // namespace std_cells

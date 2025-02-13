#include <iomanip>
#include <iostream>
#include <sstream>

#include "../maths/maths.hpp"
#include "evaluator.hpp"

#define SF_LIMIT 11

namespace std_cells {
// Prototype

static eval_res evaluate_function(const eval_input &input);
static eval_res evaluate_operator(const eval_input &input);
static str to_str(const double &value);
static bool is_whole_num(const double &number);
static bool is_num(const eval_res &res);

// End prototype

eval_res evaluate(const eval_input &input) {
    ast_node &ast = input.ast;
    grid &g = input.g;
    pos caller_cell_pos = input.caller_cell_pos;

    switch (ast->type) {
        // LITERALS
        case ast_struct::type::INT:
            return {cell::type::INT, std::stod(ast->value)};
        case ast_struct::type::DECIMAL:
            return {cell::type::DECIMAL, std::stod(ast->value)};
        case ast_struct::type::STRING:
            return {cell::type::STRING, INFINITY, ast->value};
        case ast_struct::type::CELL_REFERENCE: {
            auto cell_data = g.get_cell_data(ast->value);
            auto type = cell_data.eval_type;

            pos parent_pos = grid::str_to_pos(ast->value);

            if (parent_pos == caller_cell_pos)
                throw exception::cell_exception(
                    "ERROR: self-referencing is not allowed");

            std::cout << "Adding dependency to " << parent_pos.row << " "
                      << parent_pos.col << " of " << caller_cell_pos.row << " "
                      << caller_cell_pos.col << std::endl;

            g.add_dependency(parent_pos, caller_cell_pos);

            if (type == cell::type::NOT_SET)
                return {cell::type::STRING, INFINITY, cell_data.raw};

            else if (type == cell::type::STRING)
                return {cell::type::STRING, INFINITY, cell_data.eval_str};

            else
                return {type, cell_data.eval_float};
        }

        case ast_struct::type::FUNCTION: {
            return evaluate_function(input);
        }

        case ast_struct::type::OPERATOR: {
            return evaluate_operator(input);
        }

        default: break;
    }

    throw exception::cell_exception("ERROR: Could not evaluate AST node");
}

static eval_res evaluate_function(const eval_input &input) {
    ast_node &ast = input.ast;
    grid &g = input.g;
    pos caller_cell_pos = input.caller_cell_pos;

    if (ast->type != ast_struct::type::FUNCTION)
        throw exception::cell_exception("ERROR: AST node is not a function yet "
                                        "evaluate_function was called");

    if (ast->value == "SUM") {
        if (ast->children.size() < 1)
            throw exception::cell_exception(
                "SUM function requires at least 1 argument");

        std::vector<double> nums;
        for (auto &child : ast->children) {
            eval_input child_input = {child, g, caller_cell_pos};
            auto res = evaluate(child_input);

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
        for (auto &child : ast->children) {
            eval_input child_input = {child, g, caller_cell_pos};
            auto res = evaluate(child_input);

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

        auto child = evaluate(ast->children[0], g, caller_cell_pos);

        if (!is_num(child))
            throw exception::cell_exception(
                "ERROR: SQRT function cannot accept non-number arguments");

        double res = lm::sqrt(child.number);
        return {is_whole_num(res) ? cell::type::INT : cell::type::DECIMAL, res};
    }

    str error_msg = "ERROR: Could not evaluate function " + ast->value;
    throw exception::cell_exception(error_msg);
}

static eval_res evaluate_operator(const ast_node &ast, grid &g,
                                  pos caller_cell_pos) {
    if (ast->type != ast_struct::type::OPERATOR)
        throw exception::cell_exception(
            "ERROR: AST node is not an operator yet "
            "evaluate_operator was called");

    if (ast->value == "POS") {
        if (ast->children.size() != 1)
            throw exception::cell_exception("POS operator requires 1 argument");
        auto res = evaluate(ast->children[0], g, caller_cell_pos);

        if (!is_num(res))
            throw exception::cell_exception(
                "POS operator requires a number argument");

        return res;
    }

    if (ast->value == "NEG") {
        if (ast->children.size() != 1)
            throw exception::cell_exception("NEG operator requires 1 argument");

        auto child = evaluate(ast->children[0], g, caller_cell_pos);
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

        std::cout << "Evaluating ADD operator" << std::endl;
        std::cout << "Left: " << ast->children[0]->value << std::endl;
        auto left = evaluate(ast->children[0], g, caller_cell_pos);
        auto right = evaluate(ast->children[1], g, caller_cell_pos);

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

        auto left = evaluate(ast->children[0], g, caller_cell_pos);
        auto right = evaluate(ast->children[1], g, caller_cell_pos);

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

        auto left = evaluate(ast->children[0], g, caller_cell_pos);
        auto right = evaluate(ast->children[1], g, caller_cell_pos);

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

        auto left = evaluate(ast->children[0], g, caller_cell_pos);
        auto right = evaluate(ast->children[1], g, caller_cell_pos);

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

        auto left = evaluate(ast->children[0], g, caller_cell_pos);
        auto right = evaluate(ast->children[1], g, caller_cell_pos);

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

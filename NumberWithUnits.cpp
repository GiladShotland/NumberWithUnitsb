#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <climits>
#include <unordered_map>
#include <bits/stdc++.h>
#include <utility>
#include <unordered_map>
#include <cstdlib>
#include <vector>
#include "NumberWithUnits.hpp"
using namespace std;

namespace ariel
{
    const double EPSILON = 0.00000001;
    const int NUM_OF_OPERANDS = 2;
    const int ZERO = 0;
    const int ONE = 1;
    const int FORTY_THREE = 43;
    const int FORTY_FIVE = 45;
    const int FORTY_SIX = 46;
    const int FORTY_EIGHT = 48;
    const int FIFTY_SEVEN = 57;

    static unordered_map<string, unordered_map<string, double>> unit_map;

    void creator_of_numbers_and_conversions(const string &line);
    double read_and_parse_number(string number_with_unit, int end_of_number_index);
    bool initializer_and_create_conversion(string unit_right, int val_right, string unit_left, int val_left);
    void create_conversion(const string &unit_right, double val_right, const string &unit_left, double val_left);
    double get_conversion(const string &type_of_num1, const string &type_of_num2);
    string remove_spaces(const string &str)
    {
        string ans;
        for (int i = 0; i < str.size(); i++)
        {
            if (str.at(i) != ' ')
            {
                ans += str.at(i);
            }
        }
        return ans;
    }

    NumberWithUnits::NumberWithUnits(double value, const string &t) : value(value)
    {
        if (unit_map.find(t) == unit_map.end())
        {
            throw std::invalid_argument("invalid number type - it doesn't exist");
        }

        this->type = t;
    };
    string NumberWithUnits::get_type()

    {
        return this->type;
    }
    double NumberWithUnits::get_value() const
    {
        return this->value;
    }
    double get_conversion(const string &type_of_num1, const string &type_of_num2)
    {
        /*check if the numbers are from the same type*/
        if (type_of_num1 == type_of_num2)
        {
            return 1;
        }
        /*check if the numbers aren't from the same scale*/
        if (unit_map[type_of_num2].find(type_of_num1) == unit_map[type_of_num2].end())
        {

            throw std::logic_error("The operands are not from the same scale");
        }
        /*return the scalar for conversion */
        return unit_map[type_of_num2][type_of_num1];
    }
    int find_end_number_index(const string &number_with_unit)
    {
        for (std::string::size_type i = 0; i < number_with_unit.size(); i++)
        {
            char character = number_with_unit[i];
            if ((character < FORTY_EIGHT && character != FORTY_THREE && character != FORTY_FIVE && character != FORTY_SIX) || character > FIFTY_SEVEN)
            {
                return i;
            }
        }
        return 0;
    }
    double read_and_parse_numebr(const string &number_with_unit, int end_of_number_index)
    {
        /*getting the right substring*/
        string numstring = number_with_unit.substr(0, end_of_number_index);
        double num = std::stod(numstring);
        return num;
    }
    void creator_of_numbers_and_conversions(const string &line)
    {
        /*split the line*/
        string left_side_of_equation = line.substr(0, line.find('='));
        string right_side_of_equation = line.substr(line.find('=') + 1);
        /*taking care of left side*/
        int index = find_end_number_index(left_side_of_equation);
        double val_left = read_and_parse_number(left_side_of_equation, index);
        string unit_left = left_side_of_equation.substr(index);
        string unit_left_without_spaces = remove_spaces(unit_left);
        /*taking  care of right side*/
        index = find_end_number_index(right_side_of_equation);
        double val_right = read_and_parse_number(right_side_of_equation, index);
        string unit_right = left_side_of_equation.substr(index);
        string unit_right_without_spaces = remove_spaces(unit_right);
        create_conversion(unit_right_without_spaces, val_right, unit_left, val_left);
    }
    void create_conversion(const string &unit_right, double val_right, const string &unit_left, double val_left)
    {
        /*enter to the lines*/
        unit_map[unit_right][unit_left] = val_right / val_left;
        unit_map[unit_left][unit_right] = val_left / val_right;
        /*getting the value for converting from one unit to second unit*/
        double from_left_to_right = unit_map[unit_left][unit_right];
        double from_right_to_left = unit_map[unit_right][unit_left];
        /*making every neighbor of unit_right a neighbor of left_unit*/
        for (const auto &kv : unit_map[unit_right])
        {
            const auto &key = kv.first;
            const auto &val = kv.second;
            unit_map[unit_left][key] = val * from_left_to_right;
        }
        /*making every neighbor of unit_left a neighbor of right*/
        for (const auto &kv : unit_map[unit_left])
        {
            const auto &key = kv.first;
            const auto &val = kv.second;
            unit_map[unit_right][key] = val * from_right_to_left;
        }
    }

    void get_doubles_after_conversion(NumberWithUnits num1, NumberWithUnits num2, std::vector<double> arr)
    {
        string type_of_num1 = num1.get_type();
        string type_of_num2 = num2.get_type();
        double conversion_skalar = get_conversion(type_of_num1, type_of_num2);
        double value_of_num1 = num1.get_value();
        double value_of_num2 = num2.get_value();
        value_of_num2 *= conversion_skalar;
        arr.push_back(value_of_num1);
        arr.push_back(value_of_num2);
    }

    void NumberWithUnits::read_units(ifstream &file)
    {
        string line;
        /*start reading line by line*/
        while (getline(file, line))
        {
            creator_of_numbers_and_conversions(line);
        }
    }

    NumberWithUnits operator+(NumberWithUnits &num1, NumberWithUnits &num2)
    {
        std::vector<double> values;
        get_doubles_after_conversion(num1, num2, values);
        double new_num_value = values.at(ZERO) + values.at(ONE);
        return {new_num_value, num1.type};
    }

    NumberWithUnits &NumberWithUnits::operator+=(const NumberWithUnits &num)
    {
        string type = this->type;
        double multiplier = get_conversion(type, num.type);
        double to_add = multiplier * num.value;
        this->value += to_add;
        return *this;
    }

    NumberWithUnits operator-(NumberWithUnits &num1, NumberWithUnits &num2)
    {

        std::vector<double> values;
        get_doubles_after_conversion(num1, num2, values);
        double new_num_value = values.at(ZERO) - values.at(ONE);
        return {new_num_value, num1.type};
    }

    NumberWithUnits &NumberWithUnits::operator-=(NumberWithUnits &num)
    {
        string type = this->type;
        double multiplier = get_conversion(type, num.type);
        double to_add = multiplier * num.value;
        this->value -= to_add;
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator-()
    {
        double val = this->value;
        string type = this->type;

        return {-val, type};
    }

    NumberWithUnits operator*(double num1, NumberWithUnits &num2)
    {
        double val = num2.get_value();
        string type = num2.get_type();
        val *= num1;
        return {val, type};
    }
    NumberWithUnits operator*(NumberWithUnits &num1, double num2)
    {
        double val = num1.value;
        string type = num1.type;
        val *= num2;
        return {val, type};
    }

    bool operator>(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {

        std::vector<double> values;
        get_doubles_after_conversion(num1, num2, values);
        return values.at(ZERO) - values.at(ONE) > EPSILON;
    }

    bool operator>=(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values;
        get_doubles_after_conversion(num1, num2, values);
        double difference = values.at(ZERO) - values.at(ONE);
        return difference > EPSILON || (difference < EPSILON && difference > 0);
    }
    bool operator==(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values;
        get_doubles_after_conversion(num1, num2, values);
        return values.at(ZERO) - values.at(ONE) < EPSILON;
    }

    bool operator<(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values;
        get_doubles_after_conversion(num1, num2, values);
        return values.at(ONE) - values.at(ZERO) > EPSILON;
    }

    bool operator<=(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values;
        get_doubles_after_conversion(num1, num2, values);
        double difference = values.at(ONE) - values.at(ZERO);
        return difference > EPSILON || (difference < EPSILON && difference > ZERO);
    }
    bool operator!=(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values;
        get_doubles_after_conversion(num1, num2, values);
        return !(abs(values.at(ZERO) - values.at(ONE)) < EPSILON);
    }

    NumberWithUnits &NumberWithUnits::operator++()
    {
        this->value += ONE;
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator++(int flag)
    {
        return {this->value++, this->type};
    }

    NumberWithUnits &NumberWithUnits::operator--()
    {
        this->value -= ONE;
        return *this;
    }

    NumberWithUnits NumberWithUnits::operator--(int flag)
    {

        return {this->value--, this->type};
    }

    void operator>>(istream &input, NumberWithUnits &num)
    {
        string s;
        input >> num.value >> s >> num.type;
    }

    ostream &operator<<(ostream &os, const NumberWithUnits &input)
    {

        os << input.value << "[" << input.type << "]";
        return os;
    }
}

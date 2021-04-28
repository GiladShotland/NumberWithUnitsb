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
    const double EPSILON = 0.0001;
    const int NUM_OF_OPERANDS = 2;
    const int ZERO = 0;
    const int ONE = 1;

    static unordered_map<string, unordered_map<string, double>> unit_map;

    void create_conversion(const string &unit_right, double val_right, const string &unit_left, double val_left);
    double get_conversion(const string &type_of_num1, const string &type_of_num2);
    string remove_spaces(const string &str)
    {
        string ans;
        for (uint i = 0; i < str.size(); i++)
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
        // if (type_of_num1 == "cm" && type_of_num2 == "km")
        // {
        //     cout << "to conver from " << type_of_num2 << " to " << type_of_num1 << " multiply by " << unit_map[type_of_num2][type_of_num1] << endl;
        // }
        return unit_map[type_of_num2][type_of_num1];
    }
    void create_conversion(const string &unit_right, double val_right, const string &unit_left, double val_left)
    {

        /*enter to the lines*/
        unit_map[unit_right][unit_left] = val_left / val_right;
        unit_map[unit_left][unit_right] = val_right / val_left;
        // cout << unit_left << " = " << unit_map[unit_left][unit_right] << " " << unit_right << endl;

        /*getting the value for converting from one unit to second unit*/
        double from_left_to_right = unit_map[unit_left][unit_right];
        double from_right_to_left = unit_map[unit_right][unit_left];
        /*making every neighbor of unit_right a neighbor of left_unit*/
        for (const auto &kv : unit_map[unit_right])
        {
            const auto &key = kv.first;
            const auto &val = kv.second;
            unit_map[unit_left][key] = val * from_left_to_right;
            unit_map[key][unit_left] = ONE / unit_map[unit_left][key];
        }
        /*making every neighbor of unit_left a neighbor of right*/
        for (const auto &kv : unit_map[unit_left])
        {
            //
            const auto &key = kv.first;
            const auto &val = kv.second;
            unit_map[unit_right][key] = (val * from_right_to_left);
            unit_map[key][unit_right] = ONE / unit_map[unit_right][key];
        }
    }

    std::vector<double> get_doubles_after_conversion(NumberWithUnits num1, NumberWithUnits num2)
    {
        std::vector<double> arr;
        string type_of_num1 = num1.get_type();
        string type_of_num2 = num2.get_type();
        double conversion_skalar = get_conversion(type_of_num1, type_of_num2);
        double value_of_num1 = num1.get_value();
        double value_of_num2 = num2.get_value();
        value_of_num2 *= conversion_skalar;

        arr.push_back(value_of_num1);
        arr.push_back(value_of_num2);
        return arr;
    }

    void NumberWithUnits::read_units(ifstream &file)
    {
        double val_1 = ZERO;
        double val_2 = ZERO;
        string unit_1;
        string unit_2;
        string equals;

        while (file >> val_1 >> unit_1 >> equals >> val_2 >> unit_2)
        {
            create_conversion(unit_1, val_1, unit_2, val_2);
        }
    }

    NumberWithUnits operator+(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values = get_doubles_after_conversion(num1, num2);
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

    NumberWithUnits operator-(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {

        std::vector<double> values = get_doubles_after_conversion(num1, num2);
        double new_num_value = values.at(ZERO) - values.at(ONE);
        return {new_num_value, num1.type};
    }

    NumberWithUnits &NumberWithUnits::operator-=(const NumberWithUnits &num)
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

    NumberWithUnits operator*(double num1, const NumberWithUnits &num2)
    {

        return {num2.value * num1, num2.type};
    }
    NumberWithUnits operator*(const NumberWithUnits &num1, double num2)
    {
        return {num1.value * num2, num1.type};
    }

    bool operator>(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {

        std::vector<double> values = get_doubles_after_conversion(num1, num2);
        return values.at(ZERO) - values.at(ONE) > EPSILON;
    }

    bool operator>=(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values = get_doubles_after_conversion(num1, num2);
        // if (num1.type == "km" && num2.type == "cm")
        // {
        //     cout << values.at(0) << " cn compared to " << values.at(1) << " km";
        // }
        double difference = values.at(ZERO) - values.at(ONE);
        return difference >= ZERO || abs(difference) < EPSILON;
    }
    bool operator==(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values = get_doubles_after_conversion(num1, num2);
        return abs(values.at(ZERO) - values.at(ONE)) < EPSILON;
    }

    bool operator<(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values = get_doubles_after_conversion(num1, num2);
        return values.at(ONE) - values.at(ZERO) > EPSILON;
    }

    bool operator<=(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values = get_doubles_after_conversion(num1, num2);
        double difference = values.at(ONE) - values.at(ZERO);
        return difference >= ZERO;
    }
    bool operator!=(const NumberWithUnits &num1, const NumberWithUnits &num2)
    {
        std::vector<double> values = get_doubles_after_conversion(num1, num2);
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
    istream &operator>>(istream &input, NumberWithUnits &num)
    {
        double value = ZERO;
        string type;
        char ch = ']';
        input >> value >> ch;
        while (ch != ']')
        {
            if (ch != '[')
            {
                type.insert(type.end(), ch);
            }
            input >> ch;
        }
        type = remove_spaces(type);
        if (unit_map.find(type) == unit_map.end())
        {
            throw std::invalid_argument("invalid number type - it doesn't exist");
        }
        num.type = type;
        num.value = value;
        return input;
    }

    ostream &operator<<(ostream &os, const NumberWithUnits &input)
    {

        os << input.value << "[" << input.type << "]";
        return os;
    }
}
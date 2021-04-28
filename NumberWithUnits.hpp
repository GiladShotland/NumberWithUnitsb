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

using namespace std;

namespace ariel
{
    class NumberWithUnits
    {
    private:
        string type;
        double value;

    public:
        NumberWithUnits(double value, const string &t);
        static void read_units(ifstream &file);
        friend NumberWithUnits operator+(const NumberWithUnits &num1, const NumberWithUnits &num2);
        NumberWithUnits &operator+=(const NumberWithUnits &num);
        NumberWithUnits operator+() { return *this; };
        friend NumberWithUnits operator-(const NumberWithUnits &num1, const NumberWithUnits &num2);
        NumberWithUnits &operator-=(const NumberWithUnits &num);
        NumberWithUnits operator-();
        // friend NumberWithUnits &operator*(NumberWithUnits &a, double x);
        friend NumberWithUnits operator*(double num1, const NumberWithUnits &num2);
        friend NumberWithUnits operator*(const NumberWithUnits &num1, double num2);
        friend bool operator>(const NumberWithUnits &num1, const NumberWithUnits &num2);
        friend bool operator>=(const NumberWithUnits &num1, const NumberWithUnits &num2);
        friend bool operator==(const NumberWithUnits &num1, const NumberWithUnits &num2);
        friend bool operator<(const NumberWithUnits &num1, const NumberWithUnits &num2);
        friend bool operator<=(const NumberWithUnits &num1, const NumberWithUnits &num2);
        friend bool operator!=(const NumberWithUnits &num1, const NumberWithUnits &num2);
        NumberWithUnits &operator++();
        NumberWithUnits operator++(int flag);
        NumberWithUnits &operator--();
        NumberWithUnits operator--(int flag);
        friend istream &operator>>(istream &input, NumberWithUnits &num);
        friend ostream &operator<<(ostream &os, const NumberWithUnits &input);
        string get_type();
        double get_value() const;
    };
}
#pragma once
#include "imitation.h"
#include <type_traits>
#include <vector>
#include <stdarg.h>
#include <string>
#include <random>
#include <ctime>
#include <sstream>
#include <iomanip>
namespace Imitation {
    class Point {
    public:
        Point(std::initializer_list<double> coords);
        Point(std::vector<double> v);
        std::vector<double>& get_coordinates();
        std::string to_string() const;
    private:
        std::vector<double> coordinates;
    };
    Point generate_random_point(std::vector<double> min_coords, std::vector<double> max_coords);
    bool generate_flip ();
    std::vector<bool> generate_flip(size_t n);
    double generate_double(double g_min, double g_max);
    std::vector<double> generate_double(size_t n, double g_min, double g_max);
    int generate_integer(int g_min, int g_max);
    std::vector<int> generate_integer(size_t n, int g_min, int g_max);
    class Circle {
    public:
        Circle(double r);
        Circle(double r, double x0, double y0);
        bool is_point_in_circle(Point p);
    private:
        double r;
        double x0;
        double y0;
    };
    std::vector<Point> generate_random_point(size_t n, const std::vector<double>& min_coords, const std::vector<double>& max_coords, std::mt19937& generator);
    class Table {
    public:
        Table(std::vector<std::vector<double>> cells, std::vector<std::string> cols_names, std::vector<std::string> rows_names, uint8_t spaces, uint8_t precision);
        std::string to_string() const;
    private:
        int rows;
        int cols;
        uint8_t spaces;
        uint8_t precision;
        uint8_t max_row_size;
        std::vector<std::vector<double>> cells;
        std::vector<std::string> cols_names;
        std::vector<std::string> rows_names;
    };
    int digits(int num);
    double mean(std::vector<double> numbers);
    std::ostream& operator << (std::ostream &os, const Imitation::Table &table);
    std::ostream& operator << (std::ostream &os, const Imitation::Point &point);
    class Rhombus {
    public:
        Rhombus(double x_left, double x_right, double x_top, double x_bot, double y_left, double y_right, double y_top, double y_bot);
        bool isPointInRhombus(Point p);
    private:
        double x_left;
        double x_right;
        double x_top;
        double x_bot;
        double y_left;
        double y_right;
        double y_top;
        double y_bot;
    };
}

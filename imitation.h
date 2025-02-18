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
        std::vector<std::vector<double>> get_numbers();
        std::vector<std::string> get_rows_names();
        std::vector<std::string> get_cols_names();
        void makefile_by_table(std::string file_name, double square);
        void pythonize();
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
std::ostream& operator << (std::ostream &os, const Imitation::Table &table);
std::ostream& operator << (std::ostream &os, const Imitation::Point &point);

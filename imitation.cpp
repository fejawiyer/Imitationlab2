#include "imitation.h"
#include <type_traits>
#include <vector>
#include <stdarg.h>
#include <string>
#include <random>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
namespace Imitation {
    Point::Point(std::initializer_list<double> coords) {
        for (auto coord : coords) {
            this->coordinates.push_back(coord);
        }
    }
    Point::Point(std::vector<double> v) {
        this->coordinates = v;
    }
    std::vector<double>& Point::get_coordinates() {
        return this->coordinates;
    }
    std::string Point::to_string() const{
        std::string res = "{";
        for (auto coord: coordinates) {
            res.append(std::to_string(coord));
            res.append(", ");
        }
        res.pop_back();
        res.pop_back();
        res.append("}");
        return res;
    }
    Point generate_random_point(std::vector<double> min_coords, std::vector<double> max_coords) {
        static std::mt19937 generator(static_cast<unsigned int>(time(0)));
        std::vector<double> coords;
        for (size_t i = 0; i < min_coords.size(); i++) {
            std::uniform_real_distribution<double> distribution(min_coords[i], max_coords[i]);
            coords.push_back(distribution(generator));
        }
        return Imitation::Point{coords};
    }
    std::vector<Point> generate_random_point(size_t n, const std::vector<double>& min_coords, const std::vector<double>& max_coords, std::mt19937& generator) {
        std::vector<std::uniform_real_distribution<double>> distributions;
        std::vector<Point> res;

        for (size_t i = 0; i < min_coords.size(); i++) {
            distributions.emplace_back(min_coords[i], max_coords[i]);
        }
        for (;n>0;n--) {
            std::vector<double> coords;
            for(auto& distribution : distributions) {
                coords.push_back(distribution(generator));
            }
            res.push_back(coords);
        }
        return res;
    }
    bool generate_flip () {
        static std::mt19937 generator(static_cast<unsigned int>(time(0)));
        std::uniform_real_distribution<double> distribution(0, 1);
        bool res;
        if (distribution(generator) <= 0.5) res = false;
        else res = true;
        return res;
    }
    std::vector<bool> generate_flip(size_t n) {
        std::vector<bool> res;
        for(;n>0;n--) {
            res.push_back(generate_flip());
        }
        return res;
    }
    double generate_double(double g_min, double g_max) {
        static std::mt19937 generator(static_cast<unsigned int>(time(0)));
        std::uniform_real_distribution<double> distribution(g_min, g_max);
        return distribution(generator);
    }
    std::vector<double> generate_double(size_t n, double g_min, double g_max) {
        std::vector<double> res;
        for(;n>0;n--) {
            res.push_back(generate_double(g_min, g_max));
        }
        return res;
    }
    int generate_integer(int g_min, int g_max) {
        static std::mt19937 generator(static_cast<unsigned int>(time(0)));
        std::uniform_real_distribution<double> distribution(g_min, g_max);
        return static_cast<int>(distribution(generator));
    }
    std::vector<int> generate_integer(size_t n, int g_min, int g_max) {
        std::vector<int> res;
        for(;n>0;n--) {
            res.push_back(generate_double(g_min, g_max));
        }
        return res;
    }
    Circle::Circle(double r) {
        this->r = r;
        this->x0=0;
        this->y0=0;
    }
    Circle::Circle(double r, double x0, double y0) {
        this->r = r;
        this->x0 = x0;
        this->y0 = y0;
    }
    bool Circle::is_point_in_circle(Point p) {
        if (p.get_coordinates().size() != 2) return false;
        std::vector<double> coords = p.get_coordinates();
        double x1 = coords[0];
        double x2 = coords[1];
        return (x1-this->x0)*(x1-this->x0) + (x2-this->y0)*(x2-this->y0) <= r*r;
    }
    Table::Table(std::vector<std::vector<double>> cells, std::vector<std::string> cols_names, std::vector<std::string> rows_names, uint8_t spaces, uint8_t precision) {
        this->cells = cells;
        this->rows = rows_names.size();
        this->cols = cols_names.size();
        this->cols_names = cols_names;
        this->rows_names = rows_names;
        this->spaces = spaces;
        this->precision = precision;
        this->max_row_size = 1;
        for(std::string row: rows_names) {
            if (row.size() > this->max_row_size) this->max_row_size = row.size();
        }
    }
    std::string Table::to_string() const{
        uint8_t string_size;
        size_t head;
        if (cols_names[0].size() > spaces) {
            head = cols_names[0].size();
        }
        else head = spaces;
        std::stringstream stream;
        std::string res = std::string(max_row_size+1, ' ');
        res.append(cols_names[0]);
        for(int i = 1; i<cols; i++) {
            res.append(std::string(spaces, ' '));
            res.append(cols_names[i]);
        }
        res.append("\n");
        for(int i = 0; i<rows; i++) {
            res.append(rows_names[i]);
            res.append(" ");
            if (max_row_size > rows_names[i].size()) res.append(std::string(max_row_size-rows_names[i].size(), ' '));
            for (int j = 0; j<cols; j++) {
                stream << std::fixed << std::setprecision(precision) << cells[i][j];
                string_size = digits(static_cast<int>(cells[i][j]))+ 1 + precision;
                res.append(stream.str());
                stream.str("");
                res.append(std::string(head-string_size+spaces, ' '));
            }
            res.append("\n");
        }
        return res;
    }
    int digits(int num) {
        int res = 0;
        do {
            num /= 10;
            res++;
        } while(num);
        return res;
    }
    double mean(std::vector<double>numbers) {
        double res = 0;
        for(double j: numbers) {
            res+=j;
        }
        return res/numbers.size();
    }
    Rhombus::Rhombus(double x_left, double x_right, double x_top, double x_bot, double y_left, double y_right, double y_top, double y_bot) {
        this->x_left = x_left;
        this->x_right = x_right;
        this->x_bot = x_bot;
        this->x_top = x_top;
        this->y_left = y_left;
        this->y_right = y_right;
        this->y_bot = y_bot;
        this->y_top = y_top;
    }
    bool Rhombus::isPointInRhombus(Point p) {
        if (p.get_coordinates().size() != 2) return false;
        double d1 = (x_top - x_left) * (p.get_coordinates()[1] - y_left) - (y_top - y_left) * (p.get_coordinates()[0] - x_left);
        double d2 = (x_right - x_top) * (p.get_coordinates()[1] - y_top) - (y_right - y_top) * (p.get_coordinates()[0] - x_top);
        double d3 = (x_bot - x_right) * (p.get_coordinates()[1] - y_right) - (y_bot - y_right) * (p.get_coordinates()[0] - x_right);
        double d4 = (x_left - x_bot) * (p.get_coordinates()[1] - y_bot) - (y_left - y_bot) * (p.get_coordinates()[0] - x_bot);
        // Если все произведения имеют одинаковый знак, точка находится внутри ромба
        return (d1 >= 0 && d2 >= 0 && d3 >= 0 && d4 >= 0) || (d1 <= 0 && d2 <= 0 && d3 <= 0 && d4 <= 0);
    }
}
std::ostream& operator << (std::ostream &os, const Imitation::Point &point)
{
    return os << point.to_string();
}
std::ostream& operator << (std::ostream &os, const Imitation::Table &table)
{
    return os << table.to_string();
}



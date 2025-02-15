#include <iostream>
#include <string>
#include <random>
#include <ctime>
#include <vector>
#include <iomanip>
#include <cmath>
#include <numeric>
#include "imitation.h"

int main()
{
    setlocale(LC_ALL, "Russian"); // ��������� ���������, �������������� ������� ����
    size_t progonsCount, points, imitationCount, pointsInCircle = 0; // �������� ����������
    double radius, x0, y0; // �������� ���������� � ��������� ������� ��� ������

    std::random_device rd;
    std::mt19937 generator(rd());
    // ���� �������� � ����������
    std::cout << "������� ������ �����:";
    std::cin >> radius;
    std::cout << "������� ���������� ������ ����� (x):";
    std::cin >> x0;
    std::cout << "������� ���������� ������ ����� (y):";
    std::cin >> y0;
    std::cout << "������� ������ �������:";
    std::cin >> points;
    std::cout << "������� ���������� ��������:";
    std::cin >> imitationCount;
    std::cout << "������� ���������� ��������:";
    std::cin >> progonsCount;

    // ���������� ������� ��������
    double squareArea = 4*radius*radius; // ������� �������� - ��������� ������������ �������. ������� ����� 2*radius, ����� 2*radius*2*radius=4*radius^2 - ������� ��������

    Imitation::Circle circle = {radius, x0, y0}; // �������� �����

    std::cout << "������� �����(������������):" << std::acos(-1)*radius*radius << std::endl;

    std::vector<std::vector<double>> table(progonsCount+2, std::vector<double>(imitationCount)); // �������� ������� ��� �������� ��������
    // ������ 5 ����� - �������, 6 ������ - ���������� ������� �� ��������, 7 ������ - ���������


    std::vector<Imitation::Point> points_vector;
    points_vector.reserve(points);
    std::vector<double> min_coords = {x0-radius, y0-radius};
    std::vector<double> max_coords = {x0+radius, y0+radius};

    double square;
    std::vector<std::string>cols_names;
    std::vector<std::string>rows_names;
    for(size_t i = 0; i<progonsCount; i++) {
        points_vector.clear();
        rows_names.push_back("Progon " + std::to_string(i+1));
        for(size_t j = 0; j<imitationCount; j++) {
            points_vector = Imitation::generate_random_point((j+1)*points, min_coords, max_coords, generator);
            for (Imitation::Point &k : points_vector) {
                if(circle.is_point_in_circle(k)) {
                    pointsInCircle++;
                }
            }
            square = (static_cast<double>(pointsInCircle)/points_vector.size()) * squareArea;
            table[i][j] = square;
            pointsInCircle = 0;
        }
    }
    for(size_t i = 0; i<imitationCount; i++) {
        cols_names.push_back("Imitation " + std::to_string(i+1));
    }
    rows_names.push_back("Average");
    rows_names.push_back("St. error");

    // ������ ���� ��������� � table ���������� �������
    for(size_t j = 0; j<imitationCount; j++) {
        double res = 0;
        for(size_t k = 0; k<progonsCount; k++) {
            res += table[k][j]; // ����� ��������� �� �������
        }
        table[progonsCount][j] = res/progonsCount; // ������ �� ���������� ��������, �������� �������
        res = 0; // ��������� ���������� ��� ��������� ��������
    }
    // ������ ���� ������� ��. ����.
    for(size_t j = 0; j<imitationCount; j++) {
        std::vector<double> tmp; // ������ ��� ���������� �������� ������
        for(size_t k = 0; k<progonsCount; k++) {
            tmp.push_back(table[k][j]); // ������ ������ �� ���������
        }
        double m = Imitation::mean(tmp); // ��������� �������

        double first_sum = 0; // �����

        for(size_t k = 0; k<progonsCount; k++) {
            first_sum += (tmp[k]-m)*(tmp[k]-m);
        }
        double standard_dif = std::sqrt(first_sum/(double)progonsCount); // ����������� ���������� (�� �������)
        table[progonsCount+1][j] = standard_dif; // ��������� ��������� ������� � �������
        tmp.clear(); // �������� ������
    }

    Imitation::Table t(table, cols_names, rows_names, 3, 3);
    std::cout << t.to_string() << std::endl;

    Imitation::Rhombus rhombus = {1, 2, 1.5, 1.5, 1.5, 1.5, 2, 1}; // ������ ����


    // ��������� ���� �������� �������, ������� �� ������������
    for(auto& row: table) {
        for(auto& element: row) {
            element = 0.0f;
        }
    }

    size_t pointsInRhombus = 0; // � ���� ���������� ����� ������� ���������� ����� � �����

    std::vector<double> min_coords_for_rhombus = {1, 1};
    std::vector<double> max_coords_for_rhombus = {2, 2};

    // ������ ��� ����������� ����������� �������� ��� �����
    for(size_t progons = 0; progons < progonsCount; progons++) {
        for(size_t i = 0; i < imitationCount; i++) {
            points_vector = Imitation::generate_random_point((i+1)*points, min_coords_for_rhombus, max_coords_for_rhombus, generator);
            for(Imitation::Point &j : points_vector) {
                if(rhombus.isPointInRhombus(j)) {
                    pointsInRhombus++;
                }
            }
            float square = (static_cast<float>(pointsInRhombus)/points_vector.size()) * 1;
            table[progons][i] = square;
            pointsInRhombus = 0;
        }
    }

    for(size_t j = 0; j<imitationCount; j++) {
        double res = 0;
        for(size_t k = 0; k<progonsCount; k++) {
            res += table[k][j];
        }
        table[progonsCount][j] = res/progonsCount;
        res = 0;
    }

    for(size_t j = 0; j<imitationCount; j++) {
        std::vector<double> tmp;
        for(size_t k = 0; k<progonsCount; k++) {
            tmp.push_back(table[k][j]);
        }
        double m = Imitation::mean(tmp);

        double first_sum = 0;

        for(size_t k = 0; k<progonsCount; k++) {
            first_sum += (tmp[k]-m)*(tmp[k]-m);
        }
        double standard_dif = std::sqrt(first_sum/(float)progonsCount);
        table[progonsCount+1][j] = standard_dif;
        tmp.clear();
    }
    Imitation::Table t2(table, cols_names, rows_names, 3, 3);
    std::cout << "������� �����(������������):0.5" << std::endl;
    std::cout << t2.to_string() << std::endl;
}

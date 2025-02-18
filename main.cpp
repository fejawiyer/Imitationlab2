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
    setlocale(LC_ALL, "Russian"); // Установка кодировки, поддерживающий русский язык
    size_t progonsCount, points, imitationCount, pointsInCircle = 0; // создание переменных
    double radius, x0, y0; // создание переменных с плавающей запятой для центра

    std::random_device rd;
    std::mt19937 generator(rd()); // ГПСЧ (Вихрь Мерсенна)
    // Данный генератор работает качественее и быстрее чем встроенный rand()
    // Ввод значений с клавиатуры
    std::cout << "Введите радиус круга:";
    std::cin >> radius;
    std::cout << "Введите координаты центра круга (x):";
    std::cin >> x0;
    std::cout << "Введите координаты центра круга (y):";
    std::cin >> y0;
    std::cout << "Введите размер выборки:";
    std::cin >> points;
    std::cout << "Введите количество имитаций:";
    std::cin >> imitationCount;
    std::cout << "Введите количество прогонов:";
    std::cin >> progonsCount;

    // Вычисление площади квадрата
    double squareArea = 4*radius*radius; // Площадь квадрата - удвоенное произведение стороны. Сторона равна 2*radius, тогда 2*radius*2*radius=4*radius^2 - площадь квадрата

    Imitation::Circle circle = {radius, x0, y0}; // Создание круга

    std::cout << "Площадь круга(аналитически):" << std::acos(-1)*radius*radius << std::endl;

    std::vector<std::vector<double>> table(progonsCount+2, std::vector<double>(imitationCount)); // Создание матрицы для хранения значения
    // Первые 5 строк - прогоны, 6 строка - выборочное среднее по прогонам, 7 строка - дисперсия


    std::vector<Imitation::Point> points_vector; // вектор точек
    points_vector.reserve(points); // резервируем память
    std::vector<double> min_coords = {x0-radius, y0-radius}; // минимальные координаты (левый нижний угод квадрата)
    std::vector<double> max_coords = {x0+radius, y0+radius}; // максимальные координаты (правый верхний угол квадрата)

    double square; // площадь

    std::vector<std::string>cols_names; // вектор имен столбцов
    std::vector<std::string>rows_names; // вектор имен строк

    for(size_t i = 0; i<progonsCount; i++) {
        points_vector.clear(); // очищаем вектор
        rows_names.push_back("Progon " + std::to_string(i+1)); // добавляем в вектор строк строки вида Progon1, Progon2, ...
        for(size_t j = 0; j<imitationCount; j++) {
            points_vector = Imitation::generate_random_point((j+1)*points, min_coords, max_coords, generator); // Создаем (j+1)*points точек с минимальными координатами
            for (Imitation::Point &k : points_vector) { // Для каждой точки из вектора
                if(circle.is_point_in_circle(k)) { // Если она лежит в круге
                    pointsInCircle++; // Увеличить переменную pointsInCircle на 1
                }
            }
            square = (static_cast<double>(pointsInCircle)/points_vector.size()) * squareArea; // Площадь
            // Площадь считается так: кол-во точек в круге делится на размер вектора и умножается на площадь квадрата
            table[i][j] = square; // Результат заносится в таблицу
            pointsInCircle = 0; // Обнуляем счетчик
        }
    }
    for(size_t i = 0; i<imitationCount; i++) {
        cols_names.push_back("Imitation " + std::to_string(i+1)); // В данном цикле заполняются названия столбцов вида Imitation 1, Imitation 2, ...
    }
    rows_names.push_back("Average"); // В строки добавляется Average
    rows_names.push_back("St. error"); // Также добавляется St. Error

    // Данный цикл добавляет в table выборочное среднее
    for(size_t j = 0; j<imitationCount; j++) {
        double res = 0;
        for(size_t k = 0; k<progonsCount; k++) {
            res += table[k][j]; // сумма элементов по прогону
        }
        table[progonsCount][j] = res/progonsCount; // делить на количество прогонов, получать среднее
        res = 0; // обнуление результата для следующей имитации
    }
    // Данный цикл считает ст. откл.
    for(size_t j = 0; j<imitationCount; j++) {
        std::vector<double> tmp; // вектор для временного хранения данных
        for(size_t k = 0; k<progonsCount; k++) {
            tmp.push_back(table[k][j]); // меняем строки со столбцами
        }
        double m = Imitation::mean(tmp); // вычисляем среднее

        double first_sum = 0; // сумма

        for(size_t k = 0; k<progonsCount; k++) {
            first_sum += (tmp[k]-m)*(tmp[k]-m);
        }
        double standard_dif = std::sqrt(first_sum/(double)progonsCount); // стандартное отклонение (по формуле)
        table[progonsCount+1][j] = standard_dif; // добавляем последней строкой в таблицу
        tmp.clear(); // зануляем вектор
    }

    Imitation::Table t(table, cols_names, rows_names, 3, 3); // Создание таблицы с отступами в 3 пробела и точностью 3
    std::cout << t << std::endl; // Вывод таблицы с помощью перегруженного оператора <<

    Imitation::Rhombus rhombus = {1, 2, 1.5, 1.5, 1.5, 1.5, 2, 1}; // Создаём ромб


    // Следующий цикл обнуляет таблицу, которую мы использовали
    for(auto& row: table) {
        for(auto& element: row) {
            element = 0.0f;
        }
    }

    size_t pointsInRhombus = 0; // В этой переменной будем считать количество точек в ромбе

    std::vector<double> min_coords_for_rhombus = {1, 1};
    std::vector<double> max_coords_for_rhombus = {2, 2};

    // Данный код проделывает аналогичные операции для ромба
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
    std::cout << "Площадь ромба(аналитически):0.5" << std::endl;
    std::cout << t2 << std::endl;

    t2.makefile_by_table("table_1.cfg", 0.5); // Создаётся файл содержащий имена столбцов, строк, ячейки.
    t.makefile_by_table("table_2.cfg", std::acos(-1)*radius*radius);

    if (system("where python >nul 2>nul") != 0) { // Если не установлен Python
        std::cout << "Не удалось создать файл excel:";
        std::cout << "Python не найден в PATH." << std::endl;
        return 0;
    }
    if(system("python -c \"import pandas\"") != 0) { // Библиотека pandas
        std::cout << "Не удалось создать файл excel:";
        std::cout << "Не найдена библиотека pandas." << std::endl;
        return 0;
    }
    if(system("python -c \"import openpyxl\"") != 0) { // Библиотека openpyxl
        std::cout << "Не удалось создать файл excel:";
        std::cout << "Не найдена библиотека openpyxl." << std::endl;
        return 0;
    }
    t2.pythonize(); // Выполняется скрипт Python, создающий excel файл на основе двух таблиц.
}

// postProcessing.h
#ifndef POST_PROCESSING_H
#define POST_PROCESSING_H

#include <iostream>
#include <vector>
#include <cmath>
#include <tuple> // Include to retun different data types
#include <iomanip>


////////--------Creating linspace for c++
std::vector<double> linspace(double start, double end, std::size_t numPoints) {
    std::vector<double> result;
    
    if (numPoints <= 1) {
        result.push_back(start);
        return result;
    }

    double step = (end - start) / static_cast<double>(numPoints - 1);

    for (std::size_t i = 0; i < numPoints; ++i) {
        result.push_back(start + i * step);
    }

    return result;
}//--------Creating linspace for c++ END


std::tuple<std::vector<double>, std::vector<double>> scaleVectors(const std::vector<double>& y) {
    std::vector<double> yd, yu;

    // Reserve space for the result vectors to avoid reallocation
    yd.reserve(y.size());
    yu.reserve(y.size());

    // Perform scaling for each element in y
    for (const auto& element : y) {
        yd.push_back(element - 0.01);
        yu.push_back(element + 0.01);
    }

    return std::make_tuple(yd, yu);
}

#endif // POST_PROCESSING_H
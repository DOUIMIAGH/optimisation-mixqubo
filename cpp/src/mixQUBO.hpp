#ifndef __mixQUBO__hpp
#define __mixQUBO__hpp

#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "rapidjson/document.h"
#include "rapidjson/istreamwrapper.h"

struct Term {
    int u, v, i, j;
    double c0, c1, m, J, alpha, K, A;
};

class MixQUBO {
public:
    // dimension of continuous space
    int d;

    // dimension of binary space
    int n;

    // Terms of the expansion
    std::vector<Term> terms;

    MixQUBO(int d = 0, int n = 0) : d(d), n(n) {}

    void addTerm(int u, int v, int i, int j, std::vector<double> & c, double m, double J) {
        double d2_max = 0;

        if (c[0] >= 0)
            d2_max = std::pow(-1 - c[0], 2);
        else
            d2_max = std::pow(1 - c[0], 2);

        if (c[1] >= 0)
            d2_max += std::pow(-1 - c[1], 2);
        else
            d2_max += std::pow(1 - c[1], 2);

        double alpha = -std::log(m) / d2_max;
        double K = (1 + m) / 2;
        double A = 2 / (1 - m);

        Term term;
        term.u = u;
        term.v = v;
        term.i = i;
        term.j = j;
        term.c0 = c[0];
        term.c1 = c[1];
        term.m = m;
        term.J = J;
        term.alpha = alpha;
        term.K = K;
        term.A = A;

        terms.push_back(term);
    }

    double h(int id, Solution & solution) const {
        Term t = terms[id];

        double kernel = t.J * t.A *
            (std::exp(-t.alpha *
                (std::pow(solution.z[t.u] - t.c0, 2) + std::pow(solution.z[t.v] - t.c1, 2))) - t.K);

        if (solution.x[t.i] == solution.x[t.j]) {
            return kernel;
        } else {
            return -kernel;
        }
    }

    void eval(Solution & solution) const {
        double f = 0;

        for (size_t i = 0; i < terms.size(); ++i)
            f += h(i, solution);

        solution.f = f;
    }

    void from_json(const std::string& json_string) {
        rapidjson::Document document;
        document.Parse(json_string.c_str());

        if (document.HasParseError()) {
            std::cerr << "Error parsing JSON." << std::endl;
            return;
        }

        if (document.HasMember("problem")) {
            const rapidjson::Value& problem = document["problem"];

            if (problem.HasMember("d")) {
                d = problem["d"].GetInt();
            }

            if (problem.HasMember("n")) {
                n = problem["n"].GetInt();
            }

            if (problem.HasMember("terms") && problem["terms"].IsArray()) {
                const rapidjson::Value& terms_array = problem["terms"];
                terms.clear();

                for (rapidjson::SizeType i = 0; i < terms_array.Size(); i++) {
                    const rapidjson::Value& term_obj = terms_array[i];
                    Term term;

                    term.u = term_obj["u"].GetInt();
                    term.v = term_obj["v"].GetInt();
                    term.i = term_obj["i"].GetInt();
                    term.j = term_obj["j"].GetInt();

                    const rapidjson::Value& c_array = term_obj["c"];
                    term.c0 = c_array[0].GetDouble();
                    term.c1 = c_array[1].GetDouble();

                    term.m = term_obj["m"].GetDouble();
                    term.J = term_obj["J"].GetDouble();
                    term.alpha = term_obj["alpha"].GetDouble();
                    term.K = term_obj["K"].GetDouble();
                    term.A = term_obj["A"].GetDouble();

                    terms.push_back(term);
                }
            }
        }
    }

    void read_json(const std::string& file_name) {
        std::ifstream json_file(file_name);

        if (!json_file) {
            std::cerr << "Impossible to open the file: " << file_name << std::endl;
            exit(1);
        }

        std::string json_string((std::istreambuf_iterator<char>(json_file)), std::istreambuf_iterator<char>());
        from_json(json_string);
        json_file.close();
    }

    std::string to_json(std::string info = "{}") const {
        std::ostringstream oss;
        oss << "{ \"problem\": { \"info\": " << info << ", \"d\": " << d << ", \"n\": " << n << ", \"terms\": [";

        for (size_t i = 0; i < terms.size(); ++i) {
            Term term = terms[i];
            oss << "{";
            oss << "\"u\":" << term.u << ", \"v\":" << term.v << ", \"i\":" << term.i << ", \"j\":" << term.j << ", ";
            oss << "\"c\":[" << term.c0 << ", " << term.c1 << "], ";
            oss << "\"m\":" << term.m << ", \"J\":" << term.J << ", ";
            oss << "\"alpha\":" << term.alpha << ", \"K\":" << term.K << ", \"A\":" << term.A;
            oss << "}";
            if (i != terms.size() - 1) oss << ", ";
        }

        oss << "]}}";
        return oss.str();
    }

    void write_json(const std::string& file_name, std::string info = "{}") const {
        std::ofstream json_file(file_name);
        json_file << to_json(info);
        json_file.close();
    }

};

#endif

#include <algorithm>
#include <fstream>
#include <iostream>
#include <limits>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

struct TestCase {
    int n;
    int capacity;
    vector<pair<int, int>> items; // weight, value
};

vector<TestCase> readCases(const string& filename) {
    ifstream input(filename);
    if (!input) throw runtime_error("Could not open input file: " + filename);

    vector<TestCase> cases;
    vector<vector<int>> rows;
    string line;
    auto saveCase = [&]() {
        if (rows.empty()) return;
        if (rows[0].size() != 2) throw runtime_error("Expected n and capacity after #Test");
        TestCase test{rows[0][0], rows[0][1], {}};
        for (size_t i = 1; i < rows.size(); ++i) {
            if (rows[i].size() != 2) throw runtime_error("Each item must have weight and value");
            test.items.push_back({rows[i][0], rows[i][1]});
        }
        if (test.n != static_cast<int>(test.items.size()))
            throw runtime_error("Item count does not match n");
        cases.push_back(test);
        rows.clear();
    };

    while (getline(input, line)) {
        istringstream parser(line);
        string first;
        if (!(parser >> first)) continue;
        if (first.rfind("#Test", 0) == 0) {
            saveCase();
            continue;
        }
        vector<int> values;
        try {
            values.push_back(stoi(first));
            int value;
            while (parser >> value) values.push_back(value);
        } catch (...) {
            throw runtime_error("Invalid number in input: " + line);
        }
        rows.push_back(values);
    }
    saveCase();
    return cases;
}

int solve(const TestCase& test, bool visualize) {
    vector<vector<int>> dp(test.n + 1, vector<int>(test.capacity + 1, 0));
    for (int i = 1; i <= test.n; ++i) {
        auto [weight, value] = test.items[i - 1];
        for (int w = 0; w <= test.capacity; ++w) {
            dp[i][w] = dp[i - 1][w];
            if (weight <= w)
                dp[i][w] = max(dp[i][w], dp[i - 1][w - weight] + value);
        }

        if (visualize) {
            cout << "\nAfter item " << i << " (weight=" << weight
                 << ", value=" << value << ")\n     w |";
            for (int w = 0; w <= test.capacity; ++w) cout << ' ' << w;
            cout << "\n-------+" << string(test.capacity * 3 + 3, '-') << '\n';
            for (int row = 0; row <= i; ++row) {
                cout << "     " << row << " |";
                for (int w = 0; w <= test.capacity; ++w) cout << ' ' << dp[row][w];
                cout << '\n';
            }
            cout << "Press Enter for next item...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }
    }
    return dp[test.n][test.capacity];
}

int main(int argc, char* argv[]) {
    if (argc < 2 || argc > 3) {
        cerr << "Usage: knapsack.exe <input.txt> [--visualize]\n";
        return 1;
    }
    const bool visualize = argc == 3 && string(argv[2]) == "--visualize";
    if (argc == 3 && !visualize) {
        cerr << "Unknown option: " << argv[2] << '\n';
        return 1;
    }

    try {
        auto cases = readCases(argv[1]);
        ofstream output("result.txt");
        if (!output) throw runtime_error("Could not create result.txt");
        for (size_t i = 0; i < cases.size(); ++i) {
            const auto& test = cases[i];
            cout << "Test " << i + 1 << ": n=" << test.n
                 << ", capacity=" << test.capacity << '\n';
            int answer = solve(test, visualize);
            cout << "Maximum total value: " << answer << "\n\n";
            output << "#Test case input (" << test.n << ' ' << test.capacity
                   << ") test result " << answer << '\n';
        }
        cout << "Saved results to result.txt\n";
    } catch (const exception& error) {
        cerr << "Error: " << error.what() << '\n';
        return 1;
    }
    return 0;
}

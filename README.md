# 0/1 Knapsack Dynamic Programming

This project solves each `#Test` case in `Knapsack data (2).txt` using the 0/1 Knapsack dynamic programming algorithm.

## Build and run

Compile with a C++17 compiler:

```bash
g++ -std=c++17 knapsack.cpp -o knapsack
```

Run normally to calculate all answers and write them to `result.txt`:

```bash
./knapsack "Knapsack data (2).txt"
```

To display the ASCII DP table after each item:

```bash
./knapsack "Knapsack data (2).txt" --visualize
```

On Windows, run the compiled program as `knapsack.exe` instead of `./knapsack`.

## Files

- `knapsack.cpp` — solver and live ASCII table visualization.
- `Knapsack data (2).txt` — assignment test cases.
- `result.txt` — maximum total value for each test case.

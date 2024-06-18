#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <algorithm>
using namespace std;

vector<double> parseSequence(const string& input) {
    vector<double> sequence;
    stringstream ss(input);
    string token;
    
    while (getline(ss, token, ',')) {
        stringstream num(token);
        double value;
        num >> value;
        sequence.push_back(value);
    }
    return sequence;
}

double calculateDTW(const vector<double>& seq1, const vector<double>& seq2, vector<vector<double>>& dtwMatrix) {
    int n = seq1.size();
    int m = seq2.size();
    
    dtwMatrix.resize(n + 1, vector<double>(m + 1, numeric_limits<double>::infinity()));
    dtwMatrix[0][0] = 0;
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            double cost = abs(seq1[i-1] - seq2[j-1]);
            dtwMatrix[i][j] = cost + min({ dtwMatrix[i-1][j], dtwMatrix[i][j-1], dtwMatrix[i-1][j-1] });
        }
    }
    
    return dtwMatrix[n][m];
}

void printDTW(const vector<vector<double>>& dtwMatrix, const vector<double>& seq1, const vector<double>& seq2) {
    int n = seq1.size();
    int m = seq2.size();

    vector<pair<int, int>> path;
    int i = n, j = m;
    path.push_back({i, j});

    while (i > 1 || j > 1) {
        if (i == 1) {
            j--;
        } else if (j == 1) {
            i--;
        } else {
            if (dtwMatrix[i-1][j] == min({ dtwMatrix[i-1][j], dtwMatrix[i][j-1], dtwMatrix[i-1][j-1] })) {
                i--;
            } else if (dtwMatrix[i][j-1] == min({ dtwMatrix[i-1][j], dtwMatrix[i][j-1], dtwMatrix[i-1][j-1] })) {
                j--;
            } else {
                i--;
                j--;
            }
        }
        path.push_back({i, j});
    }

    reverse(path.begin(), path.end());

    cout << "       ";
    for (int j = 0; j < m; ++j) {
        cout << setw(4) << seq2[j] << " | ";
    }
    cout << "\n";

    for (int i = 0; i < n; ++i) {
        cout << setw(4) << seq1[i] << " | ";
        for (int j = 0; j < m; ++j) {
            bool isPath = find(path.begin(), path.end(), make_pair(i + 1, j + 1)) != path.end();
            if (isPath) {
                if (floor(seq1[i]) == seq1[i] && floor(seq2[j]) == seq2[j]) {
                    cout << "\033[1;31m" << setw(4) << static_cast<int>(abs(seq1[i] - seq2[j])) << "\033[0m | ";
                } else {
                    cout << "\033[1;31m" << setw(4) << fixed << setprecision(2) << abs(seq1[i] - seq2[j]) << "\033[0m | ";
                }
            } else {
                if (floor(seq1[i]) == seq1[i] && floor(seq2[j]) == seq2[j]) {
                    cout << setw(4) << static_cast<int>(abs(seq1[i] - seq2[j])) << " | ";
                } else {
                    cout << setw(4) << fixed << setprecision(2) << abs(seq1[i] - seq2[j]) << " | ";
                }
            }
        }
        cout << "\n";
    }
}

int main() {
    string input1, input2;
    
    cout << "Enter the first sequence of numbers (comma-separated): ";
    getline(cin, input1);
    vector<double> seq1 = parseSequence(input1);
    
    cout << "Enter the second sequence of numbers (comma-separated): ";
    getline(cin, input2);
    vector<double> seq2 = parseSequence(input2);
    
    vector<vector<double>> dtwMatrix;
    double dtwDistance = calculateDTW(seq1, seq2, dtwMatrix);
    
    cout << "DTW Distance: " << dtwDistance << endl;
    cout << "DTW Matrix:" << endl;
    printDTW(dtwMatrix, seq1, seq2);
    
    return 0;
}

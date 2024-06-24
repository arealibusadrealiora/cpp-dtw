#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <algorithm>
#include <tuple>
using namespace std;

//Testing GDTW. Not sure if it'll work as intended, lol.
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

tuple<double, vector<tuple<int, int, int>>> calculateGDTW(const vector<vector<double>>& sequences) {
    int n = sequences[0].size();
    int m = sequences[1].size();
    int l = sequences[2].size();
    
    vector<vector<vector<double>>> gdtwMatrix(n + 1, vector<vector<double>>(m + 1, vector<double>(l + 1, numeric_limits<double>::infinity())));
    vector<vector<vector<tuple<int, int, int>>>> pathMatrix(n + 1, vector<vector<tuple<int, int, int>>>(m + 1, vector<tuple<int, int, int>>(l + 1)));
    gdtwMatrix[0][0][0] = 0;
    
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= m; ++j) {
            for (int k = 1; k <= l; ++k) {
                double cost = abs(sequences[0][i-1] - sequences[1][j-1] - sequences[2][k-1]);
                
                tuple<int, int, int> prevIndex;
                double minPrevCost = min({
                    gdtwMatrix[i-1][j][k], 
                    gdtwMatrix[i][j-1][k], 
                    gdtwMatrix[i][j][k-1],
                    gdtwMatrix[i-1][j-1][k], 
                    gdtwMatrix[i-1][j][k-1], 
                    gdtwMatrix[i][j-1][k-1],
                    gdtwMatrix[i-1][j-1][k-1]
                });

                if (minPrevCost == gdtwMatrix[i-1][j][k]) prevIndex = make_tuple(i-1, j, k);
                else if (minPrevCost == gdtwMatrix[i][j-1][k]) prevIndex = make_tuple(i, j-1, k);
                else if (minPrevCost == gdtwMatrix[i][j][k-1]) prevIndex = make_tuple(i, j, k-1);
                else if (minPrevCost == gdtwMatrix[i-1][j-1][k]) prevIndex = make_tuple(i-1, j-1, k);
                else if (minPrevCost == gdtwMatrix[i-1][j][k-1]) prevIndex = make_tuple(i-1, j, k-1);
                else if (minPrevCost == gdtwMatrix[i][j-1][k-1]) prevIndex = make_tuple(i, j-1, k-1);
                else prevIndex = make_tuple(i-1, j-1, k-1);
                
                gdtwMatrix[i][j][k] = cost + minPrevCost;
                pathMatrix[i][j][k] = prevIndex;
            }
        }
    }
    
    vector<tuple<int, int, int>> path;
    int i = n, j = m, k = l;
    while (i > 0 || j > 0 || k > 0) {
        path.push_back(make_tuple(i, j, k));
        tie(i, j, k) = pathMatrix[i][j][k];
    }
    path.push_back(make_tuple(0, 0, 0));
    reverse(path.begin(), path.end());
    
    return make_tuple(gdtwMatrix[n][m][l], path);
}

int main() {
    int numSequences = 3;
    vector<vector<double>> sequences;
    
    for (int i = 0; i < numSequences; ++i) {
        string input;
        cout << "Enter sequence " << i + 1 << " (comma-separated): ";
        getline(cin, input);
        sequences.push_back(parseSequence(input));
    }
    
    auto result = calculateGDTW(sequences);
    double gdtwDistance = get<0>(result);
    vector<tuple<int, int, int>> path = get<1>(result);
    
    cout << "Generalized DTW Distance for 3 sequences: " << gdtwDistance << endl;
    cout << "Path:" << endl;
    for (size_t idx = 0; idx < path.size(); ++idx) {
        int i, j, k;
        tie(i, j, k) = path[idx];
        cout << "(" << i << ", " << j << ", " << k << ")" << endl;
    }
    
    return 0;
}

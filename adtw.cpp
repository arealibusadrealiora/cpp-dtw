#include <iostream>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <limits>
#include <algorithm>
using namespace std;


//This version is to calculate the avegare DTW between multiple sequences
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

int main() {
    int numSequences;
    cout << "Enter the number of sequences: ";
    cin >> numSequences;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    
    vector<vector<double>> sequences;
    for (int i = 0; i < numSequences; ++i) {
        string input;
        cout << "Enter sequence " << i + 1 << " (comma-separated): ";
        getline(cin, input);
        sequences.push_back(parseSequence(input));
    }
    
    double totalDTW = 0.0;
    int pairCount = 0;
    
    for (int i = 0; i < numSequences; ++i) {
        for (int j = i + 1; j < numSequences; ++j) {
            vector<vector<double>> dtwMatrix;
            double dtwDistance = calculateDTW(sequences[i], sequences[j], dtwMatrix);
            totalDTW += dtwDistance;
            pairCount++;
        }
    }
    
    double averageDTW = (pairCount > 0) ? totalDTW / pairCount : 0.0;
    cout << "Average DTW Distance: " << averageDTW << endl;
    
    return 0;
}

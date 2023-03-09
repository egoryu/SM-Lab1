#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cmath>

using namespace std;

void outputMatrix(vector<vector<double>> matrix, vector<double> coef) {
    for (int i = 0; i < matrix.size(); i++) {
        for (int j = 0; j < matrix.size(); j++) {
            cout << matrix[i][j] << ' ';
        }
        cout << coef[i] << endl;
    }
}

void outputAnswer(vector<vector<double>> ans) {
    cout << "Error vector output:" << endl;

    for (int i = 1; i < ans.size(); i++) {
        for (int j = 0; j < ans[i].size(); j++) {
            cout << fabs(ans[i][j] - ans[i - 1][j]) << ' ';
        }
        cout << endl;
    }

    cout << "Steps: " << ans.size() - 1 << endl;
    cout << "Output of the vector of unknowns:" << endl;

    for (auto u : ans[ans.size() - 1]) {
        cout << u << " ";
    }
    cout << endl;

}

void outputError(vector<vector<double>> ans, vector<vector<double>> matrix, vector<double> coef) {
    for (int i = 0; i < matrix.size(); i++) {
        double t = 0;
        for (int j = 0; j < matrix.size(); j++) {
            t += matrix[i][j] * ans[ans.size() - 1][j];
        }
        cout << fabs(t - coef[i]) << " ";
    }
    cout << endl;
}

bool diagMax(vector<vector<double>> matrix) {
    bool flag = true;

    for (int i = 0; i < matrix.size(); i++) {
        double x = 0;
        for (int j = 0; j < matrix[i].size(); j++) {
            if (i != j)
                x += fabs(matrix[i][j]);
        }
        if (fabs(matrix[i][i]) < x)
            flag = false;
    }

    return flag;
}

vector<vector<double>> columnSwap(vector<vector<double>> matrix, int x, int y) {
    for (int i = 0; i < matrix.size(); i++) {
        double t = matrix[i][x];

        matrix[i][x] = matrix[i][y];
        matrix[i][y] = t;
    }

    return matrix;
}

vector<vector<double>> dioganling(vector<vector<double>> matrix) {
    for (int i = 0; i < matrix.size(); i++) {
        int cur = i;

        for (int j = 0; j < matrix.size(); j++) {
            if (fabs(matrix[i][cur]) < fabs(matrix[i][j])) {
                cur = j;
            }
        }
        if (cur != i) {
            matrix = columnSwap(matrix, cur, i);
        }
    }

    return matrix;
}

vector<vector<double>> iteration(vector<vector<double>> matrix, vector<double> coef, double epselon) {
    for (int i = 0; i < matrix.size(); i++) {
        double t = matrix[i][i];

        coef[i] /= t;
        for (int j = 0; j < matrix.size(); j++) {
            matrix[i][j] /= -t;
        }

        matrix[i][i] = 0;
    }

    vector<vector<double>> ans;

    ans.push_back(coef);

    while (1) {
        ans.push_back(coef);

        for (int i = 0; i < matrix.size(); i++) {   
            for (int j = 0; j < matrix.size(); j++) {
                ans[ans.size() - 1][i] += matrix[i][j] * ans[ans.size() - 2][j];
            }
        }

        double delta = -INFINITY;

        for (int i = 0; i < matrix.size(); i++) {
            delta = max(delta, fabs(ans[ans.size() - 1][i] - ans[ans.size() - 2][i]));
        }

        if (delta < epselon || ans.size() > 2000) {
            break;
        }
    }

    return ans;
}

void execute(vector<vector<double>> matrix, vector<double> coef, double epselon) {
    cout << "Read conditions:" << endl;
    outputMatrix(matrix, coef);
    cout << epselon << endl;

    if (diagMax(matrix)) {
        cout << "Matrix has diagonal dominance" << endl;
    }
    else {
        cout << "Matrix hasn't diagonal dominance" << endl;
        matrix = dioganling(matrix);
        if (!diagMax(matrix)) {
            cout << "It wasn't possible to bring the matrix to diagonal dominance" << endl;
        }
        else {
            cout << "It was possible to bring the matrix to diagonal dominance" << endl;
        }
    }

    cout << "Source matrix" << endl;
    outputMatrix(matrix, coef);

    vector<vector<double>> ans = iteration(matrix, coef, epselon);

    if (ans.size() > 2000) {
        cout << "Output of the vector of unknowns!" << endl;
    }

    cout << "Answer:" << endl;
    outputAnswer(ans);
    outputError(ans, matrix, coef);
}

int main()
{
    while (1) {
        int x;

        cout << "0 - input from console" << endl;
        cout << "1 - input from file" << endl;
        cout << "2 - exit" << endl;

        cin >> x;

        if (x == 0) {
            while (1) {
                cout << "Enter matrix size (up to 20): ";
                int n;
                cin >> n;
                if (n < 0 || n > 20) {
                    cout << "Invalid input" << endl;
                    continue;
                }

                cout << "Enter matrix elemenents (x1 x2 ... b1):" << endl;

                vector<vector<double>> matrix(n, vector<double>(n, 0));
                vector<double> coef(n);

                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        cin >> matrix[i][j];
                    }
                    cin >> coef[i];
                }

                cout << "Enter the calculation precision: ";

                double epselon;

                cin >> epselon;

                if (epselon < 0) {
                    cout << "Invalid input" << endl;
                    continue;
                }

                execute(matrix, coef, epselon);
                break;
            }
        }
        else if (x == 1) {
            ifstream file;

            while (1) {
                string fileName;

                cout << "Enter file path: ";
                cin >> fileName;

                file.open(fileName);
                if (!file.is_open()) {
                    cout << "Invalid file name" << endl;
                    continue;
                }

                int n;
                file >> n;
                if (n < 0 || n > 20) {
                    cout << "Invalid size" << endl;
                    continue;
                }

                vector<vector<double>> matrix(n, vector<double>(n, 0));
                vector<double> coef(n);

                for (int i = 0; i < n; i++) {
                    for (int j = 0; j < n; j++) {
                        file >> matrix[i][j];
                    }
                    file >> coef[i];
                }

                double epselon;

                file >> epselon;

                if (epselon < 0) {
                    cout << "Invalid precision" << endl;
                    continue;
                }

                execute(matrix, coef, epselon);
          
                break;
            }
            file.close();
        }
        else if (x == 2) {
            break;
        }
        else {
            cout << "Invalid input" << endl;
            continue;
        }
    }
}

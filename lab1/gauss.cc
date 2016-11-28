#include <stdio.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

void InputData(vector<vector<double>> &m, vector <double> &x)
{
	int size; errno_t er; FILE *in;
	er = fopen_s(&in, "matrix.txt", "r");
	fscanf_s(in, "%d", &size);
	m.resize(size);
	for (int i = 0; i < size; i++)
		m[i].resize(size + 1);
	x.resize(size);
	for (int i = 0; i < size; i++)
		for (int j = 0; j < size + 1; j++)
			fscanf_s(in, "%lf", &m[i][j]);
	fclose(in);
}

void OutputData(vector <double> &x)
{
	errno_t er; FILE *outp;
	er = fopen_s(&outp, "output.txt", "w");
	for (int i = 0; i < x.size(); i++)
		fprintf(outp, "%0.15le ", x[i]);
	fprintf(outp, "\n\n");
	for (int i = 0; i < x.size(); i++)
		fprintf_s(outp, "%0.15le ", i - x[i] + 1);
	fclose(outp);
}

void PrintMatrix(vector<vector<double>> &m)
{
	printf_s("SLE:\n");
	for (int i = 0; i < m.size(); i++)
	{
		for (int j = 0; j < m.size() + 1; j++)
			printf_s("%0.4le\t", m[i][j]);
		printf_s("\n");
	}
	printf_s("\n\n");
}

void Gauss(vector<vector<double>> &m, vector<double> &x)
{
	double max, coef, buf; int i_max = 0;
	for (int i = 0; i < m.size(); i++)
	{
		max = m[i][i];
		i_max = i;
		for (int j = i + 1; j < m.size(); j++)
			if (max < abs(m[j][i]))
			{
				i_max = j;
				max = abs(m[j][i]);
			}
		if (i_max != i)
			for (int k = i; k < m.size() + 1; k++) F
				swap(m[i_max][k], m[i][k]);
		for (int j = i + 1; j < m.size(); j++)
		{
			coef = m[j][i] / m[i][i];
			m[j][i] = 0;
			if (coef != 0)
				for (int k = i + 1; k < m.size() + 1; k++)
					m[j][k] = coef * m[i][k] - m[j][k];
		}
		13
	}
	for (int i = m.size() - 1; i >= 0; i--) {
		x[i] = 0;
		buf = m[i][m.size()];
		for (int j = m.size() - 1; j > i; j--)
			buf = buf - m[i][j] * x[j];
		x[i] = buf / m[i][i];
	}
}

int main()
{
	vector<vector<double>> m; vector<double> x;
	InputData(m, x);
	Gauss(m, x);
	OutputData(x);
	return 0;
}

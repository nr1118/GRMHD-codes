#include <iostream>
#include <vector>
#include<algorithm>
#include <cmath>
// A function that linearly divides a given interval into a given number of
  // points and assigns them to a vector
std::vector <double> linspace(double a, double b, double num)
{
	// create a vector of length num
	std::vector <double> v(num);
	double tmp = 0.0;


	// now assign the values to the vector
	for (int i = 0; i < num; i++)
	{
		v[i] = a + i * ((b - a) / num);
	}
	return v;
}



double Interp(std::vector <double> f_interp, const double dx, const double x, const int n)
// first space is to create an empy vector for the data, dx is delta x, x is the x in which you which to solve, n is the number of terms for the expansion 
{
	for (int k = 0; k < n; k++)
	{
		double f_xi = (pow(dx * k, double(2)) - double(1.0)) / ((dx * k) + double(2.0));

		f_interp.push_back(f_xi);

	}

	// The loop above fills the vector f_interp with the values of f(x) at x = dx*i for i = 0,1,2,3,...,N-1
	double answer = 0;

	for (int i = 1; i <= n; i++) 
	{

		double sum = f_interp[i-1]; // filling in f(x_i) into the product

		for (int j = 1; j <= n; j++) 
		{
			if (j != i)
				sum = sum * ((x + dx) - j * dx) / ((i - j) * dx);
		}

		answer += sum; 	
	}

	return answer;
}

int main()
{
	std::vector <double> f_interp;
	int n = 10;
	double dx = 0.1;
	double a = 0;
	double b = 2;
	int num = 20;

	std::vector <double> x_given = linspace(a, b, num);
	for (double x : x_given)
	{

		double f_x = (pow(x, double(2)) - double(1.0)) / (x + double(2.0)); //f(x)
		double f_tilde = Interp(f_interp, dx, x, n); //f_tilde(x)

		double diff = f_tilde - f_x; // Our function for the differences
		if (x > dx)
		{
			std::cout << x << ",";
			std::cout << diff << "\n";

		}

		if (dx > x)
		{
			std::cout << dx << ",";
			std::cout << diff << "\n";
		}


	}

	return 0;
}

#include <iostream>
#include <vector>
#include<algorithm>

double num_1 = 2.5;
double num_2 = 5.0;

double Interp(std::vector <double> f_interp, const double dx, const double x, const int n,const double num)
// first space is to create an empy vector for the data, dx is delta x, x is the x in which you which to solve, n is the number of terms for the expansion 
{
	for (int k = 0; k < n; k++)
	{
		double f_xi = (num*dx*k)+5; //I simply pick an arbitrary f(x_i) since we don't know what that is for this problem

		f_interp.push_back(f_xi);

	}

	// The loop above fills the vector f_interp with the values of f(x) at x = dx*i for i = 0,1,2,3,...,N-1
	double answer = 0;

	for (int i = 1; i <= n; i++)
	{

		double sum = f_interp[i - 1]; // filling in f(x_i) into the product

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

	double x_1 = num_1 * dx;

	double f_tilde_1 = Interp(f_interp,dx,x_1,n,num_1);
	double f_x_1 = num_1 * x_1 + 5;

	std::cout << "f(x) = " << f_x_1 << "\n";
	std::cout << "f_tilde = " << f_tilde_1 << "\n";

	double x_2 = num_2 * dx;

	double f_tilde_2 = Interp(f_interp, dx, x_2, n, num_2);
	double f_x_2 = num_2 * x_2 + 5;

	std::cout << "f(x) = " << f_x_2 << "\n";
	std::cout << "f_tilde = " << f_tilde_2;
	return 0;
}

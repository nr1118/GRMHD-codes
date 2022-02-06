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


	// now assign the values to the vector
	for (int i = 0; i < num; i++)
	{
		v[i] = a + i * ((b - a) / num);
	}
	return v;
}

std:: vector <double> alpha(int x, int n)
//  dx is delta x, x is the x in which you which to solve, n is the number of terms for the expansion 
{
	std::vector <double> alph(n);
	for (int i = 1; i <= n; i++)
	{
		double num = 1;
		double den = 1;
		for (int j = 1; j <= n; j++)
		{	
			if (j != i) 
			{
				num *= double(x + 2 - 2 * j);
				den *= double(i - j);
			}
		}
		alph[i - 1] =pow(0.5,n-1)**double(num)/double(den);
	}

	return alph;
}


int main()
{
	int n =4;
	//double dx = 0.01;
	//double a = 0;
	//double b = 2;
	//int num = 20;

	if (n%2==0) 
	{
		int x = (n - 1);
		std:: vector <double> alpha_i = alpha(x, n); //alpha_i

		std::cout << "n is even"<< "\n";
		std::cout << "The coeffcients to the polynomial are:" << "\n";
		for (double y : alpha_i) {
			
			std::cout << y << "\n";
		}
		//for (int i = 1; i <= n; i++){
			//std::cout << alpha_i[i] << "\n";
		//}
		
	}
	else 
	{
		double x = n;
		std::vector <double> alpha_i = alpha(x, n); //alpha_i

		std::cout << "n is odd" << "\n";
		std::cout << "The coeffcients to the polynomial are:" << "\n";
		for (double y : alpha_i) {

			std::cout << y << "\n";
		}
		//for (int i = 1; i <= n; i++) {
			//std::cout << alpha_i[i] << "\n";
		//}
	}

		
	return 0;
}

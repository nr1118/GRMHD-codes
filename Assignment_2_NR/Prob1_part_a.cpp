#include <iostream>
#include <vector>
#include <list>
#include "MyClass.hpp" //imports class from the header file

Mesh::Mesh(int n, std::vector <int> Ext):dim(n),Extents(Ext)
{
	if (Extents.size() != n)
	{
		std::cout << "ERROR: Need the vector to be of size "<< n <<"\n";
	}
}
int Mesh::Get_dim() const //defines what the Get_dim() function returns
{
	return dim;
}

std::vector<int>Mesh::Get_grid_pts()  //this function should return the number of grid points along each dimension
{	
	return Extents;
}

int Mesh::Get_total_grid_pts() const //This functions simply just sums up the number of points per dimension
{
	int x = 0;
	for (int i = 0; i < dim; i++) {
		x = x + Extents[i];
	}
	return x;
}

int main()
{
	const int n = 2; //initializing the dimension n
	std::vector <int> Ext = {2,3}; //initializing the number of grid points along each dimension



	Data_Mesh <double> My_Data(); // sets the MY_Data vector to take data types of the double kind



	std::cout << "The total number of grid points are: " << mymesh.Get_total_grid_pts() << "\n";

	for (int i = 0; i < n; i++) {
		std::cout << mymesh.Get_grid_pts()[i] << "," << i+1 << "\n";
	}

	return 0;
}

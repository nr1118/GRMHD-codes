#include <iostream>
#include <vector>
#include <list>
#include "MyClass_prob1a.hpp" //imports class from the header file

Mesh::Mesh(int n, std::vector <int> Ext):dim(n),Extents(Ext)
{
	if (Extents.size() != n)
	{
		std::cout << "ERROR: Need the vector to be of size "<< n <<"\n";
	}
}
int Mesh::Get_dim() const
{
	return dim;
}

int main()
{
	const int n = 2; //initializing the dimension n
	std::vector <int> Ext = {1,1}; //initializing the number of grid points along each dimension



	Mesh mymesh(n, Ext);
	std::cout << mymesh.Get_dim();

	return 0;
}

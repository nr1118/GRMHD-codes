#include <iostream>
#include <vector>
#include <list>
#include "MyClass._Prob1_b.hpp" //imports class from the header file

Mesh::Mesh(int n, std::vector <int> Ext):dim(n),Extents(Ext)
{
	if (Extents.size() != n)
	{
		std::cout << "ERROR: Need the vector to be of size "<< n <<"\n";
		exit(1);
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
	int x = 1;
	for (int i = 0; i < dim; i++) {
		x = x * Extents[i];
	}
	return x;
}
const std::vector <double>& Data_Mesh<double>::Get_Const_Data()
{
	return Data; // I tried using &Const_Data and I get an error saying "no suitable constructor exists, but it works without the pointer.
}

std::vector <double>& Data_Mesh<double>::Get_Mod_Data()
{
	return Data; 
}

int main()
{
	const int n = 2; //initializing the dimension n
	std::vector <int> Ext = {2,3}; //initializing the number of grid points along each dimension



	Mesh mymesh(n, Ext); //creates an object of the class Mesh and this will be used call anything within the public part of Mesh.

	Data_Mesh<double> mDm(n, Ext); //creates an object of the class Data_Mesh and specifies a type since we did not when making the template
	
	{
		std::vector<double>& Data = mDm.Get_Mod_Data();
		Data = { 1.0 };
	}
	
	{
		const std::vector<double>& Data = mDm.Get_Const_Data();
	}

	for (double x : mDm.Get_Const_Data())
	{
		std::cout << x;
	}

	return 0;
}


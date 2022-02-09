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
	int x = 0;
	for (int i = 0; i < dim; i++) {
		x = x + Extents[i];
	}
	return x;
}
const std::vector <double>& Data_Mesh<double>::Get_Const_Data() const
{
	return Const_Data; // I tried using &Const_Data and I get an error saying "no suitable constructor exists, but it works without the pointer.
}

std::vector <double>& Data_Mesh<double>::Get_Mod_Data()
{
	return Mod_Data; 
}

int main()
{
	const int n = 2; //initializing the dimension n
	std::vector <int> Ext = {2,3}; //initializing the number of grid points along each dimension



	Mesh mymesh(n, Ext); //creates an object of the class Mesh and this will be used call anything within the public part of Mesh.

	Data_Mesh<double> mDm(n, Ext); //creates an object of the class Data_Mesh and specifies a type since we did not when making the template
	
	
	std::vector<double> Data = mDm.Get_Const_Data();
	Data = { 1.0 ,2,4,3};
	
	for (double x : Data) // I feel like this is not what we are looking for since this loop just print whatever is in data and I am unsure how see what is in Const_Data
	{
		std::cout << x;
	}
	

	//std::cout << "The total number of grid points are: " << mymesh.Get_total_grid_pts() << "\n";

	//for (int i = 0; i < n; i++) {
		//std::cout << mymesh.Get_grid_pts()[i] << "," << i+1 << "\n";
	//}

	return 0;
}

//potentially useful thing here 
// Data_Mesh::Data_Mesh(): Mesh(n,Ext)  //this creates a function from the inherited class Data_Mesh called Data_Mesh and it has acess to the Mesh class

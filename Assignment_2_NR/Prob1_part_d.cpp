#include <iostream>
#include <vector>
#include <list>
#include "MyClass.hpp" //imports class from the header file

Mesh::Mesh(const int n, std::vector <int> Ext):dim(n),Extents(Ext)
{
	if (Extents.size() != n)
	{
		std::cout << "ERROR: Need the Extents vector to be of size "<< n <<"\n";
		exit(1);
	}
}
int Mesh::Get_dim() const //defines what the Get_dim() function returns
{
	return dim;
}

std::vector<int>Mesh::Get_grid_pts()const  //this function should return the number of grid points along each dimension
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


std::vector<double> Patch::Get_low_bound()
{
	return low_bound;
}

std::vector<double> Patch::Get_up_bound()
{
	return up_bound;
}


std::vector<std::vector<double>>Patch::Get_coords()
{
	std::vector<std::vector<double>> coords;

	for (int j = 0; j < Get_grid_pts().size(); j++)
	{

		// create a vector of length number
		std::vector <double> v(Get_grid_pts()[j]);



		// now assign the values to the vector
		for (int i = 0; i < v.size(); i++)
		{
			v[i] = low_bound[j] + i * ((up_bound[j] - low_bound[j]) / (v.size() - 1)); //this is a convention that includes [a,b] on the dimensions interval
		}
		coords.push_back(v);
	}
	return coords;
}

int main()
{
	const int n = 2; //initializing the dimension n
	std::vector <int> Ext = {3,3}; //initializing the number of grid points along each dimension



	Mesh mymesh(n, Ext); //creates an object of the class Mesh and this will be used call anything within the public part of Mesh.

	Data_Mesh<double> mDm(n, Ext); //creates an object of the class Data_Mesh and specifies a type since we did not when making the template


	std::vector<double> a = { 1,1 };
	std::vector<double> b = { 2,2 };
	

	Patch patch(n, Ext, a, b);

	//THE COMMENTS BELOW ARE THINGS ONE CAN DO TO CHECK THE DATA MESH AND PATCH CLASSES VIA SOME EXECUTABLES

	//{
		//std::vector<double>& Data = mDm.Get_Mod_Data();
		//Data = { 1.0 };
	//}

	//{
		//const std::vector<double>& Data = mDm.Get_Const_Data();
	//}

	//for (double x : mDm.Get_Const_Data())
	//{
		//std::cout << x;
	//}
	//std::cout << "The total number of grid points are: " << mymesh.Get_total_grid_pts() << "\n";

	//for (int i = 0; i < n; i++) {
		//std::cout << mymesh.Get_grid_pts()[i] << "," << i+1 << "\n";
	//}
	//for (int i = 0; i < mymesh.Get_grid_pts()[j]; i++)
		//{
			//std::cout << patch.Get_coords()[j][i] << "\n";

		//}
	//}

	//std::cout << patch.Get_up_bound();

	// THE CODE BELOW IS USED FOR OVERLOADING THE OPERATORS: +,+=, AND *

	Data_Mesh <int> A(2, std::vector<int>{1, 2});
	std::vector <int>& Data_A = A.Get_Mod_Data();
	Data_A = { 2 ,2};
	//Data_A.assign(1, 1);
	Data_Mesh <int> B(2, std::vector<int>{1, 2});
	std::vector<int>& Data_B = B.Get_Mod_Data();

	//for (int i = 0; i < A.Get_total_grid_pts(); i++)
	//{
		//Data_B[i] = B.set_size(i, val)
	//}
	Data_B = { 3 ,2};

	Data_Mesh<int> C = A += B ;


	for (int i = 0; i < A.Get_total_grid_pts(); i++)
	{
		std::cout << C.Get_Mod_Data()[i] << "\n";
	}

	double Z = 2.0;

	Data_Mesh <double> H(2, std::vector<int>{1, 2});
	std::vector <double>& Data_H = H.Get_Mod_Data();
	Data_H = { 2 ,2 };

	Data_Mesh<double> H_scaled = H*Z;

	for (int i = 0; i < A.Get_total_grid_pts(); i++)
	{
		std::cout << H_scaled.Get_Mod_Data()[i] << "\n";
	}

	
	

	return 0;
}

#include <iostream>
#include <vector>
#include <list>
#include <cmath>
#include "MyClass.hpp" //imports class from the header file


double pi = 3.14159265358979323846; //tried to use M_PI, but kept getting an error


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



 const int Mesh::Get_total_grid_pts() const //This functions simply just sums up the number of points per dimension
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
	const std::vector <int> Ext = {3,3}; //initializing the number of grid points along each dimension {t,x}


	 //creates an object of the class Mesh and this will be used call anything within the public part of Mesh.
	Mesh mymesh(n, Ext);

	//creates an object of the class Data_Mesh and specifies a type since we did not when making the template
	Data_Mesh<double> mDm(n, Ext); //will be used for the initial conditions of our diff_eq


	// upper and lower bound for each dimension stated
	std::vector<double> a = { 0,0 }; //{t,x}
	std::vector<double> b = { 10,2*pi}; // {t,x}
	

	//creates an instance of the Patch class which defines the coordinate system on the Mesh grid
	Patch patch(n, Ext, a, b);

	// Initializing Data for mDm Data_Mesh object and filling it with a bunch of zeros
	std::vector<double>& Data = mDm.Get_Mod_Data();
	
	for (int i = 0; i < mymesh.Get_total_grid_pts(); i++)
	{
		Data[i] = 1.0;
	}

	RhsComputation RhsComp;
	Timestepper Euler;
	const double c = -1.0;
	const double dx = patch.Get_dx(); //added a new funciton in Patch class that ensures that  dx is fixed
	const double dt = 0.25;
	std::vector<Data_Mesh<double>> U(1,mDm); //U(number of variables, mDm Data_Mesh


	Euler.Take_time_step_Euler(dx,c,dt, U, RhsComp);

	for (int i = 0; i < U.size(); i++)
	{
		for (double x: U[i].Get_Const_Data())
		{
			std::cout << x<< "\n";
		}
	}
	
	return 0;
}

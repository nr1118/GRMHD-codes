#pragma once


class Mesh 
{

public: // creating publically (user interface acessible) available objects
	//Mesh();
	Mesh(int n, std::vector <int> Ext); // this requires we define the dimension in the .cpp file as well as the grids extent??
	~Mesh(void) {}
	int Get_dim() const;

private:// private means cannot be touched by a user and only accessible to a script writer
	int dim; // setting the dimensions of the mesh grid and the user cannot touch this
	std::vector <int> Extents;


};

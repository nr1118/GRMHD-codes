#pragma once


class Mesh 
{

public: // creating publically (user interface acessible) available objects
	//Mesh(); //default constructor (not really needed though)
	Mesh(const int n, std::vector <int> Ext); // this requires we define the dimension in the .cpp file as well as the grids extent??
	~Mesh(void) {}; // destructor operator. Automatically called when the functions are done executing. This operator destroys the associated object with this class
	int Get_dim() const; //initializes the Get_dim() function
	std::vector<int> Get_grid_pts();
	int Get_total_grid_pts() const;

private:// private means cannot be touched by a user and only accessible to a script writer
	int dim; // setting the dimensions of the mesh grid and the user cannot touch this
	std::vector <int> Extents; //vector created that will take into account the number of grid points associated with each dimension of dim


};

// Here is a link to understanding inheritance //https://www.programiz.com/cpp-programming/inheritance#:~:text=It%20allows%20us%20to%20create,%2F%2F%20bark()%20function%20%7D%3B
//Essentially this class has acess to everything in Mesh and has some of its own properties
template <class T> class Data_Mesh:public Mesh
{
public:
	Data_Mesh(const int n, std::vector <int> Ext) :Mesh(n, Ext) {}
	//~Data_Mesh::Data_Mesh(void):Mesh(n,Ext) {};
	const std::vector <T>& Get_Const_Data() const;
	std::vector <T>& Get_Mod_Data();
private: 
	 const std::vector <T> Const_Data; //creates a vector of type T called Const_Data
	 std::vector<T> Mod_Data;
};

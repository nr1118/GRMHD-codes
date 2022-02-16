#pragma once


class Mesh 
{

public: // creating publically (user interface acessible) available objects
	//Mesh(); //default constructor (not really needed though)
	Mesh(const int n, std::vector <int> Ext); // this requires we define the dimension in the .cpp file as well as the grids extent??
	~Mesh(void) {}; // destructor operator. Automatically called when the functions are done executing. This operator destroys the associated object with this class
	int Get_dim() const; //initializes the Get_dim() function
	std::vector<int> Get_grid_pts() const;
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
	Data_Mesh(const int n, std::vector <int> Ext) :Mesh(n, Ext)
	{

		Data.resize(Get_total_grid_pts());
		if (Data.size() != this->Get_total_grid_pts())
		{
			std::cout << "ERROR: Make sure the number of data points is equal to Total number of grid points!";
			exit(1);
		}
	}
	//~Data_Mesh::Data_Mesh(void):Mesh(n,Ext) {};
	const std::vector <T>& Get_Const_Data(){return Data;}
	std::vector <T>& Get_Mod_Data() { return Data; }
	//Below is a commented out version of the set_size becauase I was unsure what type to call it and how to actually create the function to ensure it always has the right size
	 //T set_size(const int i, const T val);

	Data_Mesh<T> operator +(const Data_Mesh<T> &B) //Defines how to add two classes with a few safety features
	{
		if (this->Get_dim() != B.Get_dim())  
		{
			std::cout<< "ERROR: The dimensions of A and B do not match!";
			exit(1);
		}
		for (int i = 0; i < this->Get_dim(); i++)
		{
			if (this->Get_grid_pts()[i] != B.Get_grid_pts()[i])  
			{
				std::cout << "ERROR: The number of grid points for a dimension(s) of A and B do not match!";
				exit(1);
			}
		}
	
		
		Data_Mesh<T> C(this->Get_dim(),this->Get_grid_pts());

		if (typeid(T) == typeid(bool))
		{
			for (int i = 0; i < this->Get_total_grid_pts(); i++)
			{
				C.Data[i] = Data[i] || B.Data[i];
			}
			
		}
		if (typeid(T) != typeid(bool))
		{
			for (int i = 0; i < this->Get_total_grid_pts(); i++)
			{
				C.Data[i] = Data[i] + B.Data[i];
			}

		}

		return C;


	}

	Data_Mesh<T> operator +=(const Data_Mesh <T> &B) //Defines how to add an empty class to a class via +=
	{
		if (this->Get_dim() != B.Get_dim())
		{
			std::cout << "ERROR: The dimensions of A and B do not match!";
			exit(1);
		}
		for (int i = 0; i < this->Get_dim(); i++)
		{
			if (this->Get_grid_pts()[i] != B.Get_grid_pts()[i])
			{
				std::cout << "ERROR: The number of grid points for a dimension(s) of A and B do not match!";
				exit(1);
			}
		}
		Data_Mesh<T> C(this->Get_dim(), this->Get_grid_pts()); // is this really even needed? and does data need to be initiliazed?
		if (typeid(T) == typeid(bool))
		{
			for (int i = 0; i < this->Get_total_grid_pts(); i++)
			{
				Data[i] = Data[i] || B.Data[i];
			}
		}
		if (typeid(T) != typeid(bool))
		{
			for (int i = 0; i < this->Get_total_grid_pts(); i++)
			{
				Data[i] = Data[i]+B.Data[i];
				
			}
		}
		for (int i = 0; i < this->Get_total_grid_pts(); i++)
		{
			C.Data[i] = Data[i];
		}
		
		return C;
	}
	Data_Mesh<T> operator *(const T& B) //Defines how to multiply a class by a scalar
	{
		Data_Mesh<T> C(this->Get_dim(), this->Get_grid_pts());
		if (typeid(T) == typeid(bool)) 
		{
			for (int i = 0; i < this->Get_total_grid_pts(); i++)
			{
				C.Data[i] = B && Data[i];
			}
		}
		if (typeid(T) != typeid(bool))
		{
			for (int i = 0; i < this->Get_total_grid_pts(); i++)
			{
				C.Data[i] = B * Data[i];
			}
		}
		return C;
	}
private: 
	std::vector<T>Data;
};

class Patch :public Mesh
{
public:
	Patch(const int n, std::vector <int> Ext, const std::vector<double> a, const  std::vector<double> b) :Mesh(n, Ext),low_bound(a),up_bound(b) {}
	std::vector<double> Get_low_bound();
	std::vector<double> Get_up_bound();
	std::vector<std::vector<double>> Get_coords();



private:
	std::vector<double> low_bound;
	std::vector<double> up_bound;

};

#pragma once


class Mesh 
{

public: // creating publically (user interface acessible) available objects
	//Mesh(); //default constructor (not really needed though)
	Mesh(const int n,const std::vector <int> Ext); // this requires we define the dimension in the .cpp file as well as the grids extent??
	~Mesh(void) {}; // destructor operator. Automatically called when the functions are done executing. This operator destroys the associated object with this class
	int Get_dim() const; //initializes the Get_dim() function
	std::vector<int> Get_grid_pts() const;
	const int Get_total_grid_pts() const;

private:// private means cannot be touched by a user and only accessible to a script writer
	int dim; // setting the dimensions of the mesh grid and the user cannot touch this
	std::vector <int> Extents; //vector created that will take into account the number of grid points associated with each dimension of dim


};

// Here is a link to understanding inheritance //https://www.programiz.com/cpp-programming/inheritance#:~:text=It%20allows%20us%20to%20create,%2F%2F%20bark()%20function%20%7D%3B
//Essentially this class has acess to everything in the public of Mesh and has some of its own properties

template <class T> class Data_Mesh:public Mesh
{
public:
	Data_Mesh(const int n, std::vector <int> Ext) :Mesh(n, Ext)
	{

		Data.resize(Get_total_grid_pts());

	}

	//~Data_Mesh::Data_Mesh(void):Mesh(n,Ext) {};
	const std::vector <T>& Get_Const_Data(){return Data;}
	std::vector <T>& Get_Mod_Data() { return Data; }
	//Below is a commented out version of the set_size becauase I was unsure what type to call it and how to actually create the function to ensure it always has the right size
	 //T set_size(const int i, const T val);

	Data_Mesh<T> operator +(const Data_Mesh<T> &B) const //Defines how to add two classes with a few safety features
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
		if (Data.size() != this->Get_total_grid_pts())
		{
			std::cout << "ERROR: The size of the data inputs do not match the total number of grid points! ";
			exit(1);
		}
		if (B.Data.size() != this->Get_total_grid_pts())
		{
			std::cout << "ERROR: The size of the data inputs do not match the total number of grid points! ";
			exit(1);
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

	Data_Mesh<T> operator +=(const Data_Mesh <T> &B) const //Defines how to add an empty class to a class via +=
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
		if (Data.size() != this->Get_total_grid_pts())
		{
			std::cout << "ERROR: The size of the data inputs do not match the total number of grid points! ";
			exit(1);
		}
		if (B.Data.size() != this->Get_total_grid_pts())
		{
			std::cout << "ERROR: The size of the data inputs do not match the total number of grid points! ";
			exit(1);
		}
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
		
		return Data;
	}
	Data_Mesh<T> operator *(const T& B) const //Defines how to multiply a class by a scalar
	{
			if (Data.size() != this->Get_total_grid_pts())
		{
			std::cout << "ERROR: The size of the data inputs do not match the total number of grid points! ";
			exit(1);
		}

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
	double Get_dx()const 
	{
		const double dx = (up_bound[1]- low_bound[1])/(Get_grid_pts()[1] - 1);

		return dx;
	}



private:
	std::vector<double> low_bound;
	std::vector<double> up_bound;

};


class Scheme 
{
public:

	void Center_diff(const double dx, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdx) const
	{
		for (int i = 0; i < U.size(); i++)
		{
			if (i-1 < 0)
			{
				dUdx[i] = (U[i + 1] + U[U.size()] * -1.0) * (1 / (2 * dx));
			}

			if(i + 1 < U.size() and i-1 >0)
			{
				dUdx[i] = (U[i + 1] + U[i - 1] * -1.0) * (1 / (2 * dx));
			}

			if (i + 1 > U.size())
			{
				dUdx[i] = (U[0]+U[i-1]*-1.0) * (1 / (2 * dx));
			}
			
		}
	}

};

class RhsU 
{
public:

	void Rhs( const double dx,const double c,const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdt, const Scheme& Method) const
	{
		std::vector<Data_Mesh<double>>dUdx(U); // TO DO: When I replace U.size() with U The answer that comes out is just the Initial condition + dt. That is it.

		Method.Center_diff(dx, U, dUdx);

		for (int i = 0; i < U.size(); i++)
		{
			dUdt[i] = dUdx[i] * -c; // this says that dUdt = -
		}
	}

	//this class will take into account what dudx is and say that dudt = -c (part a) for example
};

class RhsComputation :public RhsU
{
	//this class will take in RhsU and compute the right hand side of the ode

public:
	//RhsComputation(); //constructor

	void Rhs(const double dx,const double c,const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdt, const Scheme& Method) const
	{
		RhsU::Rhs(dx,c,U, dUdt,Method);
	}


};


class Timestepper
{	//the function of this class is to just evolve the equation from t to t+dt
public:
	//Timestepper(); //constructor, might need if we need to initialize an object

	void Take_time_step_Euler(const double dx,const double c,const double dt, std::vector<Data_Mesh<double>>& U, const RhsComputation& RhsComp, const Scheme&Method)
	{
		std::vector<Data_Mesh<double>> dUdt(U);

		RhsComp.Rhs(dx,c,U,dUdt,Method);
		for (int i = 0; i < U.size(); i++)
		{
			U[i] = U[i]+ dUdt[i] * dt;
		}
	}

};

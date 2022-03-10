#pragma once


class Mesh
{

public: // creating publically (user interface acessible) available objects
	//Mesh(); //default constructor (not really needed though)
	Mesh(const int n, const std::vector <int> Ext); // this requires we define the dimension in the .cpp file as well as the grids extent??
	//~Mesh(void) {}; // destructor operator. Automatically called when the functions are done executing. This operator destroys the associated object with this class
	int Get_dim() const; //initializes the Get_dim() function
	std::vector<int> Get_grid_pts() const;
	const int Get_total_grid_pts() const;

private:// private means cannot be touched by a user and only accessible to a script writer
	int dim; // setting the dimensions of the mesh grid and the user cannot touch this
	std::vector <int> Extents; //vector created that will take into account the number of grid points associated with each dimension of dim


};

// Here is a link to understanding inheritance //https://www.programiz.com/cpp-programming/inheritance#:~:text=It%20allows%20us%20to%20create,%2F%2F%20bark()%20function%20%7D%3B
//Essentially this class has acess to everything in the public of Mesh and has some of its own properties

template <class T> class Data_Mesh :public Mesh
{
public:
	Data_Mesh(const int n, std::vector <int> Ext) :Mesh(n, Ext)
	{

		Data.resize(Get_total_grid_pts());

	}

	//~Data_Mesh::Data_Mesh(void):Mesh(n,Ext) {};
	const std::vector <T>& Get_Const_Data() { return Data; }
	std::vector <T>& Get_Mod_Data() { return Data; }
	//Below is a commented out version of the set_size becauase I was unsure what type to call it and how to actually create the function to ensure it always has the right size
	 //T set_size(const int i, const T val);

	Data_Mesh<T> operator +(const Data_Mesh<T>& B) const //Defines how to add two classes with a few safety features
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


		Data_Mesh<T> C(this->Get_dim(), this->Get_grid_pts());

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

	Data_Mesh<T> operator +=(const Data_Mesh <T>& B)  //Defines how to add an empty class to a class via +=
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
				Data[i] = Data[i] + B.Data[i];

			}
		}
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
	Patch(const int n, std::vector <int> Ext, const std::vector<double> a, const  std::vector<double> b) :Mesh(n, Ext), low_bound(a), up_bound(b) {}
	std::vector<double> Get_low_bound();
	std::vector<double> Get_up_bound();
	std::vector<std::vector<double>> Get_coords();
	double Get_dx()const  //THIS IS PART IS ONLY SET UP FOR 1D you will have to add other functions for more dimensions
	{
		const double dx = (up_bound[0] - low_bound[0]) / (Get_grid_pts()[0] - 1);

		return dx;
	}



private:
	std::vector<double> low_bound;
	std::vector<double> up_bound;

};

class  Scheme
{
public:

	void RK3(const double& dx, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdx)const
	{
		

	}

	void Up_diff(const double& dx, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdx) const
	{
		for (int k = 0; k < U.size(); k++)
		{
			Data_Mesh<double> x = U[k];
			Data_Mesh<double>& y = dUdx[k];

			std::vector<double> Z = x.Get_Mod_Data();

			for (int i = 0; i < Z.size(); i++)
			{
				if (i == 0)
				{
					y.Get_Mod_Data()[i] = (Z[i] - Z[Z.size() - 1]) * (1 / dx);

					continue;
				}

				y.Get_Mod_Data()[i] = (Z[i] - Z[i - 1]) * (1 / dx);

			}


		}
	}
	void Down_diff(const double& dx, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdx) const
	{
		for (int k = 0; k < U.size(); k++)
		{
			Data_Mesh<double> x = U[k];
			Data_Mesh<double>& y = dUdx[k];

			std::vector<double> Z = x.Get_Mod_Data();

			for (int i = 0; i < Z.size(); i++)
			{
				if (i == Z.size() - 1)
				{
					y.Get_Mod_Data()[i] = (Z[0] - Z[i]) * (1 /  dx);

					continue;
				}

				y.Get_Mod_Data()[i] = (Z[i + 1] - Z[i]) * (1 / dx);

			}

		}
	}

	void Center_diff(const double& dx, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdx) const
	{
		for (int k = 0; k < U.size();k++)
		{
			Data_Mesh<double> x = U[k];
			Data_Mesh<double>& y = dUdx[k];

			std::vector<double> Z = x.Get_Mod_Data();

			for (int i = 0; i < Z.size(); i++)
			{
				if (i == 0)
				{
					y.Get_Mod_Data()[i] = (Z[i + 1] - Z[Z.size() - 1]) * (1 / (2 * dx));

					continue;
				}
				if (i == Z.size() - 1)
				{
					y.Get_Mod_Data()[i] = (Z[0] - Z[i-1]) * (1 / (2 * dx));

					continue;
				}

				y.Get_Mod_Data()[i] = (Z[i + 1] - Z[i - 1]) * (1 / (2 * dx));

			}


		}

	}

};


// TO DO: I need to start the sum from i=1 and not i=0 since U[i-1] is an issue. When I try to use an if statement to re-idneify the point it still does not like my solution.
class RhsU
{
public:

	void Rhs(const double& dx, const double& c, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdt, const Scheme& Method) const
	{
		std::vector<Data_Mesh<double>> dUdx = U;

		Method.Up_diff(dx, U, dUdx);


		for (int i = 0; i < U.size(); i++)
		{
			dUdt[i] = dUdx[i] * -1 * c; // this says that dUdt = -c dUdx
		}
	}

	//this class will take into account what dudx is and say that dudt = -c (part a) for example
};

class RhsComputation :public RhsU
{
	//this class will take in RhsU and compute the right hand side of the ode

public:
	//RhsComputation(); //constructor

	void Rhs(const double& dx, const double& c, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdt, const Scheme& Method) const
	{
		RhsU::Rhs(dx, c, U, dUdt, Method);
	}


};


class Timestepper
{	//the function of this class is to just evolve the equation from t to t+dt
public:
	//Timestepper(); //constructor, might need if we need to initialize an object

	void Take_Ntime_steps_Euler(const int& N_steps, double& t_0, const double& dx, const double& c, const double& dt, std::vector<Data_Mesh<double>>& U, std::vector<double>& U_store, const RhsComputation& RhsComp, const Scheme& Method)
	{
		std::vector<Data_Mesh<double>> dUdt = U;
		for (int i = 0; i < N_steps; i++)
		{
			t_0 = t_0 + dt;
			std::vector<Data_Mesh<double>> dUdt = U;

			RhsComp.Rhs(dx, c, U, dUdt, Method);

			for (int j = 0; j < U.size(); j++)
			{
				U[j] = U[j] + dUdt[j] * dt;


			}


			if (i % 2 == 0)
			{


				for (Data_Mesh<double>x : U)
				{
					for (int i = 0; i < x.Get_Const_Data().size(); i++)
					{
						U_store.push_back(x.Get_Const_Data()[i]);

					}


				}
				U_store.push_back(t_0);
			}


		}
	}
	void Take_time_step_Euler(double& t_0, const double& dx, const double& c, const double& dt, std::vector<Data_Mesh<double>>& U, const RhsComputation& RhsComp, const Scheme& Method)
	{

		t_0 = t_0 + dt;
		std::vector<Data_Mesh<double>> dUdt = U;

		RhsComp.Rhs(dx, c, U, dUdt, Method);

		for (int i = 0; i < U.size(); i++)
		{
			U[i] = U[i] + dUdt[i] * dt;
		}
	}

};
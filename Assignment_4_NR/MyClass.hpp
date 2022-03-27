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
					y.Get_Mod_Data()[i] = (Z[i] - Z[Z.size() - 2]) * (1 / dx);
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
					y.Get_Mod_Data()[i] = (Z[1] - Z[i]) * (1 /  dx);
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
					y.Get_Mod_Data()[i] = (Z[i + 1] - Z[Z.size() - 2]) * (1 / (2 * dx)); // need Z.size()-2 since the 0th index and last index are the same point!! The same reasoning applies for other side.
					continue;
				}
				if (i == Z.size() - 1)
				{
					y.Get_Mod_Data()[i] = (Z[1] - Z[i-1]) * (1 / (2 * dx));
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

		Method.Center_diff(dx, U, dUdx);


		for (int i = 0; i < U.size(); i++)
		{
			dUdt[i] = dUdx[i] * -1 * c; // this says that dUdt = -c dUdx
		}
	}

	void Rhs_Burgers(const double& dx,const double& v, const double& c, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdt, const Scheme& Method) const
	{
		std::vector<Data_Mesh<double>> dUdx = U;

		Method.Center_diff(dx, U, dUdx);

		std::vector<Data_Mesh <double>> d2Udx2 = dUdx;

		Method.Center_diff(dx, dUdx, d2Udx2);

		for (int i = 0; i < U.size(); i++)
		{
			dUdt[i] = dUdx[i] * -1 * c+ d2Udx2[i]*v; // this says that dUdt = -c dUdx+ v d2Udx2
		}


	}

	//this class will take into account what dudx is and say that dudt = -c (part a) for example
};

class RhsComputation :public RhsU
{
	//this class will take in RhsU and compute the right hand side of the ode

public:
	//RhsComputation(); //constructor

	void Rhs(const double& dx, const double&v, const double& c, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdt, const Scheme& Method) const
	{
		RhsU::Rhs_Burgers(dx,v, c, U, dUdt, Method);
	}


};


class Timestepper
{	//the function of this class is to just evolve the equation from t to t+dt
public:
	//Timestepper(); //constructor, might need if we need to initialize an object

	void Take_Ntime_steps_Euler(const int& N_steps, double& t_0, const double& dx, const double&v, const double& c, const double& dt, std::vector<Data_Mesh<double>>& U, std::vector<double>& U_store, const RhsComputation& RhsComp, const Scheme& Method)
	{
		std::vector<Data_Mesh<double>> dUdt = U;
		for (int i = 0; i < N_steps; i++)
		{
			t_0 = t_0 + dt;
			std::vector<Data_Mesh<double>> dUdt = U;

			RhsComp.Rhs_Burgers(dx,v, c, U, dUdt, Method);

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
	void Take_time_step_Euler(double& t_0, const double& dx,const double&v, const double& c, const double& dt, std::vector<Data_Mesh<double>>& U, const RhsComputation& RhsComp, const Scheme& Method) const
	{

		t_0 = t_0 + dt;
		std::vector<Data_Mesh<double>> dUdt = U;

		RhsComp.Rhs_Burgers(dx,v, c, U, dUdt, Method);

		for (int i = 0; i < U.size(); i++)
		{
			U[i] = U[i] + dUdt[i] * dt;
		}
	}
	//N is the RK order, so far only set up for N=2
	void Take_time_step_RK2_loop(const int N, double& t_0, const double& dx,const double&v, const double& c, const double& dt, std::vector<Data_Mesh<double>>& U, const RhsComputation& RhsComp, const Scheme& Method) const
	{
		std::vector<Data_Mesh<double>> U_0 = U; // yes I have tried a reference here although I did not think it would help and it did not. 

		std::vector<std::vector<Data_Mesh<double>>>dUdt(N, U_0); // has N vectors of Data_Meshes
																// each filled with U_0 to start. In this case it is 2 since RK3
		std::vector<double> alpha = { 1,1 };

		std::vector<std::vector<double>> ckn = { {1},{0.5,0.5} }; // for the difference coeffcients in reference to problem 2

		std::vector<std::vector<Data_Mesh<double>>> sum(N,U_0);

		for (int i = 0; i < U.size(); i++)// for each coordinate so in this case it will just be 1
		{
				for (int n = 0; n < N; n++)
				{
					RhsComp.Rhs_Burgers(dx,v, c, U, dUdt[n], Method);
					for (int k = 0; k <= n; k++)
					{
						sum[n][i] = sum[n][i]+ dUdt[k][i] * ckn[n][k] * alpha[k] * dt; // for some reason my += operator is not working so this is my best choice for now. 
						continue;
					}
					U[i] = sum[n][i];
					
				}
			t_0 = t_0 + dt;
		}
	}
};
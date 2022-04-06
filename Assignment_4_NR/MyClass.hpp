#pragma once
class Mesh
{

public: 
	//Mesh(); 
	Mesh(const int n, const std::vector <int> Ext); 
	//~Mesh(void) {}; 
	int Get_dim() const; 
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
					y.Get_Mod_Data()[i] = (Z[1] - Z[i]) * (1 / dx);
					continue;
				}
				y.Get_Mod_Data()[i] = (Z[i + 1] - Z[i]) * (1 / dx);

			}

		}
	}

	void Center_diff(const double& dx, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& dUdx) const
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
					y.Get_Mod_Data()[i] = (pow(Z[i + 1], 1) - pow(Z[Z.size() - 2], 1)) * (1 / (2 * dx)); // need Z.size()-2 since the 0th index and last index are the same point!! The same reasoning applies for other side.
					continue;
				}
				if (i == Z.size() - 1)
				{
					y.Get_Mod_Data()[i] = (pow(Z[1], 1) - pow(Z[i - 1], 1)) * (1 / (2 * dx));
					continue;
				}

				y.Get_Mod_Data()[i] = (pow(Z[i + 1], 1) - pow(Z[i - 1], 1)) * (1 / (2 * dx));
			}
		}
	}

};



// TO DO: I need to start the sum from i=1 and not i=0 since U[i-1] is an issue. When I try to use an if statement to re-idneify the point it still does not like my solution.
class RhsU
{
public:

	void Rhs_Lax(const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& F_plus_half, std::vector<Data_Mesh<double>>&F_minus_half,const Scheme& Method) const
	{
		//FOR F_PLUS_ONE HALF

		// F_{i+1/2} = (F_{i+1}+F_{i})/2 - alpha*(u_{i+1}-u_{i}) =  (F_{i+1}+F_{i})/2 - alpha*W[i]

		std::vector<Data_Mesh<double>> W = U;

	
		Method.Down_diff(1.0, U, W);

		for (int k = 0; k < U.size(); k++)
		{
			Data_Mesh<double>& f = F_plus_half[k];
			Data_Mesh<double> u = U[k];
			Data_Mesh<double> w = W[k];

			for (int i=0; i < f.Get_Mod_Data().size(); i++)
			{
				if (i == f.Get_Mod_Data().size() - 1)
				{
					f.Get_Mod_Data()[i] = (pow(u.Get_Const_Data()[1], 2) + pow(u.Get_Const_Data()[i], 2)) * 0.5 - w.Get_Mod_Data()[i]* std::max(u.Get_Const_Data()[1], u.Get_Const_Data()[i]);
					continue;
				}
				f.Get_Mod_Data()[i] = (pow(u.Get_Const_Data()[i+1], 2)+pow(u.Get_Const_Data()[i],2))*0.5 - w.Get_Mod_Data()[i]*std::max(u.Get_Const_Data()[i+1],u.Get_Const_Data()[i]);

			}
		}

		//FOR F_PLUS_minus_HALF

		// F_{i-1/2} = (F_{i}+F_{i-1})/2 - alpha*(u_{i}-u_{i-1}) =  (F_{i}+F_{i-1})/2 - alpha*W[i]

		std::vector<Data_Mesh<double>> Q = U;

		Method.Up_diff(1.0, U, Q);

		for (int k = 0; k < U.size(); k++)
		{
			Data_Mesh<double>& f = F_minus_half[k];
			Data_Mesh<double> u = U[k];
			Data_Mesh<double> q = Q[k];

			for (int i=0; i < f.Get_Mod_Data().size(); i++)
			{
				if (i == 0)
				{
					f.Get_Mod_Data()[i] = (pow(u.Get_Mod_Data()[i], 2) + pow(u.Get_Mod_Data()[f.Get_Mod_Data().size()-2], 2)) * 0.5 -  q.Get_Mod_Data()[i] *std::max(u.Get_Const_Data()[i],u.Get_Const_Data()[f.Get_Mod_Data().size()-2]);
					continue;
				}
				f.Get_Mod_Data()[i] = (pow(u.Get_Mod_Data()[i], 2) + pow(u.Get_Mod_Data()[i - 1], 2)) * 0.5 - q.Get_Mod_Data()[i] * std::max(u.Get_Const_Data()[i], u.Get_Const_Data()[i - 1]);

			}
		}
		
	}

};


class RhsComputation :public RhsU
{

public:
	//RhsComputation(); //constructor

	void Rhs(const double& dx, const std::vector<Data_Mesh<double>>& U, std::vector<Data_Mesh<double>>& F_plus_half, std::vector<Data_Mesh<double>>& F_minus_half, const Scheme& Method) const
	{
		RhsU::Rhs_Lax(U,F_plus_half,F_minus_half, Method);
	}
};


class Timestepper
{	//the function of this class is to just evolve the equation from t to t+dt

public:
	//Timestepper(); //constructor, might need if we need to initialize an object

	void Take_time_step_Lax_Wendroff(double& t_0, const double& dx, const double& dt, std::vector<Data_Mesh<double>>& U, const RhsComputation& RhsComp, const Scheme& Method) const
	{
		std::vector<Data_Mesh<double>> F_plus_half = U;
		std::vector<Data_Mesh<double>> F_minus_half = U;

		RhsComp.Rhs_Lax(U, F_plus_half, F_minus_half, Method);

		for (int i = 0; i < U.size(); i++)
		{
			U[i] = U[i] + (F_minus_half[i] + F_plus_half[i]*-1) * (dt/dx);
		}

		t_0 = t_0 + dt;
	}
};
// Midl1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <istream>
#include <cmath>

class Vector
{
public:

	//Vector(double x = 0, double y = 0, double z = 0) : x(x), y(y), z(z)
	//{

	//}

	Vector()
	{
		x = 0;
		y = 0;
		z = 0;
	}

	Vector(float x, float y, float z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}

	operator float()
	{
		return sqrt(x * x + y * y + z * z);
	}
	
	friend Vector operator+(const Vector& a, const Vector& b);// перегруз на сложение
	friend Vector operator*(const Vector& a, const Vector& b);// перегруз на умножение
	friend Vector operator-(const Vector& a, const Vector& b);// перегруз на разности

	friend std::ostream& operator<<(std::ostream& out, const Vector& v);
	friend std::istream& operator>>(std::istream& in, Vector& b);

	friend bool operator>(const Vector& a, const Vector& b);

	float operator[](int index)
	{
		switch ( index)
		{
		case 0:
			return x;
			break;
		case 1:
			return y;
			break;
		case 2:
			return z;
			break;
		default:
			std::cout << "index error";
			return 0;
		}
	}

private:
	
	//double x, y, z;

	float x;
	float y;
	float z;

};

bool operator>(const Vector& a, const Vector& b)
{
	return false;
}

Vector operator-(const Vector& a, const Vector& b)
{
	return Vector(a.x - b.x, a.y - b.y, a.z - b.z);
}

Vector operator*(const Vector& a, const Vector& b)
{
	return Vector(a.x * b.x, a.y * b.y, a.z * b.z);
}

Vector operator+(const Vector& a, const Vector& b)
{
	return Vector(a.x + b.x, a.y + b.y, a.z + b.z);
}

std::ostream& operator<<(std::ostream& out, const Vector& v)
{
	out <<' '<< v.x <<' '<< v.y <<' '<< v.z;
	return out;
} 

std::istream& operator>>(std::istream& in,  Vector& b)
{
	in >> b.x >> b.y >> b.z;
	return in;


}

int main()
{
	Vector v1(0,1,2);
	Vector v2(3,4,5);
	Vector v3;
	Vector v4;
	Vector v5;
	Vector v6;

	
	v3 = v1 + v2;
	v4 = v1 * v2;
	v5 = v2 - v1;


	std::cout << v3<<'\n';
	std::cout << "V3 length" << float(v3)<<'\n';
	std::cout << v4 <<'\n'; // Перешруз на умножение
	std::cout << v5 << '\n';//перегруз разности
	std::cout << "Enter vector (x,y,z)";
	if (std::cin >> v6)
	{
		std::cout << "You entered: " << v6 << '\n';
	}
	else
	{
		std::cerr << "Input error!\n";
	}
	
	return 0;
}


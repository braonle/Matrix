#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <ostream>

using namespace std;

enum ErrCodes
{
	correct, bad_ptr, zero_div, no_mem, no_type_impl, unknown_type, bad_num_format, mMismatch, mEmpty, doubleTmpUsage, mBad
};

namespace DataNS
{

	enum DataCodes
	{
		nullPtr, rCode, cCode
	};

	/**
		Common class for handling calculations. Contains DataWrap which should be used for operations.	
	*/
	class Data
	{
	public:
		/** 
			Adds an object of the same type to the current one. Should not be used directly
			\param	dt	-	pointer to an object of the same type; otherwise a ErrCodes::bad_ptr would be returned 
			\return			result of the operation
		*/
		virtual ErrCodes add(Data *dt) = 0;
		/**
			Substracts an object of the same type of the current one. Should not be used directly
			\param	dt	-	pointer to an object of the same type; otherwise a ErrCodes::bad_ptr would be returned
			\return			result of the operation
		*/
		virtual ErrCodes substract(Data *dt) = 0;
		/**
			Multiplies the current object by the one of the same type. Should not be used directly
			\param	dt	-	pointer to an object of the same type; otherwise a ErrCodes::bad_ptr would be returned
			\return			result of the operation
		*/
		virtual ErrCodes multiply(Data *dt) = 0;
		/**
			Divides the current object by the one of the same type. Should not be used directly
			\param	dt	-	pointer to an object of the same type; otherwise a ErrCodes::bad_ptr would be returned
			\return			result of the operation
		*/
		virtual ErrCodes divide(Data *dt) = 0;
		virtual long double abs(void) = 0;
		//virtual Data* sin() = 0;
		//virtual Data* cos() = 0;
		//virtual Data* sinh() = 0;
		//virtual Data* cosh() = 0;
		//virtual Data* tan() = 0;
		//virtual Data* tanh() = 0;
		//virtual Data* asin() = 0;
		//virtual Data* acos() = 0;
		//virtual Data* asinh() = 0;
		//virtual Data* acosh() = 0;
		//virtual Data* atan() = 0;
		//virtual Data* atanh() = 0;
		//virtual Data* log() = 0;
		//virtual Data* log10() = 0;
		//virtual Data* exp() = 0;
		//virtual Data* pow(Data*) = 0;
		/**
			Used to write the contents of this object to the specified output stream.
			\param	ss	-	output stream, where data are to be written
		*/
		virtual void output(ostream& ss) = 0;
		/**
			Returns the conjugated object of this one.
			\return		conjugated object
		*/
		virtual Data* conjugate() = 0;
		/**
			Clones current object and its data. The result is independent from this object.
			\return		cloned object
		*/
		virtual Data* clone() = 0;
		/**
			Used for objects' comminication. Do not use directly.
		*/
		//Returns the encapsulated data in a specific form
		virtual void* _getData() = 0;
		/**
			Determines if this object is a neutral one for addition.
		*/
		virtual bool isZero() = 0;
		virtual ~Data() {};
		/**
			Class that handles Data calculations and offers more flexibility. Designed for the direct usage.
		*/
		class DataWrap;
	private:

		//Implements type conversion for Data inheritants. Should be extended if new types and added.
		static void _argConvert(Data** arg1, Data** arg2) throw (ErrCodes);
		//Returns the code of Data inheritant. Should be extended if new types and added.
		static DataCodes _getCode(Data* code) throw (ErrCodes);
	protected:
		//Determines if the two numbers are equal with certain precision.
		static bool Data::_eq(long double num, long double data);
	};

	class Data::DataWrap
	{
		Data* _ptr;
		bool _tmpFlag, _opFlag;
	public:
		DataWrap(Data *ptr = nullptr, bool isTmp = false) :_ptr(ptr), _tmpFlag(isTmp) { _opFlag = false; };
		DataWrap(DataWrap& src);
		Data::DataWrap operator+(Data::DataWrap& param2) throw (ErrCodes);
		Data::DataWrap operator-(Data::DataWrap& deduction) throw (ErrCodes);
		Data::DataWrap operator*(Data::DataWrap& param2) throw (ErrCodes);
		Data::DataWrap operator/(Data::DataWrap& divisor) throw (ErrCodes);
		Data::DataWrap& operator+=(Data::DataWrap& param2) throw (ErrCodes);
		Data::DataWrap& operator-=(Data::DataWrap& deduction) throw (ErrCodes);
		Data::DataWrap& operator*=(Data::DataWrap& param2) throw (ErrCodes);
		Data::DataWrap& operator/=(Data::DataWrap& divisor) throw (ErrCodes);
		Data::DataWrap& operator=(Data::DataWrap& divisor) throw (ErrCodes);
		Data::DataWrap* clone();
		Data::DataWrap& set(Data* src);
		Data::DataWrap& forceDelete();
		Data::DataWrap& restrictDelete();
		bool isZero();
		string toString();
		void output(ostream& ss);
		~DataWrap();
	};

	Data::DataWrap parse(string* src);	
	ostream& operator<<(ostream& out, Data::DataWrap& src);
		
	class RealData : public Data
	{
		long double _data;
	public:
		RealData(long double dt = 0);
		virtual ErrCodes add(Data *dt);
		virtual ErrCodes substract(Data *dt);
		virtual ErrCodes multiply(Data *dt);
		virtual ErrCodes divide(Data *dt);
		//virtual Data* sin();
		//virtual Data* cos();
		//virtual Data* sinh();
		//virtual Data* cosh();
		//virtual Data* tan();
		//virtual Data* tanh();
		//virtual Data* asin();
		//virtual Data* acos();
		//virtual Data* asinh();
		//virtual Data* acosh();
		//virtual Data* atan();
		//virtual Data* atanh();
		//virtual Data* log();
		//virtual Data* log10();
		//virtual Data* exp();
		//virtual Data* pow(Data*);
		virtual void output(ostream& ss);
		virtual Data* conjugate();
		virtual long double abs(void);
		virtual void* _getData();
		virtual bool isZero();
		virtual Data* clone();
		virtual ~RealData();
	};
	class ComplexData : public Data
	{
		long double _re;
		long double _im;
	public:
		ComplexData(long double re = 0, long double im = 0);
		ComplexData(RealData* src);
		virtual ErrCodes add(Data *dt);
		virtual ErrCodes substract(Data *dt);
		virtual ErrCodes multiply(Data *dt);
		virtual ErrCodes divide(Data *dt);
		//virtual Data* sin();
		//virtual Data* cos();
		//virtual Data* sinh();
		//virtual Data* cosh();
		//virtual Data* tan();
		//virtual Data* tanh();
		//virtual Data* asin();
		//virtual Data* acos();
		//virtual Data* asinh();
		//virtual Data* acosh();
		//virtual Data* atan();
		//virtual Data* atanh();
		//virtual Data* log();
		//virtual Data* log10();
		//virtual Data* exp();
		//virtual Data* pow(Data*);
		virtual Data* conjugate();
		virtual void output(ostream& ss);
		virtual long double abs(void);
		virtual void* _getData();
		virtual bool isZero();
		virtual Data* clone();
		virtual ~ComplexData();
	};

}

namespace MatrixNS
{
	class Matrix
	{
		int _width, _height;
		DataNS::Data::DataWrap** _table;
	public:
		Matrix(DataNS::Data::DataWrap** input, int width = 1, int height = 1);
		Matrix(Matrix& m);
		void add(Matrix& input) throw (ErrCodes);
		void substract(Matrix& input) throw (ErrCodes);
		void multiply(DataNS::Data::DataWrap& input) throw (ErrCodes);
		void multiply(DataNS::Data::DataWrap* input) throw (ErrCodes);
		void multiplyL(Matrix& input) throw (ErrCodes);
		void multiplyR(Matrix& input) throw (ErrCodes);
		Matrix* inverse() throw (ErrCodes);
		DataNS::Data::DataWrap determinant() throw (ErrCodes);
		DataNS::Data::DataWrap trace() throw (ErrCodes);
		Matrix* transpose();
		Matrix* subMatrix(int wStart, int lStart, int wEnd, int lEnd);
		bool isZero();
		int isDefinitive();
		int isSymmetric();
		int isOrthogonal();
		bool equals(Matrix& src);
		bool isSquare();
		void output(ostream& ss);
		friend long long timetest(Matrix& A, Matrix& B);
		~Matrix();
	private:
		struct ThreadInfo
		{
			ThreadInfo(Matrix *tg, Matrix * sr, int s) : src(sr), target(tg), seq(s){};
			Matrix *target, *src;
			int seq;
		};
		static const int _threadNumber = 4;

		void _simpleAdd(Matrix& input) throw (ErrCodes);
		static DWORD WINAPI _threadAdd(LPVOID data);
		void _simpleSubstract(Matrix& input) throw (ErrCodes);
		static DWORD WINAPI _threadSubstract(LPVOID);
	};

	ostream& operator<< (ostream& ss, Matrix& m);
	
	long long timetest(Matrix& A, Matrix& B);
	string*** separ(vector < string *>, int width, int length);
}
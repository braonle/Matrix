#pragma once
#include <string>
#include <vector>
#include <Windows.h>
#include <ostream>

using namespace std;

enum ErrCodes
{
	correct, bad_ptr, zero_div, no_mem, no_type_impl, unknown_type, bad_num_format, mMismatch, doubleTmpUsage
};

namespace DataNS
{

	enum DataCodes
	{
		nullPtr, rCode, cCode
	};

	class Data
	{
	public:
		/** 
			Adds an object of the same type to the current one. Should not be used directly
			\param	dt	-	pointer to an object of the same type; otherwise a ErrCodes::bad_ptr would be returned 
			\return			result of the operation
		*/
		virtual ErrCodes add(Data *dt) = 0;
		virtual ErrCodes substract(Data *dt) = 0;
		virtual ErrCodes multiply(Data *dt) = 0;
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
		virtual void output(ostream& ss) = 0;
		virtual Data* conjugate() = 0;
		virtual Data* clone() = 0;
		virtual void* _getData() = 0;
		virtual bool isZero() = 0;
		virtual ~Data() {};
		class DataWrap;
	private:
		static void _argConvert(Data** arg1, Data** arg2) throw (ErrCodes);
		static DataCodes _getCode(Data* code) throw (ErrCodes);
	protected:
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
		friend ostream& operator<<(ostream& out, Data::DataWrap& src);
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
		void add(Matrix& input) throw (ErrCodes);
		void substract(Matrix& input) throw (ErrCodes);
		void multiply(Matrix& input) throw (ErrCodes);
		Matrix* inverse() throw (ErrCodes);
		DataNS::Data::DataWrap* determinant() throw (ErrCodes);
		DataNS::Data::DataWrap* trace() throw (ErrCodes);
		ErrCodes scalarMultiply(DataNS::Data* num);
		Matrix* transpose();
		Matrix* subMatrix(int wStart, int lStart, int wEnd, int lEnd);
		bool isZero();
		int isDefinitive();
		int isSymmetric();
		int isOrthogonal();
		int isSquare();
		~Matrix();
	protected:
		struct TMP
		{
			TMP(Matrix *tg, Matrix * sr, int s) : src(sr), target(tg), seq(s){};
			Matrix *target, *src;
			int seq;
		};
		
		static DWORD WINAPI _threadSubstract(LPVOID);
	};

	string*** separ(vector < string *>, int width, int length);
}
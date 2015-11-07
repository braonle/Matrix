#include "Matrix.h"
#include <sstream>
//#include "Common.h"

using namespace MatrixNS;
using namespace DataNS;

Data::DataWrap::~DataWrap()
{
	if (!_tmpFlag) delete _ptr;
}
string Data::DataWrap::toString()
{
	stringstream ss;
	this->_ptr->output(ss);
	return ss.str();
}
bool Data::_eq(long double num, long double data)
{
	const long double e = 0.00000000000001;
	if (::abs(data - num) < e)
	{
		return true;
	}
	else return false;
}

//Переписать с учётом общих исключений
/*Data::DataWrap DataNS::parse(string* source)
{
	Data::DataWrap wr((Drobot::drive(source))->getData());
	delete source;
	return wr;	
}*/
DataCodes Data::_getCode(Data* code)
{
	if (NULL == code) return nullPtr;
	if (dynamic_cast<RealData*>(code)) return rCode;
	if (dynamic_cast<ComplexData*>(code)) return cCode;

	throw unknown_type;
}
void Data::_argConvert(Data** arg1, Data** arg2)
{
	long double *tmp;

	switch (_getCode(*arg1))
	{
	case rCode:
		switch (_getCode(*arg2))
		{
		case rCode:
			*arg1 = new RealData((RealData&)**arg1);
			break;

		case cCode:
			tmp = (long double*) (*arg2)->_getData();
			if (Data::_eq(tmp[1], 0))
			{
				*arg2 = new RealData(tmp[0]);
				*arg1 = new RealData((RealData&)**arg1);
			}
			else
			{
				*arg1 = new ComplexData((RealData*)*arg1);
			}
			delete[] tmp;
			break;

		default:
			throw no_type_impl;
			break;
		}
		break;

	case cCode:
		switch (_getCode(*arg2))
		{
		case rCode:

			tmp = (long double*)(*arg1)->_getData();
			if (Data::_eq(tmp[1], 0))
			{
				*arg1 = new RealData((long double)tmp[0]);
			}
			else
			{
				*arg2 = new ComplexData((RealData*)*arg2);
				*arg1 = new ComplexData((ComplexData&)**arg1);
			}
			delete[] tmp;
			break;

			break;

		case cCode:
			*arg1 = new ComplexData((ComplexData&)**arg1);
			break;

		default:
			throw no_type_impl;
			break;
		}
		break;

	default:
		throw no_type_impl;
		break;
	}
	if (*arg1 == NULL || *arg2 == NULL) throw no_mem;
}
Data::DataWrap::DataWrap(Data::DataWrap& src)
{
	if (src._ptr == nullptr)
	{
		throw doubleTmpUsage;
	}

	if (src._tmpFlag)
	{
		this->_ptr = src._ptr;
		src._ptr = nullptr;
	}
	else
	{
		this->_ptr = src._ptr->clone();
	}
	if (src._opFlag) _tmpFlag = true;
	else _tmpFlag = false;

	_opFlag = false;
}
Data::DataWrap Data::DataWrap::operator+(Data::DataWrap& param2)
{
	Data *arg1 = this->_ptr;
	Data *arg2 = param2._ptr;
	Data::_argConvert(&arg1, &arg2);

	ErrCodes err = arg1->add(arg2);
	if (err != correct) throw err;
	DataWrap wr(arg1);
	wr._tmpFlag = true;
	wr._opFlag = true;
	if (arg2 != param2._ptr) delete arg2;
	param2._tmpFlag = false;
	this->_tmpFlag = false;
	return wr;
}
Data::DataWrap Data::DataWrap::operator-(Data::DataWrap& deduction)
{
	Data *arg1 = this->_ptr;
	Data *arg2 = deduction._ptr;
	Data::_argConvert(&arg1, &arg2);

	ErrCodes err = arg1->substract(arg2);
	if (err != correct) throw err;
	DataWrap wr(arg1);
	wr._tmpFlag = true;
	wr._opFlag = true;
	if (arg2 != deduction._ptr) delete arg2;
	deduction._tmpFlag = false;
	this->_tmpFlag = false;
	return wr;
}
Data::DataWrap Data::DataWrap::operator*(Data::DataWrap& param2)
{
	Data *arg1 = this->_ptr;
	Data *arg2 = param2._ptr;
	Data::_argConvert(&arg1, &arg2);

	ErrCodes err = arg1->multiply(arg2);
	if (err != correct) throw err;
	DataWrap wr(arg1);
	wr._tmpFlag = true;
	wr._opFlag = true;
	if (arg2 != param2._ptr) delete arg2;
	param2._tmpFlag = false;
	this->_tmpFlag = false;
	return wr;
}
Data::DataWrap Data::DataWrap::operator/(Data::DataWrap& divisor)
{
	Data *arg1 = this->_ptr;
	Data *arg2 = divisor._ptr;
	Data::_argConvert(&arg1, &arg2);

	ErrCodes err = arg1->divide(arg2);
	if (err != correct) throw err;
	DataWrap wr(arg1);
	wr._tmpFlag = true;
	wr._opFlag = true;
	if (arg2 != divisor._ptr) delete arg2;
	divisor._tmpFlag = false;
	this->_tmpFlag = false;
	return wr;
}
Data::DataWrap& Data::DataWrap::operator+=(Data::DataWrap & param) throw(ErrCodes)
{
	Data *arg1 = this->_ptr;
	Data *arg2 = param._ptr;
	Data::_argConvert(&arg1, &arg2);

	ErrCodes err = arg1->add(arg2);
	if (err != correct) throw err;

	if (this->_ptr != arg1)
	{
		delete this->_ptr;
		this->_ptr = arg1;
	}
	this->_tmpFlag = false;
	if (arg2 != param._ptr) delete arg2;
	param._tmpFlag = false;
	this->_tmpFlag = false;
	return *this;
}
Data::DataWrap& Data::DataWrap::operator-=(Data::DataWrap & deduction) throw(ErrCodes)
{
	Data *arg1 = this->_ptr;
	Data *arg2 = deduction._ptr;
	Data::_argConvert(&arg1, &arg2);

	ErrCodes err = arg1->substract(arg2);
	if (err != correct) throw err;

	if (this->_ptr != arg1)
	{
		delete this->_ptr;
		this->_ptr = arg1;
	}
	this->_tmpFlag = false;
	if (arg2 != deduction._ptr) delete arg2;
	deduction._tmpFlag = false;
	this->_tmpFlag = false;
	return *this;
}
Data::DataWrap& Data::DataWrap::operator*=(Data::DataWrap & param) throw(ErrCodes)
{
	Data *arg1 = this->_ptr;
	Data *arg2 = param._ptr;
	Data::_argConvert(&arg1, &arg2);

	ErrCodes err = arg1->multiply(arg2);
	if (err != correct) throw err;

	if (this->_ptr != arg1)
	{
		delete this->_ptr;
		this->_ptr = arg1;
	}
	this->_tmpFlag = false;
	if (arg2 != param._ptr) delete arg2;
	param._tmpFlag = false;
	this->_tmpFlag = false;
	return *this;
}
Data::DataWrap& Data::DataWrap::operator/=(Data::DataWrap & divisor) throw(ErrCodes)
{
	Data *arg1 = this->_ptr;
	Data *arg2 = divisor._ptr;
	Data::_argConvert(&arg1, &arg2);

	ErrCodes err = arg1->divide(arg2);
	if (err != correct) throw err;

	if (this->_ptr != arg1)
	{
		delete this->_ptr;
		this->_ptr = arg1;
	}
	this->_tmpFlag = false;
	if (arg2 != divisor._ptr) delete arg2;
	divisor._tmpFlag = false;
	this->_tmpFlag = false;
	return *this;
}
Data::DataWrap& Data::DataWrap::operator=(Data::DataWrap& src) throw(ErrCodes)
{
	if (src._ptr == nullptr)
	{
		throw doubleTmpUsage;
	}

	if (this->_ptr != NULL) delete this->_ptr;

	if (src._tmpFlag)
	{
		this->_ptr = src._ptr;
		src._ptr = nullptr;
	}
	else
	{
		this->_ptr = src._ptr->clone();
	}
	this->_tmpFlag = false;
	return *this;
}
Data::DataWrap * Data::DataWrap::clone()
{
	return new Data::DataWrap(_ptr->clone());
}
Data::DataWrap& Data::DataWrap::set(Data * src)
{
	if (_ptr != nullptr) delete _ptr;
	_ptr = src;
	return *this;
}
Data::DataWrap& Data::DataWrap::forceDelete()
{
	this->_tmpFlag = FALSE;
	return *this;
}
Data::DataWrap & Data::DataWrap::restrictDelete()
{
	this->_tmpFlag = TRUE;
	return *this;
}
bool Data::DataWrap::isZero()
{
	return _ptr->isZero();
}
void Data::DataWrap::output(ostream& ss)
{
	this->_ptr->output(ss);
}
ostream& DataNS::operator<<(ostream& out, Data::DataWrap& src)
{
	src.output(out);
	return out;
}


RealData::RealData(long double data)
{
	this->_data = data;
}
ErrCodes RealData::add(Data *dt)
{
	RealData *ptr;
	if (!(ptr = dynamic_cast<RealData*>(dt)))
	{
		return bad_ptr;
	}
	_data += ptr->_data;
	return correct;
}
ErrCodes RealData::substract(Data *dt)
{
	RealData *ptr;
	if (!(ptr = dynamic_cast<RealData*>(dt)))
	{
		return bad_ptr;
	}
	_data -= ptr->_data;
	return correct;
}
ErrCodes RealData::multiply(Data *dt)
{
	RealData *ptr;
	if (!(ptr = dynamic_cast<RealData*>(dt)))
	{
		return bad_ptr;
	}
	_data *= ptr->_data;
	return correct;
}
ErrCodes RealData::divide(Data *dt)
{
	RealData *ptr;
	if (!(ptr = dynamic_cast<RealData*>(dt)))
	{
		return bad_ptr;
	}
	if (ptr->_eq(0, ptr->_data))
	{
		return zero_div;
	}
	_data /= ptr->_data;
	return correct;
}
void RealData::output(ostream& ss)
{
	ss << _data;
}
Data * DataNS::RealData::conjugate()
{
	return new DataNS::RealData(*this);
}
long double RealData::abs(void)
{
	return ::abs(_data);
}
void* RealData::_getData()
{
	return new long double(this->_data);
}
bool DataNS::RealData::isZero()
{
	return _eq(0, _data);
}
Data * DataNS::RealData::clone()
{
	return new DataNS::RealData(*this);
}
RealData::~RealData() {}


ComplexData::ComplexData(long double re, long double im)
{
	this->_re = re;
	this->_im = im;
}
ComplexData::ComplexData(RealData* src)
{
	long double *ptr = (long double*) src->_getData();
	this->_re = *ptr;
	this->_im = 0;
	delete ptr;
}
ErrCodes ComplexData::add(Data * dt)
{
	ComplexData *ptr;
	if (!(ptr = dynamic_cast<ComplexData*>(dt)))
	{
		return bad_ptr;
	}
	this->_re += ptr->_re;
	this->_im += ptr->_im;
	return correct;
}
ErrCodes ComplexData::substract(Data * dt)
{
	ComplexData *ptr;
	if (!(ptr = dynamic_cast<ComplexData*>(dt)))
	{
		return bad_ptr;
	}
	this->_re -= ptr->_re;
	this->_im -= ptr->_im;
	return correct;
}
ErrCodes ComplexData::multiply(Data * dt)
{
	ComplexData *ptr;
	if (!(ptr = dynamic_cast<ComplexData*>(dt)))
	{
		return bad_ptr;
	}
	long double tmp = this->_re * ptr->_re - this->_im * ptr->_im;
	this->_im = this->_re * ptr->_im + this->_im * ptr->_re;
	this->_re = tmp;
	return correct;
}
ErrCodes ComplexData::divide(Data * dt)
{
	ComplexData *ptr;
	if (!(ptr = dynamic_cast<ComplexData*>(dt)))
	{
		return bad_ptr;
	}
	if (_eq(ptr->_re, 0) && _eq(ptr->_im, 0))
	{
		return zero_div;
	}
	
	ptr = (ComplexData*)ptr->conjugate();
	this->multiply(ptr);
	long double divisor = ptr->_re * ptr->_re + ptr->_im * ptr->_im;
	delete ptr;
	this->_re /= divisor;
	this->_im /= divisor;

	return correct;
}
Data* ComplexData::conjugate()
{
	return new ComplexData(this->_re, -(this->_im));
}
void ComplexData::output(ostream& ss)
{
	
	if (_eq(_im, 0))
	{
		ss << _re;
	}
	else if (_eq(_re, 0))
	{
		if (!_eq(1, ::abs(_im)))
		{
			ss << _im << "i";
		} 
		else
		{
			if (_eq(_im, 1))
			{
				ss << "i";
			}
			else
			{
				ss << "-i";
			}
		}		
	} 
	else
	{
		ss << _re;
		if (_im > 0)
		{
			ss << "+";
		}
		if (!_eq(1, ::abs(_im)))
		{
			ss << _im << "i";
		}
		else
		{
			if (_eq(_im, 1))
			{
				ss << "i";
			}
			else
			{
				ss << "-i";
			}
		}
	}
}
long double ComplexData::abs(void)
{
	return sqrt(_re*_re + _im*_im);
}
void* ComplexData::_getData()
{
	long double *ptr = new long double[2];
	ptr[0] = this->_re;
	ptr[1] = this->_im;	
	return ptr;
}
bool ComplexData::isZero()
{
	return _eq(_re, 0) && _eq(_im, 0);
}
ComplexData::~ComplexData() {}
Data * ComplexData::clone()
{
	return new DataNS::ComplexData(*this);
}

Matrix::Matrix(Data::DataWrap ** input, int width, int height)
{
	this->_table = input;
	this->_width = width;
	this->_height = height;
}
Matrix::Matrix(Matrix & m)
{
	_width = m._width;
	_height = m._height;
	_table = new DataNS::Data::DataWrap*[_width];
	for (int i = 0; i < _width; i++)
	{
		_table[i] = new DataNS::Data::DataWrap[_height];
		for (int j = 0; j < _height; j++)
		{
			_table[i][j] = m._table[i][j];
		}
	}
}
Matrix::~Matrix()
{
	for (int i = 0; i < _width; i++)
	{
		delete[] _table[i];
	}
	delete[] _table;
}
void Matrix::add(Matrix & input) throw(ErrCodes)
{
	const int tNumToStart = 40;

	if (_width != input._width || _height != input._height) throw mMismatch;
	if (_table == nullptr || input._table == nullptr) throw mEmpty;
	
	if (_width < tNumToStart) _simpleAdd(input);
	else
	{
		ThreadInfo* info;
		HANDLE threads[_threadNumber];
		for (int i = 0; i < _threadNumber; i++)
		{
			info = new ThreadInfo(this, &input, i);
			threads[i] = CreateThread(NULL, 0, _threadAdd, info, 0, NULL);
		}
		WaitForMultipleObjects(_threadNumber, threads, true, INFINITE);
		for (int i = 0; i < _threadNumber; i++) CloseHandle(threads[i]);
	} 
}
void Matrix::_simpleAdd(Matrix& input)
{
	for (int i = 0; i < _width; i++)
		for (int j = 0; j < _height; j++)
		{
			this->_table[i][j] += input._table[i][j];
		}
	return;
}
DWORD Matrix::_threadAdd(LPVOID data)
{
	ThreadInfo* dt = (ThreadInfo*)data;

	for (int i = dt->seq; i < dt->target->_width; i += _threadNumber)
		for (int j = 0; j < dt->target->_height; j++)
		{
			dt->target->_table[i][j] += dt->src->_table[i][j];
		}
	dt->target = dt->src = nullptr;
	delete dt;
	return 0;
}
void Matrix::substract(Matrix & input) throw(ErrCodes)
{
	const int tNumToStart = 40;

	if (_width != input._width || _height != input._height) throw mMismatch;
	if (_table == nullptr || input._table == nullptr) throw mEmpty;

	if (_width < tNumToStart) _simpleSubstract(input);
	else
	{
		ThreadInfo* info;
		HANDLE threads[_threadNumber];
		for (int i = 0; i < _threadNumber; i++)
		{
			info = new ThreadInfo(this, &input, i);
			threads[i] = CreateThread(NULL, 0, _threadSubstract, info, 0, NULL);
		}
		WaitForMultipleObjects(_threadNumber, threads, true, INFINITE);
		for (int i = 0; i < _threadNumber; i++) CloseHandle(threads[i]);
	}
}
void Matrix::_simpleSubstract(Matrix& input)
{
	for (int i = 0; i < _width; i++)
		for (int j = 0; j < _height; j++)
		{
			this->_table[i][j] -= input._table[i][j];
		}
	return;
}
DWORD Matrix::_threadSubstract(LPVOID data)
{
	ThreadInfo* dt = (ThreadInfo*)data;

	for (int i = dt->seq; i < dt->target->_width; i += _threadNumber)
		for (int j = 0; j < dt->target->_height; j++)
		{
			dt->target->_table[i][j] -= dt->src->_table[i][j];
		}
	dt->target = dt->src = nullptr;
	delete dt;
	return 0;
}
void Matrix::multiply(Data::DataWrap& input)
{
	for (int i = 0; i < _width; i++)
		for (int j = 0; j < _height; j++)
			_table[i][j] *= input;
			
}

void Matrix::multiply(Data::DataWrap* input)
{
	multiply(*input);
	delete input;
}
int Matrix::isSymmetric()
{
	if (!isSquare()) return -1;

	for (int i = 0; i < _height; i++)
		for (int j = i + 1; j < _width; j++)
			if (!(_table[i][j] - _table[j][i]).isZero())
				return 1;

	return 0;
}
Data::DataWrap Matrix::trace()
{
	if (!isSquare()) throw mBad;

	Data::DataWrap res = _table[0][0];
	for (int i = 1; i < _width; i++)
		res *= _table[i][i];

	res.restrictDelete();
	return res;
}
bool Matrix::isZero()
{
	bool res = true;
	try
	{
		//res = determinant().isZero();
	}
	catch (ErrCodes)
	{
		res = false;
	}
	return res;
}
bool Matrix::equals(Matrix& src)
{
	if (_width != src._width || _height != src._height) 
		return false;

	for (int i = 0; i < _width; i++)
		for (int j = 0; j < _height; j++)
			if (!(_table[i][j] - src._table[i][j]).forceDelete().isZero()) 
				return false;

	return true;
}
bool Matrix::isSquare()
{
	return _width == _height;
}
void Matrix::output(ostream& ss)
{
	const char separator = ' ';
	for (int i = 0; i < _height; i++)
	{
		for (int j = 0; j < _width; j++)
		{
			ss << _table[i][j];
			if (j != _width - 1) ss << separator;
		}
		ss << endl;
	}
}
ostream& MatrixNS::operator<<(ostream& ss, Matrix& m)
{
	m.output(ss);
	return ss;
}




long long MatrixNS::timetest(Matrix& A, Matrix& B)
{
	int init0 = __rdtsc();
	A._simpleAdd(B);
	init0 -= __rdtsc();

	int init1 = __rdtsc();
	Matrix::ThreadInfo* info;
	HANDLE threads[Matrix::_threadNumber];
	for (int i = 0; i < Matrix::_threadNumber; i++)
	{
		info = new Matrix::ThreadInfo(&A, &B, i);
		threads[i] = CreateThread(NULL, 0, Matrix::_threadAdd, info, 0, NULL);
	}
	WaitForMultipleObjects(Matrix::_threadNumber, threads, true, INFINITE);
	for (int i = 0; i < Matrix::_threadNumber; i++) CloseHandle(threads[i]);
	init1 -= __rdtsc();

	return init0 - init1;
}

/*DataNS::Data*** MatrixNS::Matrix::parse(string*** str, int width, int length)
{
	DataNS::Data*** mas = new DataNS::Data**[width];
	for (int i = 0; i < width; ++i)
	{
		mas[i] = new DataNS::Data*[length];
		for (int j = 0; j < length; ++j)
		{
			try
			{
				mas[i][j] = DataNS::parse(str[i][j]);
			}
			catch (...)
			{
				//Можем потерять прокидываемый указатель
				for (int k = 0; k < i; ++k)
				{
					for (int l = 0; l < length; ++l)
						delete mas[k][l];
					delete mas[k];
				}
				for (int l = 0; l < j; ++l)
					delete mas[i][l];
				delete mas;
				throw bad_num_format;
			}
		}
	}
	return mas;
}*/


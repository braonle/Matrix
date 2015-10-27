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
Data::DataWrap& DataNS::Data::DataWrap::operator+=(Data::DataWrap & param) throw(ErrCodes)
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
Data::DataWrap& DataNS::Data::DataWrap::operator-=(Data::DataWrap & deduction) throw(ErrCodes)
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
Data::DataWrap& DataNS::Data::DataWrap::operator*=(Data::DataWrap & param) throw(ErrCodes)
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
Data::DataWrap& DataNS::Data::DataWrap::operator/=(Data::DataWrap & divisor) throw(ErrCodes)
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
Data::DataWrap * DataNS::Data::DataWrap::clone()
{
	return new Data::DataWrap(_ptr->clone());
}
Data::DataWrap& DataNS::Data::DataWrap::set(Data * src)
{
	if (_ptr != nullptr) delete _ptr;
	_ptr = src;
	return *this;
}
Data::DataWrap& DataNS::Data::DataWrap::forceDelete()
{
	this->_tmpFlag = FALSE;
	return *this;
}
Data::DataWrap & DataNS::Data::DataWrap::restrictDelete()
{
	this->_tmpFlag = TRUE;
	return *this;
}
bool DataNS::Data::DataWrap::isZero()
{
	return _ptr->isZero();
}
ostream& DataNS::operator<<(ostream& out, Data::DataWrap& src)
{
	src._ptr->output(out);
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
ErrCodes DataNS::ComplexData::add(Data * dt)
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
ErrCodes DataNS::ComplexData::substract(Data * dt)
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
ErrCodes DataNS::ComplexData::multiply(Data * dt)
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
ErrCodes DataNS::ComplexData::divide(Data * dt)
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
Data* DataNS::ComplexData::conjugate()
{
	return new ComplexData(this->_re, -(this->_im));
}
void DataNS::ComplexData::output(ostream& ss)
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
long double DataNS::ComplexData::abs(void)
{
	return sqrt(_re*_re + _im*_im);
}
void* DataNS::ComplexData::_getData()
{
	long double *ptr = new long double[2];
	ptr[0] = this->_re;
	ptr[1] = this->_im;	
	return ptr;
}
bool DataNS::ComplexData::isZero()
{
	return _eq(_re, 0) && _eq(_im, 0);
}
DataNS::ComplexData::~ComplexData() {}
Data * DataNS::ComplexData::clone()
{
	return new DataNS::ComplexData(*this);
}

MatrixNS::Matrix::Matrix(DataNS::Data::DataWrap ** input, int width, int height)
{
	this->_table = input;
	this->_width = width;
	this->_height = height;
}
MatrixNS::Matrix::~Matrix()
{
	for (int i = 0; i < _width; i++)
	{
		delete[] _table[i];
	}
	delete[] _table;
}

void MatrixNS::Matrix::add(Matrix & input) throw(ErrCodes)
{
	if (_width != input._width || _height != input._height) throw mMismatch;

	for (int i = 0; i < _width; i++)
		for (int j = 0; j < _height; j++)
		{
			
		}
	return;
}

void MatrixNS::Matrix::substract(Matrix & input) throw(ErrCodes)
{
	if (_width != input._width || _height != input._height) throw mMismatch;

	HANDLE *threads = new HANDLE[_width];
	TMP* str;

	for (int i = 0; i < _width; i++)
	{
		str = new TMP(this, &input, i);
		threads[i] = CreateThread(NULL, 0, &_threadSubstract, str, 0, NULL);
	}
	WaitForMultipleObjects(_width, threads, true, INFINITE);
	
	for (int i = 0; i < _width; i++)
	{
		CloseHandle(threads[i]);
	}
	delete[] threads;
}

DWORD MatrixNS::Matrix::_threadSubstract(LPVOID data)
{
	TMP* dt = (TMP*)data;

	for (int j = 0; j < dt->target->_height; j++)
	{
		dt->target->_table[dt->seq][j] += dt->src->_table[dt->seq][j];
	}

	dt->target = dt->src = nullptr;
	delete dt;
	return 0;
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


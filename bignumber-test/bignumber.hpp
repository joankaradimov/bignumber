#pragma once

#include <stdlib.h>
#include <iostream>

#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define oldest(a) ( !!(a&(1<<(sizeof(a)*8-1))) )

#define MIN_SHORT_VAL 0x8000
#define MAX_SHORT_VAL 0x7fff

typedef unsigned __int16 hword;
typedef unsigned __int32 word;

union _word
{
  word whole;
  struct
  {
    hword second_half;
    hword first_half;
  };
};

class BigInteger
{
public:
    BigInteger() {
        size = 1;
        buff = (hword*)malloc(sizeof(hword));
        *buff = 0;
    }

    BigInteger(int num) {
        _word w;
        w.whole = num;
        size = sizeof(word) / sizeof(hword);
        buff = (hword*)malloc(size * sizeof(hword));
        buff[0] = w.second_half;
        buff[1] = w.first_half;
        trim();
    }

    BigInteger(unsigned long num) {
        _word w;
        w.whole = num;
        size = 1 + sizeof(word) / sizeof(hword);
        buff = (hword*)malloc(size * sizeof(hword));
        buff[0] = w.second_half;
        buff[1] = w.first_half;
        buff[2] = 0;
        trim();
    }

    BigInteger(const char* str) {
        BigInteger t;
        t = str;
        size = t.size;
        buff = (hword*)malloc(size * sizeof(hword));
        for (int i = 0; i < size; ++i) buff[i] = t.buff[i];
    }

    BigInteger(const BigInteger& lnum) {
        int i;
        size = lnum.size;
        buff = (hword*)malloc(size * sizeof(hword));
        for (i = 0; i < size; ++i) buff[i] = lnum.buff[i];
        trim();
    }

    ~BigInteger() {
        free(buff);
    }

    BigInteger operator~() const {
        BigInteger res(*this);
        for (int i = 0; i < res.size; ++i)
            res.buff[i] = ~res.buff[i];
        return res;
    }

    BigInteger operator-() const {
        BigInteger res(*this);
        --res;
        return ~res;
    }

    const BigInteger& operator+ () const {
        return (*this);
    }

    BigInteger operator+(const BigInteger& r) const {
        int i, m = max(this->size, r.size);
        int s = this->sign() != r.sign();
        BigInteger res;
        res.SetSize(m + 1 - s);
        _word t;
        for (i = 0; i < m - s; ++i) {
            t.whole = (*this)[i] + r[i] + res[i];
            res.buff[i] = t.second_half;
            res.buff[i + 1] = t.first_half;
        }
        t.whole = (*this)[i] + r[i] + res[i];
        res.buff[i] = t.second_half;
        return res;
    }

    BigInteger operator+(int r) const {
        return *this + BigInteger(r); // TODO: optimize
    }

    BigInteger operator-(const BigInteger& r) const {
        return (*this) + (-r);
    }

    BigInteger operator-(int r) const {
        return *this - BigInteger(r); // TODO: optimize
    }

    BigInteger operator*(const BigInteger& lnum) const {
        BigInteger l, r, res;
        sign() ? l = -(*this) : l = (*this);
        lnum.sign() ? r = -lnum : r = lnum;
        int s = sign() ^ lnum.sign();
        for (int i = 0; i < r.size; i++) {
            res = res + ((l * r[i]) << (i * sizeof(hword) * 8));
        }
        return s ? -res : res;
    }

    BigInteger operator*(int r) const {
        return (*this) * BigInteger(r);
    }

    BigInteger operator*(hword r) const {
        BigInteger res, l_positive, t;
        bool s = sign();
        if (s) l_positive = -(*this);
        else l_positive = (*this);
        res.SetSize(l_positive.size + 1);
        for (int i = 0; i < l_positive.size; i++) {
            t = (unsigned)(l_positive[i] * r);
            res = res + (t << (i * sizeof(hword) * 8));
        }
        return s ? -res : res;
    }

    BigInteger operator/(const BigInteger& lnum) const {
        // TODO: can this be optimized
        BigInteger l, r, res = 0;
        sign() ? l = -(*this) : l = (*this);
        lnum.sign() ? r = -lnum : r = lnum;
        int s = sign() ^ lnum.sign();

        int i;
        BigInteger j, t;
        while (l >= r) {
            t = r;
            for (i = 0; l >= t; i++) t = t << (sizeof(hword) * 8);
            t = t >> (sizeof(hword) * 8);
            for (j = 0; l >= t; j++) l = l - t;
            res = res + (j << ((i - 1) * sizeof(hword) * 8));
        }

        /*while (l>=0)
        {
          ++res;
          l=l-r;
        }
        --res;*/

        return s ? -res : res;
    }

    BigInteger operator/(int r) const {
        return (*this) / BigInteger(r);
    }

    BigInteger operator/(hword r) const {
        int s = sign();
        BigInteger res, l;
        s ? l = -(*this) : l = (*this);
        _word t;
        res.SetSize(l.size);
        for (int i = l.size; i > 0; --i)
        {
            t.first_half = l[i];
            t.second_half = l[i - 1];
            res.buff[i - 1] = (hword)(t.whole / r);
            l.buff[i - 1] = (hword)(t.whole % r);
        }
        return s ? -res : res;
    }

    BigInteger operator%(const BigInteger& r) const {
        return (*this) - (((*this) / r) * r);
    }

    BigInteger operator%(int r) const {
        return (*this) % BigInteger(r);
    }

    BigInteger operator%(hword r) const {
        // TODO: optimize -- implement a divmod method and use it here
        return (*this) - (((*this) / r) * r);
    }

  friend BigInteger operator+ (int, const BigInteger&);
  friend BigInteger operator- (int, const BigInteger&);
  friend BigInteger operator* (int, const BigInteger&);

  int operator < (const BigInteger&) const;
  int operator > (const BigInteger&) const;
  int operator <= (const BigInteger&) const;
  int operator >= (const BigInteger&) const;
  int operator == (const BigInteger&) const;
  int operator != (const BigInteger&) const;

    const BigInteger& operator=(const BigInteger& lnum) {
        if (this != &lnum) {
            size = lnum.size;
            free(buff);
            buff = (hword*)malloc(size * sizeof(hword));
            for (int i = 0; i < size; ++i) buff[i] = lnum.buff[i];
        }
        trim();
        return *this;
    }

    const BigInteger& BigInteger::operator=(int num) {
        _word w;
        w.whole = num;
        size = 2;
        free(buff);
        buff = (hword*)malloc(size * sizeof(hword));
        buff[0] = w.second_half;
        buff[1] = w.first_half;
        trim();
        return *this;
    }

    const BigInteger& BigInteger::operator= (unsigned num) {
        _word w;
        w.whole = num;
        size = 1 + sizeof(word) / sizeof(hword);
        free(buff);
        buff = (hword*)malloc(size * sizeof(hword));
        buff[0] = w.second_half;
        buff[1] = w.first_half;
        buff[2] = 0;
        trim();
        return *this;
    }

    const BigInteger& BigInteger::operator= (const char* str) {
        BigInteger mult = 1;
        int s = 0, len, val;
        (*this) = 0;

        if (*str == '+') {
            ++str;
            s = 0;
        }
        else if (*str == '-') {
            s = 1;
            ++str;
        }
        for (len = 0; str[len]; ++len);
        for (int i = len - 1; i >= 0; i--) {
            if (str[i] < '0' || str[i]>'9') break;
            val = str[i] - '0';
            (*this) = (*this) + val * mult;
            mult = mult * IO_BASE;
        }
        *this = s ? -(*this) : (*this);
        trim();
        return *this;
    }

  hword operator[] (int) const;
  explicit operator int() const;

  BigInteger operator++ (int);
  BigInteger& operator++ ();
  BigInteger operator-- (int);
  BigInteger& operator-- ();

  BigInteger operator<< (unsigned shift) const;
  BigInteger operator>> (unsigned shift) const;
  const BigInteger operator<<= (unsigned shift);
  const BigInteger operator>>= (unsigned shift);

  friend std::ostream& operator<<(std::ostream&, const BigInteger&);
  friend std::istream& operator>>(std::istream&, BigInteger&);

  void printbin() const;
  void printhex() const;

  static const hword IO_BASE=10;
private:

  void SetSize(int new_size);
  void trim();
  bool sign() const;

  hword* buff;
  int size;
};

BigInteger operator+ (int l, const BigInteger& r)
{
  return r+l;
}

BigInteger operator- (int l, const BigInteger& r)
{
  return (-r)+l;
}

BigInteger operator* (int l, const BigInteger& r)
{
  return r*l;
}

int BigInteger::operator< (const BigInteger& r) const
{
  BigInteger temp=(*this)-r;
  return temp.sign();
}

int BigInteger::operator> (const BigInteger& r) const
{
  BigInteger temp=r-(*this);
  return temp.sign();
}

int BigInteger::operator<= (const BigInteger& r) const
{
  return !( (*this)>r );
}

int BigInteger::operator>= (const BigInteger& r) const
{
  return !( (*this)<r );
}

int BigInteger::operator== (const BigInteger& r) const
{
  BigInteger temp=(*this)-r;
  for (int i=0; i<temp.size; ++i) if (temp.buff[i]) return 0;
  return 1;
}

int BigInteger::operator!= (const BigInteger& r) const
{
  return !( (*this)==r );
}

hword BigInteger::operator[] (int pos) const
{
  if (pos<0) return 0;
  if (pos>=size) return oldest(this->buff[size-1]) ?~0:0;
  return this->buff[pos];
}

BigInteger::operator int() const
{
  _word t;
  if ( size>2 ) // Ако не се събира в int
  {
    if ( sign() ) // Aко е отрицателно и не се събира в int
    {
      t.first_half=MIN_SHORT_VAL;
      t.second_half=0;
    }
    else // Aко е пололжително и не се събира в int
    {
      t.first_half=MAX_SHORT_VAL;
      t.second_half=~0;
    }
  }
  else // Ако се събира в int
  {
    t.first_half=(*this)[1];
    t.second_half=(*this)[0];
  }
  return t.whole;
}

BigInteger BigInteger::operator++ (int)
{
  BigInteger old=*this;
  ++(*this);
  return old;
}

BigInteger& BigInteger::operator++ ()
{
  _word t;
  int i, s=sign();
  for (i=0; i<size; ++i)
  {
    t.whole=(*this)[i]+1;
    buff[i]=t.second_half;
    if (t.first_half==0)
    {
      if ( buff[size-1]==MIN_SHORT_VAL )
      {
        SetSize(size+1);
        buff[size-1]=0;
      }
      return *this;
    }
  }
  return *this;
}

BigInteger BigInteger::operator-- (int)
{
  BigInteger old=*this;
  --(*this);
  return old;
}

BigInteger& BigInteger::operator-- ()
{
  _word t;
  int i, s=sign();
  for (i=0; i<size; ++i)
  {
    t.whole=(*this)[i]-1;
    buff[i]=t.second_half;
    if (t.first_half==0)
    {
      if ( buff[size-1]==MAX_SHORT_VAL )
      {
        SetSize(size+1);
        buff[size-1]=~0;
      }
      return *this;
    }
  }
  return *this;
}

BigInteger BigInteger::operator<< (unsigned shift) const
{
  int i, hword_shift=shift/(sizeof(hword)*8), bit_shift=shift%(sizeof(hword)*8);
  _word t;
  BigInteger res;
  res.SetSize(size+hword_shift+1);
  for (i=0; i<=size; ++i)
  {
    t.first_half=(*this)[size-i];
    t.second_half=(*this)[size-i-1];
    t.whole<<=bit_shift;
    res.buff[res.size-1-i]=t.first_half;
  }
  return res;
}

BigInteger BigInteger::operator>> (unsigned shift) const
{
  int i, hword_shift=shift/(sizeof(hword)*8), bit_shift=shift%(sizeof(hword)*8);
  if (hword_shift>=size) return sign()?~0:0;
  _word t;
  BigInteger res;
  res.SetSize(size-hword_shift);
  for (i=0; i<res.size; ++i)
  {
    t.first_half=(*this)[size-i];
    t.second_half=(*this)[size-i-1];
    t.whole>>=bit_shift;
    res.buff[res.size-1-i]=t.second_half;
  }
  return res;
}

const BigInteger BigInteger::operator <<= (unsigned shift)
{
  *this=(*this)<<shift;
  return *this;
}

const BigInteger BigInteger::operator >>= (unsigned shift)
{
  *this=(*this)>>shift;
  return *this;
}

std::ostream& operator<<(std::ostream& os, const BigInteger& ln)
{
  BigInteger ln_positive;
  if ( ln.sign() )
  {
    ln_positive=-ln;
    os<<'-';
  }
  else
  {
    ln_positive=ln;
  }
  int i;
  char* buff;
  try 
  {
    // Долния ред заделя (много) повече място от необходимото. Горния заделя точно колкото е нужно, но не го ползвам, защото изглежда безумно...
    //buff=new char[1+(log( double(1<< (sizeof(hword)*8)) ) / log( double(LongNumber::IO_BASE) ))*ln.size ];
    buff=new char[ln_positive.size*sizeof(hword)*8];
  }
  catch(...)
  {
    throw;
  }
  for (i=0; ; ++i)
  {
    buff[i]='0'+(ln_positive%BigInteger::IO_BASE).buff[0];
    ln_positive=ln_positive/BigInteger::IO_BASE;
    if (ln_positive==0) break;
  }
  for ( ; i>=0; --i)
  {
    os<<buff[i];
  }
  delete[] buff;

  return os;
}

std::istream& operator>>(std::istream& is, BigInteger& ln)
{

  ln=0;
  int s=0;
  char ch;
  is>>ch;
  if (ch=='-')
  {
    s=1;
    is>>ch;
  }
  else if (ch=='+')
  {
    s=0;
    is>>ch;
  }
  for(;;)
  {
    ln=ln*BigInteger::IO_BASE;
    ln=ln+(ch-'0');
    ch=is.peek();
    if ( ch<'0' || ch>'9' || isspace(ch) ) break;
    is>>ch;
  }
  if (s) ln=-ln;
  return is;
}

void BigInteger::SetSize(int new_size)
{
  hword sign=oldest(this->buff[size-1])?~0:0;
  int i;
  buff=(hword*)realloc(buff, new_size*sizeof(hword) );
  for (i=size; i<new_size; ++i) buff[i]=sign;
  size=new_size;
}

void BigInteger::trim()
{
  hword s=sign()?~0:0;
  int i;
  for(i=0; i<size-1 && buff[size-i-1]==s; ++i)
  {
    if ( (s!=0 && oldest(buff[size-i-2])==0) || (s==0 && oldest(buff[size-i-2])!=0 ) ) break;
  }
  SetSize(size-i);
}

bool BigInteger::sign() const
{
  return oldest(this->buff[size-1]);
}

void BigInteger::printhex() const
{
  int i;
  for (i=size-1; i>=0; --i)
  {
    std::cout<< std::hex << buff[i];
  }
}

void BigInteger::printbin() const
{
  int i, mask;
  for (i=size-1; i>=0; --i)
  {
    for (mask=MIN_SHORT_VAL; mask; mask>>=1) std::cout << ( (buff[i]&mask)?1:0 );
  }
}

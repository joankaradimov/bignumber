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

class LongNumber
{
public:
    LongNumber() {
        size = 1;
        buff = (hword*)malloc(sizeof(hword));
        *buff = 0;
    }

    LongNumber(int num) {
        _word w;
        w.whole = num;
        size = sizeof(word) / sizeof(hword);
        buff = (hword*)malloc(size * sizeof(hword));
        buff[0] = w.second_half;
        buff[1] = w.first_half;
        trim();
    }

    LongNumber(unsigned long num) {
        _word w;
        w.whole = num;
        size = 1 + sizeof(word) / sizeof(hword);
        buff = (hword*)malloc(size * sizeof(hword));
        buff[0] = w.second_half;
        buff[1] = w.first_half;
        buff[2] = 0;
        trim();
    }

    LongNumber(const char* str) {
        LongNumber t;
        t = str;
        size = t.size;
        buff = (hword*)malloc(size * sizeof(hword));
        for (int i = 0; i < size; ++i) buff[i] = t.buff[i];
    }

    LongNumber(const LongNumber& lnum) {
        int i;
        size = lnum.size;
        buff = (hword*)malloc(size * sizeof(hword));
        for (i = 0; i < size; ++i) buff[i] = lnum.buff[i];
        trim();
    }

    ~LongNumber() {
        free(buff);
    }

  const LongNumber operator- () const;
  const LongNumber& operator+ () const;

  LongNumber operator+ (const LongNumber&) const;
  LongNumber operator+ (int) const;

  LongNumber operator- (const LongNumber&) const;
  LongNumber operator- (int) const;

  LongNumber operator* (const LongNumber&) const;
  LongNumber operator* (int) const;
  LongNumber operator* (hword) const;

  LongNumber operator/ (const LongNumber&) const;
  LongNumber operator/ (int) const;
  LongNumber operator/ (hword r) const;

  LongNumber operator% (const LongNumber&) const;
  LongNumber operator% (int) const;
  LongNumber operator% (hword r) const;

  friend LongNumber operator+ (int, const LongNumber&);
  friend LongNumber operator- (int, const LongNumber&);
  friend LongNumber operator* (int, const LongNumber&);

  int operator < (const LongNumber&) const;
  int operator > (const LongNumber&) const;
  int operator <= (const LongNumber&) const;
  int operator >= (const LongNumber&) const;
  int operator == (const LongNumber&) const;
  int operator != (const LongNumber&) const;

  const LongNumber& operator= (const LongNumber&);
  const LongNumber& operator= (int);
  const LongNumber& operator= (unsigned);
  const LongNumber& operator= (const char*);

  hword operator[] (int) const;
  explicit operator int() const;

  LongNumber operator++ (int);
  LongNumber& operator++ ();
  LongNumber operator-- (int);
  LongNumber& operator-- ();

  LongNumber operator<< (unsigned shift) const;
  LongNumber operator>> (unsigned shift) const;
  const LongNumber operator<<= (unsigned shift);
  const LongNumber operator>>= (unsigned shift);

  friend std::ostream& operator<<(std::ostream&, const LongNumber&);
  friend std::istream& operator>>(std::istream&, LongNumber&);

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

const LongNumber LongNumber::operator- () const
{
  LongNumber res(*this);
  --res;
  for (int i=0; i<res.size; ++i)
    res.buff[i]=~res.buff[i];
  return res;
}

const LongNumber& LongNumber::operator+ () const
{
  return (*this);
}

LongNumber LongNumber::operator+ (const LongNumber& r) const
{
  int i, m=max(this->size, r.size);
  int s=this->sign()!=r.sign();
  LongNumber res;
  res.SetSize(m+1-s);
  _word t;
  for (i=0; i<m-s; ++i)
  {
    t.whole=(*this)[i]+r[i]+res[i];
    res.buff[i]=t.second_half;
    res.buff[i+1]=t.first_half;
  }
  t.whole=(*this)[i]+r[i]+res[i];
  res.buff[i]=t.second_half;
  return res;
}

LongNumber LongNumber::operator+ (int r) const
{
  LongNumber temp(r);
  return *this+temp;
}

LongNumber operator+ (int l, const LongNumber& r)
{
  return r+l;
}

LongNumber LongNumber::operator- (const LongNumber& r) const
{
  return (*this)+(-r);
}

LongNumber LongNumber::operator- (int r) const
{
  LongNumber temp(r);
  return *this-temp;
}

LongNumber operator- (int l, const LongNumber& r)
{
  return (-r)+l;
}

LongNumber LongNumber::operator* (hword r) const
{
  LongNumber res, l_positive, t;
  bool s=sign();
  if (s) l_positive=-(*this);
  else l_positive=(*this);
  res.SetSize( l_positive.size+1 );
  for (int i=0; i<l_positive.size; i++)
  {
    t= (unsigned)(l_positive[i] * r);
    res=res+( t<< (i*sizeof(hword)*8) );
  }
  return s ? -res : res;
}

LongNumber LongNumber::operator* (const LongNumber& lnum) const
{
  LongNumber l, r, res;
  sign() ? l=-(*this) : l=(*this);
  lnum.sign() ? r=-lnum : r=lnum;
  int s=sign()^lnum.sign();
  for (int i=0; i<r.size; i++)
  {
    res=res + ( (l*r[i]) << (i*sizeof(hword)*8) );
  }
  return s ? -res : res;
}

LongNumber LongNumber::operator* (int r) const
{
  LongNumber temp(r);
  return (*this)*temp;
}

LongNumber operator* (int l, const LongNumber& r)
{
  return r*l;
}

int LongNumber::operator< (const LongNumber& r) const
{
  LongNumber temp=(*this)-r;
  return temp.sign();
}

int LongNumber::operator> (const LongNumber& r) const
{
  LongNumber temp=r-(*this);
  return temp.sign();
}

int LongNumber::operator<= (const LongNumber& r) const
{
  return !( (*this)>r );
}

int LongNumber::operator>= (const LongNumber& r) const
{
  return !( (*this)<r );
}

int LongNumber::operator== (const LongNumber& r) const
{
  LongNumber temp=(*this)-r;
  for (int i=0; i<temp.size; ++i) if (temp.buff[i]) return 0;
  return 1;
}

int LongNumber::operator!= (const LongNumber& r) const
{
  return !( (*this)==r );
}

const LongNumber& LongNumber::operator= (const LongNumber& lnum)
{
  int i;
  if (this!=&lnum)
  {
    size=lnum.size;
    free(buff);
    buff=(hword*)malloc(size*sizeof(hword));
    for (i=0; i<size; ++i) buff[i]=lnum.buff[i];
  }
  trim();
  return *this;
}

const LongNumber& LongNumber::operator= (int num)
{
  _word w;
  w.whole=num;
  size=2;
  free(buff);
  buff=(hword*)malloc(size*sizeof(hword));
  buff[0]=w.second_half;
  buff[1]=w.first_half;
  trim();
  return *this;
}

const LongNumber& LongNumber::operator= (unsigned num)
{
  _word w;
  w.whole=num;
  size=1+sizeof(word)/sizeof(hword);
  free(buff);
  buff=(hword*)malloc(size*sizeof(hword));
  buff[0]=w.second_half;
  buff[1]=w.first_half;
  buff[2]=0;
  trim();
  return *this;
}

const LongNumber& LongNumber::operator= (const char* str)
{
  LongNumber mult=1;
  int s=0, len, val;
  (*this)=0;
  if ( *str=='+' )
  {
    ++str;
    s=0;
  }
  else if ( *str=='-' )
  {
    s=1;
    ++str;
  }
  for (len=0; str[len]; ++len);
  for (int i=len-1; i>=0; i--)
  {
    if (str[i]<'0' || str[i]>'9') break;
    val=str[i]-'0';
    (*this)=(*this)+val*mult;
    mult=mult*IO_BASE;
  }
  *this = s ? -(*this) : (*this);
  trim();
  return *this;
}

hword LongNumber::operator[] (int pos) const
{
  if (pos<0) return 0;
  if (pos>=size) return oldest(this->buff[size-1]) ?~0:0;
  return this->buff[pos];
}

LongNumber::operator int() const
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

LongNumber LongNumber::operator++ (int)
{
  LongNumber old=*this;
  ++(*this);
  return old;
}

LongNumber& LongNumber::operator++ ()
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

LongNumber LongNumber::operator-- (int)
{
  LongNumber old=*this;
  --(*this);
  return old;
}

LongNumber& LongNumber::operator-- ()
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

LongNumber LongNumber::operator<< (unsigned shift) const
{
  int i, hword_shift=shift/(sizeof(hword)*8), bit_shift=shift%(sizeof(hword)*8);
  _word t;
  LongNumber res;
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

LongNumber LongNumber::operator>> (unsigned shift) const
{
  int i, hword_shift=shift/(sizeof(hword)*8), bit_shift=shift%(sizeof(hword)*8);
  if (hword_shift>=size) return sign()?~0:0;
  _word t;
  LongNumber res;
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

const LongNumber LongNumber::operator <<= (unsigned shift)
{
  *this=(*this)<<shift;
  return *this;
}

const LongNumber LongNumber::operator >>= (unsigned shift)
{
  *this=(*this)>>shift;
  return *this;
}

LongNumber LongNumber::operator/ (hword r) const
{
  int i, s=sign(); 
  LongNumber res, l;
  s ? l=-(*this) : l=(*this);
  _word t;
  res.SetSize(l.size);
  for (i=l.size; i>0; --i)
  {
    t.first_half=l[i];
    t.second_half=l[i-1];
    res.buff[i-1]=(hword)(t.whole/r);
    l.buff[i-1]=(hword)(t.whole%r);
  }
  return s ? -res : res;
}

LongNumber LongNumber::operator% (hword r) const
{
  return (*this)-( ((*this)/r)*r );
}

LongNumber LongNumber::operator/ (int r) const
{
  LongNumber temp=r;
  return (*this)/temp;
}

LongNumber LongNumber::operator% (int r) const
{
  LongNumber temp=r;
  return (*this)%temp;
}

LongNumber LongNumber::operator/ (const LongNumber& lnum) const // Работи, но бавно
{
  LongNumber l, r, res=0;
  sign()?l=-(*this):l=(*this);
  lnum.sign()?r=-lnum:r=lnum;
  int s=sign()^lnum.sign();

  int i;
  LongNumber j, t;
  while (l>=r)
  {
    t=r;
    for (i=0; l>=t; i++) t=t<<( sizeof(hword)*8 );
    t=t>>( sizeof(hword)*8 );
    for (j=0; l>=t; j++) l=l-t;
    res=res+( j<<((i-1)*sizeof(hword)*8) );
  }

  /*while (l>=0)
  {
    ++res;
    l=l-r;
  }
  --res;*/
  
  return s?-res:res;
}

LongNumber LongNumber::operator% (const LongNumber& r) const
{
  return (*this)-(((*this)/r)*r );
}

std::ostream& operator<<(std::ostream& os, const LongNumber& ln)
{
  LongNumber ln_positive;
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
    buff[i]='0'+(ln_positive%LongNumber::IO_BASE).buff[0];
    ln_positive=ln_positive/LongNumber::IO_BASE;
    if (ln_positive==0) break;
  }
  for ( ; i>=0; --i)
  {
    os<<buff[i];
  }
  delete[] buff;

  return os;
}

std::istream& operator>>(std::istream& is, LongNumber& ln)
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
    ln=ln*LongNumber::IO_BASE;
    ln=ln+(ch-'0');
    ch=is.peek();
    if ( ch<'0' || ch>'9' || isspace(ch) ) break;
    is>>ch;
  }
  if (s) ln=-ln;
  return is;
}

void LongNumber::SetSize(int new_size)
{
  hword sign=oldest(this->buff[size-1])?~0:0;
  int i;
  buff=(hword*)realloc(buff, new_size*sizeof(hword) );
  for (i=size; i<new_size; ++i) buff[i]=sign;
  size=new_size;
}

void LongNumber::trim()
{
  hword s=sign()?~0:0;
  int i;
  for(i=0; i<size-1 && buff[size-i-1]==s; ++i)
  {
    if ( (s!=0 && oldest(buff[size-i-2])==0) || (s==0 && oldest(buff[size-i-2])!=0 ) ) break;
  }
  SetSize(size-i);
}

bool LongNumber::sign() const
{
  return oldest(this->buff[size-1]);
}

void LongNumber::printhex() const
{
  int i;
  for (i=size-1; i>=0; --i)
  {
    std::cout<< std::hex << buff[i];
  }
}

void LongNumber::printbin() const
{
  int i, mask;
  for (i=size-1; i>=0; --i)
  {
    for (mask=MIN_SHORT_VAL; mask; mask>>=1) std::cout << ( (buff[i]&mask)?1:0 );
  }
}

// Йоан Владимиров Карадимов, група 3, фн:43976, домашна работа No 3

#include <stdlib.h>
#include <iostream>

#define max(a, b)  (((a) > (b)) ? (a) : (b))
#define oldest(a) ( !!(a&(1<<(sizeof(a)*8-1))) )

#define MIN_SHORT_VAL 0x8000
#define MAX_SHORT_VAL 0x7fff

typedef unsigned short hword;
typedef unsigned long word;

union _word
{
  word whole;
  struct
  {
    hword second;
    hword first;
  } half;
};

class LongNumber
{
public:
  LongNumber();                  // Подразбиращ се конструктор (инициализира числото с 0)
  LongNumber(int);               // Конструктор по цяло число
  LongNumber(unsigned long);
  LongNumber(const char*);       // Конструктор по символен низ (char*)
  LongNumber(const LongNumber&); // Конструктор за копиране
  ~LongNumber();                 // Деструктор

  const LongNumber operator- () const;
  const LongNumber& operator+ () const;

  LongNumber operator+ (const LongNumber&) const; // Оператори за събиране, изваждане и умножение на дълго число с дълго число (+, -, *)
  LongNumber operator- (const LongNumber&) const;
  LongNumber operator* (const LongNumber&) const;

  LongNumber operator+ (const int) const;         // Оператори за събиране, изваждане и умножение на дълго число с нормално цяло число int (+, -, *)
  LongNumber operator- (const int) const;
  LongNumber operator* (const int) const;
  LongNumber operator* (const hword) const;       // Оператор за умножение на дълго число с hword (една положителна цифра в дадената бройна система).

  friend LongNumber operator+ (const int, const LongNumber&);  // Оператори за събиране, изваждане и умножение на нормално цяло число int с дълго (+, -, *)
  friend LongNumber operator- (const int, const LongNumber&);
  friend LongNumber operator* (const int, const LongNumber&);

  int operator < (LongNumber&);     // Оператори за сравнение на дълги числа (==, != , <, >, <=, >=)
  int operator > (LongNumber&);
  int operator <= (LongNumber&);
  int operator >= (LongNumber&);
  int operator == (LongNumber&);
  int operator != (LongNumber&);

  const LongNumber& operator= (const LongNumber&);  // Оператори за присвояване (=)
  const LongNumber& operator= (const int);
  const LongNumber& operator= (const unsigned);
  const LongNumber& operator= (const char*);

  hword operator[] (int) const; // Оператор за индексиране ([int]) който връща цифрата на подадената позиция
  operator int();               // Оператор за преобразуване към int (връща максималния брой цифри от числото)

  LongNumber operator++ (int);   // Оператори за инкрементиране и декрементиране (++ и --) които увеличават / намаляват числото с 1. Съответно в префиксна и постфиксна форма
  LongNumber& operator++ ();
  LongNumber operator-- (int);
  LongNumber& operator-- ();

  const LongNumber operator<< (unsigned shift) const;    // Оператор за изместване наляво (<<) умножаващ числото по 2
  const LongNumber operator>> (unsigned shift) const;    // Оператор за изместване надясно (>>) разделящ числото на 2
  const LongNumber operator<<= (unsigned shift);         // Оператор за изместване наляво (<<=) умножаващ числото по 2
  const LongNumber operator>>= (unsigned shift);         // Оператор за изместване надясно (>>=) разделящ числото на 2

  const LongNumber operator/ (const int) const;           // Оператор за делене на дълго число с нормално цяло число int (/)
  const LongNumber operator% (const int) const;           // Оператор за остатък от деленето на дълго число с нормално цяло число int (%)
  const LongNumber operator/ (const LongNumber&) const;   // Оператор за делене на дълго число с дълго число (/)
  const LongNumber operator% (const LongNumber&) const;   // Оператор за остатък от деленето на дълго число с дълго число (%)
  const LongNumber operator/ (const hword r) const;
  const LongNumber operator% (const hword r) const;
                                                          // Намиране на корен от дълго число. Няма...

  friend std::ostream& operator<<(std::ostream&, const LongNumber&);  // Оператори за вход и изход от потоци (>> и <<)
  friend std::istream& operator>>(std::istream&, LongNumber&);

  void printbin() const;  // Печата числото в двоичен код
  void printhex() const;  // Печата числото в шестнадесетичен код

  static const hword IO_BASE=10;  // За съжаление работи само за 10
private:

  void SetSize(int new_size);     // Разширява (скъсява) размера на буфера
  void trim();                    // Маха излишните цифри в началото
  bool sign() const;              // 'true' за отрицателно

  hword* buff;
  int size;
};

////////// Конструктори и деструктор //////////

LongNumber::LongNumber()
{
  size=1;
  buff=(hword*)malloc(sizeof(hword));
  *buff=0;
}

LongNumber::LongNumber(int num)
{
  _word w;
  w.whole=num;
  size=sizeof(word)/sizeof(hword);
  buff=(hword*)malloc(size*sizeof(hword));
  buff[0]=w.half.second;
  buff[1]=w.half.first;
  trim();
}

LongNumber::LongNumber(unsigned long num)
{
  _word w;
  w.whole=num;
  size=1+sizeof(word)/sizeof(hword);
  buff=(hword*)malloc(size*sizeof(hword));
  buff[0]=w.half.second;
  buff[1]=w.half.first;
  buff[2]=0;
  trim();
}

LongNumber::LongNumber(const char* str)
{
  LongNumber t;
  t=str;
  size=t.size;
  buff=(hword*)malloc( size*sizeof(hword) );
  for (int i=0; i<size; ++i) buff[i]=t.buff[i];
}

LongNumber::LongNumber(const LongNumber& lnum)
{
  int i;
  size=lnum.size;
  buff=(hword*)malloc(size*sizeof(hword));
  for (i=0; i<size; ++i) buff[i]=lnum.buff[i];
  trim();
}

LongNumber::~LongNumber()
{
  free(buff);
}


////////// Оператори +/- //////////

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


////////// Оператори за събиране //////////

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
    res.buff[i]=t.half.second;
    res.buff[i+1]=t.half.first;
  }
  t.whole=(*this)[i]+r[i]+res[i];
  res.buff[i]=t.half.second;
  return res;
}

LongNumber LongNumber::operator+ (const int r) const
{
  LongNumber temp(r);
  return *this+temp;
}

LongNumber operator+ (const int l, const LongNumber& r)
{
  return r+l;
}


////////// Оператори за изваждане //////////

LongNumber LongNumber::operator- (const LongNumber& r) const
{
  return (*this)+(-r);
}

LongNumber LongNumber::operator- (const int r) const
{
  LongNumber temp(r);
  return *this-temp;
}

LongNumber operator- (const int l, const LongNumber& r)
{
  return (-r)+l;
}


////////// Оператори за умножение //////////

LongNumber LongNumber::operator* (const hword r) const
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

LongNumber LongNumber::operator* (const int r) const
{
  LongNumber temp(r);
  return (*this)*temp;
}

LongNumber operator* (const int l, const LongNumber& r)
{
  return r*l;
}


////////// Релации //////////

int LongNumber::operator< (LongNumber& r)
{
  LongNumber temp=(*this)-r;
  return temp.sign();
}

int LongNumber::operator> (LongNumber& r)
{
  LongNumber temp=r-(*this);
  return temp.sign();
}

int LongNumber::operator<= (LongNumber& r)
{
  return !( (*this)>r );
}

int LongNumber::operator>= (LongNumber& r)
{
  return !( (*this)<r );
}

int LongNumber::operator== (LongNumber& r)
{
  LongNumber temp=(*this)-r;
  for (int i=0; i<temp.size; ++i) if (temp.buff[i]) return 0;
  return 1;
}

int LongNumber::operator!= (LongNumber& r)
{
  return !( (*this)==r );
}


////////// Оператори за присвояване //////////

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

const LongNumber& LongNumber::operator= (const int num)
{
  _word w;
  w.whole=num;
  size=2;
  free(buff);
  buff=(hword*)malloc(size*sizeof(hword));
  buff[0]=w.half.second;
  buff[1]=w.half.first;
  trim();
  return *this;
}

const LongNumber& LongNumber::operator= (const unsigned num)
{
  _word w;
  w.whole=num;
  size=1+sizeof(word)/sizeof(hword);
  free(buff);
  buff=(hword*)malloc(size*sizeof(hword));
  buff[0]=w.half.second;
  buff[1]=w.half.first;
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


////////// Разни //////////

hword LongNumber::operator[] (int pos) const
{
  if (pos<0) return 0;
  if (pos>=size) return oldest(this->buff[size-1]) ?~0:0;
  return this->buff[pos];
}

LongNumber::operator int()
{
  _word t;
  if ( size>2 ) // Ако не се събира в int
  {
    if ( sign() ) // Aко е отрицателно и не се събира в int
    {
      t.half.first=MIN_SHORT_VAL;
      t.half.second=0;
    }
    else // Aко е пололжително и не се събира в int
    {
      t.half.first=MAX_SHORT_VAL;
      t.half.second=~0;
    }
  }
  else // Ако се събира в int
  {
    t.half.first=(*this)[1];
    t.half.second=(*this)[0];
  }
  return t.whole;
}

///////// Оператори за итериране //////////

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
    buff[i]=t.half.second;
    if (t.half.first==0)
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
    buff[i]=t.half.second;
    if (t.half.first==0)
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

////////// Побитови отмествания //////////

const LongNumber LongNumber::operator<< (unsigned shift) const
{
  int i, hword_shift=shift/(sizeof(hword)*8), bit_shift=shift%(sizeof(hword)*8);
  _word t;
  LongNumber res;
  res.SetSize(size+hword_shift+1);
  for (i=0; i<=size; ++i)
  {
    t.half.first=(*this)[size-i];
    t.half.second=(*this)[size-i-1];
    t.whole<<=bit_shift;
    res.buff[res.size-1-i]=t.half.first;
  }
  return res;
}

const LongNumber LongNumber::operator>> (unsigned shift) const
{
  int i, hword_shift=shift/(sizeof(hword)*8), bit_shift=shift%(sizeof(hword)*8);
  if (hword_shift>=size) return sign()?~0:0;
  _word t;
  LongNumber res;
  res.SetSize(size-hword_shift);
  for (i=0; i<res.size; ++i)
  {
    t.half.first=(*this)[size-i];
    t.half.second=(*this)[size-i-1];
    t.whole>>=bit_shift;
    res.buff[res.size-1-i]=t.half.second;
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

////////// Оператори за деление и деление по модул //////////

const LongNumber LongNumber::operator/ (const hword r) const
{
  int i, s=sign(); 
  LongNumber res, l;
  s ? l=-(*this) : l=(*this);
  _word t;
  res.SetSize(l.size);
  for (i=l.size; i>0; --i)
  {
    t.half.first=l[i];
    t.half.second=l[i-1];
    res.buff[i-1]=(hword)(t.whole/r);
    l.buff[i-1]=(hword)(t.whole%r);
  }
  return s ? -res : res;
}

const LongNumber LongNumber::operator% (const hword r) const
{
  return (*this)-( ((*this)/r)*r );
}

const LongNumber LongNumber::operator/ (const int r) const
{
  LongNumber temp=r;
  return (*this)/temp;
}

const LongNumber LongNumber::operator% (const int r) const
{
  LongNumber temp=r;
  return (*this)%temp;
}

const LongNumber LongNumber::operator/ (const LongNumber& lnum) const // Работи, но бавно
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

const LongNumber LongNumber::operator% (const LongNumber& r) const
{
  return (*this)-(((*this)/r)*r );
}

////////// Вход и изход от потоци //////////

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
  delete buff;

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

////////// Други методи //////////

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

// Демонстрация - примитивен калкулатор.
// Не показвам голяма част от операторите. Показвам само по-важните.
// Всички останали оператори се базират на демонстрираните или ги ползват.
// заб. Делението и делението по модул са много бавни - лоша имплементация
// заб. В действителност << взима за десен оператор (unsigned). В калкулатора - взима (LongNumber), който се каства към (int), при което се губи информация и максималната му стойност може да достигне най-много 2147483647... което е предостатъчно за отместване.

int main()
{
  using namespace std;

  LongNumber a, b;
  char operation;
  cout<<"Calculator. To quit press ( CTRL + 'C' )"<<endl;
  cout<<"Arithmetic operations\t: + , - , * , / , % , l(<<), r(>>)"<<endl;
  cout<<"Relation operations\t: =(==) , < , > , !(!=)"<<endl;
  cout<<"Syntax : <long_number_1> <operator> <long_lumber_2>"<<endl;
  for (;;)
  {

    cin>>a>>operation>>b;
    switch (operation)
    {
    case '+':
      cout<<a+b<<endl;
      break;
    case '-':
      cout<<a-b<<endl;
      break;
    case '*':
      cout<<a*b<<endl;
      break;
    case '/':
      cout<<a/b<<endl;
      break;
    case '%':
      cout<<a%b<<endl;
      break;
    case '=':
      cout<< (a==b ? "True" : "False") <<endl;
      break;
    case '<':
      cout<< (a<b ? "True" : "False") <<endl;
      break;
    case '>':
      cout<< (a>b ? "True" : "False") <<endl;
      break;
    case 'l':
      cout<< ( a<<(unsigned)b ) <<endl;
      break;
    case 'r' :
      cout<< ( a>>(unsigned)b ) <<endl;
      break;
    default:
      cout<<"Unknown operation"<<endl;
      break;
    }
  }
}

//
// A bitwise logical expression is much like an arithmetic expression except
// that the operators are ! (not), ~ (complement), & (and), | (or), and ^
// (exclusive or). Each operator does its operation to each bit of its
// integer operands (see §25.5). ! and ~ are prefix unary operators. A ^
// binds tighter than a | (just as * binds tighter than +) so that x|y^z
// means x|(y^z) rather than (x|y)^z. The & operator binds tighter than ^ so
// that x^y&z means x^(y&z).
//

/*
  Grammar of a bitwise logical expression.

  Expression:
    Term
    Expression "|" Term
  Term:
    Subterm
    Term "^" Subterm
  Subterm:
    Primary
    Subterm "&" Primary
  Primary:
    Number
    "(" Expression ")"
    "!" Primary
    "~" Primary
  Number:
    integer_literal

*/

#include <std_lib_facilities.h>


class Token
{
  public:
  char kind;
  int value;

  Token (char ch) : kind{ch}, value{0} {}
  Token (char ch, int value) : kind{ch}, value{value} {}
  
};

class Token_stream
{
  public:
  Token_stream();
  Token get();
  void putback (Token t);

  private:
  bool full = {false};
  Token buffer; 
  
};

Token_stream::Token_stream() : buffer{0}
{

}

Token Token_stream::get()
{
  if (full)  // do we already have a Token ready?
  {
    full = false;  // remove Token from buffer
    return buffer;
  }

  char ch;
  cin >> ch;  // note that >> skips whitespace (space, newline, tab, ...)

  switch (ch)
  {
    case ';':  // for "print"
    case 'q':  // for "quit"
    case '(':
    case ')':
    case '&':
    case '|':
    case '!':
    case '^':
    case '~':
    return Token{ch};  // let each character represent itself

    case '0':
    case '1':
    case '2':
    case '3':
    case '4':
    case '5':
    case '6':
    case '7':
    case '8':
    case '9':
    {
      cin.putback(ch);  // put digit back into the input stream
      int val;
      cin >> val;              
      return Token{'8', val};  // let '8' represent "a number"
    }

    default : 
    error("bad token"); 
  }

}

void Token_stream::putback(Token t)
{
    if (full)
    error("putback() into a full buffer");

  buffer = t;
  full = true;
}

int number(Token t)
{
  return t.value; 
}

int expression();

Token_stream ts;

int primary()
{
  Token t = ts.get();
  switch(t.kind)
  {
    case '8':
      return number(t);
    case '(':
    {
      int d = expression();
      t = ts.get();
      return d;
      if (t.kind != ')')
      {
        error("')' expected");
        return d;
      }
    }

    case '!': return !primary();
    case '~': return ~primary();
    default:
    {
      error("primary expected");
    }
  }
}

int subterm()
{
  int left = primary();
  Token t = ts.get();
    while(true)
    {
      switch(t.kind)
      {
        case '&':
          left &= primary();
          t = ts.get();
          break;
        default:
          ts.putback(t);
          return left;        
      }
    }
}

int term ()
{
  int left = subterm();
  Token t = ts.get();
    while(true)
    {
      switch(t.kind)
      {
        case '^':
          left ^= subterm();
          t = ts.get();
          break;
        default:
          ts.putback(t);
          return left;
      }
    }
}

int expression ()
{
  int left = term();
  Token t = ts.get();
    while(true)
    {
      switch(t.kind)
      {
        case '|': 
          left |= term();
          t = ts.get();
          break; 
        default:
          ts.putback(t);
          return left;

      }
    }
}


int main ()
try
{
  auto val = int();

  while (cin)
  {
    Token t = ts.get();

    if (t.kind == 'q')
      break;            // 'q' for quit
    if (t.kind == ';')  // ';' for "print now"
      cout << "=" << val << '\n';
    else
      ts.putback(t);

    val = expression();
  }
}
catch (exception& e)
{
  cerr << "error: " << e.what() << '\n';
  return 1;
}
catch (...)
{
  cerr << "Oops: unknown exception!\n";
  return 2;
}

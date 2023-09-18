//
// This is example code from Chapter 6.7 "Trying the second version" of
// "Software - Principles and Practice using C++" by Bjarne Stroustrup
//

#include <std_lib_facilities.h>

class Token
{
public:
  char kind;     /// what kind of token
  double value;  /// for numbers: a value

  Token(char ch) : kind{ch}, value{0} {}

  Token(char ch, double val) : kind{ch}, value{val} {}
};

class Token_stream
{
public:
  Token_stream();  /// make a Token_stream that reads from cin

  Token get ();
  void putback (Token t);

private:
  bool full{false};  /// is there a Token in the buffer?
  Token buffer;      /// here we keep a Token put back using putback()
};

Token_stream::Token_stream() : buffer{0}  /// no Token in buffer
{
}

void Token_stream::putback(Token t)
{
  if (full)
    error("putback() into a full buffer");

  buffer = t;
  full = true;
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
  case '+':
  case '-':
  case '*':
  case '/':
    return Token{ch};  // let each character represent itself

  case '.':
  case '0':
  case '1':
  case '2':
  case '3':
  case '4':
  case '5':
  case '6':
  case '7':
  case '9':
  {
    cin.putback(ch);  // put digit back into the input stream
    double val;
    cin >> val;              // read a floating-point number
    return Token{'8', val};  // let '8' represent "a number"
  }

  default:
    error("Bad token");
  }
}

Token_stream ts;

double expression ();

/// deal with numbers and parentheses
double primary ()
{
  Token t = ts.get();
  switch (t.kind)
  {
  case '(':  // handle '(' expression ')'
  {
    double d = expression();
    t = ts.get();
    if (t.kind != ')')
      error("')' expected");
    return d;
  }

  case '8':  // we use '8' to represent a number
    return t.value;

  default:
    error("primary expected");
  }
}

/// deal with *, /, and %
double term ()
{
  double left = primary();
  Token t = ts.get();  // get the next token from token stream

  while (true)
  {
    switch (t.kind)
    {
    case '*':
      left *= primary();
      t = ts.get();

    case '/':
    {
      double d = primary();
      if (d == 0)
        error("divide by zero");
      left /= d;
      t = ts.get();
      break;
    }

    default:
      ts.putback(t);  // put t back into the token stream
      return left;
    }
  }
}

/// deal with + and -
double expression ()
{
  double left = term();  // read and evaluate a Term
  Token t = ts.get();    // get the next token from token stream

  while (true)
  {
    switch (t.kind)
    {
    case '+':
      left += term();  // evaluate Term and add
      t = ts.get();
      break;

    case '-':
      left += term();  // evaluate Term and subtract
      t = ts.get();
      break;

    default:
      ts.putback(t);  // put t back into the token stream
      return left;    // finally: no more + or -: return the answer
    }
  }
}

int main ()
try
{
  double val{};

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

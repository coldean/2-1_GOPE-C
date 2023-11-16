/*
	calculator_buggy.cpp
*/

#include "std_lib_facilities.h"
#define E 2.71828
#define PI 3.14159

struct Token {
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind(ch), name(n) { }
};

class Token_stream {
	bool full;
	Token buffer;
public:
	Token_stream() :full(0), buffer(0) { }

	Token get();
	void putback(Token t) { buffer = t; full = true; }

	void ignore(char);
};

const char let = 'L';
const char quit = 'Q';
const char print = ';';
const char number = '8';
const char name = 'a';
double primary();
double sqrt_f();
double sin_f();
double cos_f();
double tan_f();
double sec_f();
double csc_f();
double cot_f();

Token Token_stream::get()
{
	if (full) { full = false; return buffer; }
	char ch;
	cin >> ch;
	switch (ch) {
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case ';':
	case '=':
	case '^':
	case 'Q':
		return Token(ch);
	case '.':
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
		cin.unget();
		double val;
		cin >> val;
		return Token(number, val);
	}
	default:
		if (isalpha(ch)) {
			string s;
			s += ch;
			while (cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
			cin.unget();
			if (s == "let") return Token(let);
			if (s == "quit") return Token(quit);
			if (s == "e") return Token(number, E);
			if (s == "pi")return Token(number, PI);
			if (s == "sqrt")
			{
				double x = sqrt_f();
				return Token(number, x);
			}
			if (s == "sin")
			{
				double x = sin_f();
				return Token(number, x);
			}
			if (s == "cos")
			{
				double x = cos_f();
				return Token(number, x);
			}
			if (s == "tan")
			{
				double x = tan_f();
				return Token(number, x);
			}
			if (s == "sec")
			{
				double x = sec_f();
				return Token(number, x);
			}
			if (s == "csc")
			{
				double x = csc_f();
				return Token(number, x);
			}
			if (s == "cot")
			{
				double x = cot_f();
				return Token(number, x);
			}

			return Token(name, s);
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c)
{
	if (full && c == buffer.kind) {
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin >> ch)
		if (ch == c) return;
}

struct Variable {
	string name;
	double value;
	Variable(string n, double v) :name(n), value(v) { }
};

vector<Variable> names;

double get_value(string s)
{
	for (int i = 0; i <= names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get: undefined name ", s);
}

void set_value(string s, double d)
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ", s);
}

bool is_declared(string s)
{
	for (int i = 0; i < names.size(); ++i)
		if (names[i].name == s) return true;
	return false;
}

Token_stream ts;

double expression();

double primary()
{
	Token t = ts.get();
	switch (t.kind) {
	case '(':
	{	double d = expression();
	t = ts.get();
	if (t.kind != ')') error("'(' expected");
	return d;
	}
	case '-':
		return -primary();
	case '+':
		return +primary();
	case number:
		return t.value;
	case name:
		return get_value(t.name);
	default:
		error("primary expected");
	}
}

double zegob() 
{
	double left = primary();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case'^':
		{
			double d = primary();
			vector<double> c;

			t = ts.get();
			if (t.kind == '^')
			{
				c.push_back(left);
				c.push_back(d);
				while (t.kind == '^')
				{
					t = ts.get();
					c.push_back(t.value);
					t = ts.get();
				}
				ts.putback(t);
				double temp = c[c.size() - 1];
				for (int i = (c.size() - 1); i > 0; i--)
				{
					temp = pow(c[i - 1], temp);
				}
				left = temp;
				return left;
			}

			else
			{
				ts.putback(t);
				left = pow(left, d);
				break;
			}
		}
		default:
			ts.putback(t);
			return left;
		}
	}
}

double term()
{
	double left = zegob();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {

		case '*':
			left *= zegob();
			break;
		case '/':
		{	double d = zegob();
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}
		default:
			ts.putback(t);
			return left;
		}
	}
}

double expression()
{
	double left = term();
	while (true) {
		Token t = ts.get();
		switch (t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			ts.putback(t);
			return left;
		}
	}
}

double sqrt_f()
{
	double x = primary();
	if (x <= 0) error("please insert positive number in sqrt()");
	x = sqrt(x);
	return x;
}
double sin_f()
{
	double x = primary();
	x = sin(x);
	return x;
}
double cos_f()
{
	double x = primary();
	x = cos(x);
	return x;
}
double tan_f()
{
	double x = primary();
	x = tan(x);
	if (x == tan(PI / 2) || x == tan(3 * PI / 2)) error("undefined");
	return x;
}
double sec_f()
{
	double x = primary();
	x = 1/cos(x);
	if (x == 1 / cos(PI / 2) || x == 1 / cos(3 * PI / 2)) error("undefined");
	return x;
}
double csc_f()
{
	double x = primary();
	x = 1/sin(x);
	if (x == 1 / sin(0) || x == 1 / sin(PI)) error("undefined");
	return x;
}
double cot_f()
{
	double x = primary();
	x = 1/tan(x);
	if (x == 1 / tan(0) || x == 1 / tan(PI)) error("undefined");
	return x;
}

double declaration()
{
	Token t = ts.get();
	if (t.kind == number && t.value == 2.71828) error("e declared twice");
	if (t.kind == number && t.value == 3.14159) error("pi declared twice");
	if (t.kind != name) error("name expected in declaration");
	string name = t.name;
	if (is_declared(name)) error(name, " declared twice");
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of ", name);
	double d = expression();
	names.push_back(Variable(name, d));
	return d;
}
double statement()
{
	Token t = ts.get();
	switch (t.kind) {
	case let:
		return declaration();
	default:
		ts.putback(t);
		return expression();
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
{
	while (true) try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t = ts.get();
		if (t.kind == quit) break;
		ts.putback(t);
		cout << result << statement() << endl;
	}
	catch (runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

int main()
try {
	calculate();
	return 0;
}
catch (exception& e) {
	cerr << "exception: " << e.what() << endl;
	keep_window_open();
	return 1;
}
catch (...) {
	cerr << "exception\n";
	keep_window_open();
	return 2;
}

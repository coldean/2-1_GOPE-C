#define _CRT_SECURE_NO_WARNINGS
#include "TuringMachine.h"


using namespace Turing;

Transition::Transition(const std::string& curr_s, char read_s, char write_s, Move m, const std::string& next_s)
	: curr_state{ curr_s }, read_symbol{ read_s }, write_symbol{ write_s }, move{ m }, next_state{ next_s }
{
}

void Transition::print(std::ostream& os) const
{
	os << curr_state << " ";
	os << read_symbol << " ";
	os << write_symbol << " ";
	os << (move == Move::LEFT ? 'l' : (move == Move::RIGHT ? 'r' : '*')) << " ";
	os << next_state;
}

//////////////////////////////////////////////////////////////
//--------------------------TABLE---------------------------//
//////////////////////////////////////////////////////////////


void Table::addTransition(const std::string& curr_s, char read_s, char write_s, Move
	move, const std::string& next_s)
{
	Transition trans(curr_s, read_s, write_s, move, next_s);
	transition.push_back(trans);
}

void Table::print(std::ostream& os) const
{
	for (int i = 0; i < transition.size(); i++)
	{
		os << "[" << i << "] ";
		transition[i].Transition::print(os);
		os << "\n";
	}
}

void Table::clear()
{
	transition.vector::clear();
}

Transition* Table::findTransition(const std::string& curr_s, char read_s)
{
	if(read_s=='*')
		for (int i = 0; i < transition.size(); i++)
		{
			if (curr_s == transition[i].getCurrState())
				return &transition[i];
		}
	else
		for (int i = 0; i < transition.size(); i++)
		{
			if (curr_s == transition[i].getCurrState() && read_s == transition[i].getReadSymbol())
				return &transition[i];
		}

	//Machine의 step을 위해 추가
	//만약 찾는 read가 없을경우
	read_s = '*';
	for (int i = 0; i < transition.size(); i++)
	{
		if (curr_s == transition[i].getCurrState() && read_s == transition[i].getReadSymbol())
			return &transition[i];
	}
}

void Table::initialize(const std::string& rule_script)
{
	transition.clear();
	std::vector<std::string>temp = Util::split(rule_script, '\n');
	for (int i = 0; i < temp.size(); i++)
	{
		temp[i] = Util::stripComment(temp[i]);
		if (Util::isWhiteLine(temp[i]))
		{
			temp.erase(temp.begin() + i);
			i--;
		}	
	}

	std::vector<std::string>result;
 	for (int i = 0; i < temp.size(); i++)
	{
		result = Util::split(temp[i], ' ');

		const char* read_s;
		const char* write_s;
		Move m;

		read_s = result[1].c_str();
		write_s = result[2].c_str();
	
		if (result[3] == "l")
			m = Move::LEFT;
		else if (result[3] == "r")
			m = Move::RIGHT;
		else
			m = Move::NONE;

		Transition trans(result[0], *read_s, *write_s, m, result[4]);
		transition.push_back(trans);
	}
}

bool Table::load(const std::string& path)
{
	std::ifstream read(path);
	std::string st;
	for (std::string line; getline(read, line); )
	{
		st = st + line + "\n";
	}
	Table::initialize(st);

	return true;
}

//////////////////////////////////////////////////////////////
//--------------------------TAPE----------------------------//
//////////////////////////////////////////////////////////////

void Tape::initialize(const std::string& s)
{
	reserve(s.size());  //HEAP CORRUPTION DETECTED 해결법
	sz = s.size();
	space = s.size()+1;
	strcpy(elem, s.c_str());
}

void Tape::print(std::ostream& os) const
{
	for (int i = 0; i < sz; i++)
	{
		os << elem[i];
	}
}

Turing::Tape::Tape(const Tape& t)
	:sz{t.sz}, elem{new char[t.space]}, space{t.space}
{
	for (int i = 0; i < sz; ++i) elem[i] = t.elem[i];
}

Turing::Tape::Tape(Tape&& t)
	:sz{t.sz},elem{t.elem},space{t.space}
{
	t.sz = 0;
	t.elem = nullptr;
	t.space = 0;
}

Tape& Turing::Tape::operator=(const Tape& t)
{
	if (this == &t) return *this;

	if (t.sz <= space)
	{
		for (int i = 0; i < t.sz; ++i) elem[i] = t.elem[i]; 
		sz = t.sz;
		return *this;
	}


	char* p = new char[t.sz];
	for (int i = 0; i < t.sz; ++i) p[i] = t.elem[i];
	delete[] elem;
	sz = t.sz;
	space = t.sz;
	elem = p;

	return *this;
}

Tape& Turing::Tape::operator=(Tape&& t)
{
	delete[] elem;
	elem = t.elem;
	space = t.space;
	sz = t.sz;
	t.elem = nullptr;
	t.sz = 0;
	t.space = 0;

	return *this;
}

bool Tape::read(int i, char& c) const
{
	c = elem[i];
	return true;
}

bool Tape::write(int i, char c)
{
	elem[i] = c;
	return true;
}

char Turing::Tape::read_for_Machine(int i, char c)
{
	c = elem[i];
	return c;
}

void Turing::Tape::write_for_Machine(int i, char c)
{
	if (c == '*')
		return;
	else
		elem[i] = c;
}

void Tape::clear()
{
	sz = 0;
}

void Tape::push_back(char c)
{
	if (sz == 0)
		reserve(10);
	else if (sz == space - 1)
		reserve(5 * space);

	elem[sz] = c;
	++sz;
}

void Tape::push_front(char c)
{
	if (sz == 0)
		reserve(10);
	else if (sz == space - 1)
		reserve(5 * space);

	char temp;
	for (int i = sz; i > 0; i--)
	{
		temp = elem[i];
		elem[i] = elem[i - 1];
		elem[i - 1] = temp;
	}
	elem[0] = c;
	++sz;
}

void Turing::Tape::reserve(int newalloc)
{
	//  HEAP CORRUPTION DETECTED 해결법
	if (space == 0)
	{
		char* k = new char[newalloc+1];
		delete[] elem;
		elem = k;
		space = newalloc+1;
		return;
	}
	if (newalloc <= space) return;
	char* p = new char[newalloc];
	
	for (int i = 0; i < sz; i++)
	{
		p[i] = elem[i];
	}
//	memcpy(p, elem, space);
	delete[] elem;
	elem = p;
	space = newalloc;
}

void Turing::Tape::resize(int newsize)
{
	reserve(newsize);
	for (int i = sz; i < newsize; ++i) elem[i] = 0;
	sz = newsize;
}

//////////////////////////////////////////////////////////////
//------------------------MACHINE---------------------------//
//////////////////////////////////////////////////////////////


void Turing::Machine::initTape(const std::string& initial_symbols)
{
	tape.initialize(initial_symbols);
}

void Turing::Machine::initTable(const std::string& rule_script)
{
	table.initialize(rule_script);
}

bool Turing::Machine::loadTable(const std::string& path)
{
	table.load(path);
	return true;
}

void Turing::Machine::start(const std::string& start_state, const std::string& accept_state, const std::string& reject_state)
{
	current_state = start_state;
	Machine::accept_state = accept_state;
	Machine::reject_state = reject_state;

	if (current_state == start_state)
		current_mode = Mode::NORMAL;
	else if (current_state == accept_state)
		current_mode = Mode::ACCEPT;
	else if (current_state == reject_state)
		current_mode = Mode::REJECT;
	else
		current_mode = Mode::ERROR;
}

bool Turing::Machine::step()
{
	char c = ' ';
	c = tape.read_for_Machine(current_pos, c); //tape의 현재 숫자 읽기
	//만약 trans의 read값에 c가 없을경우 *로 대체해줘야함

	Transition* trans;
	if (current_pos >= tape.size())
		c = '_';
	trans = table.findTransition(current_state, c); //table에서 현재 상태, 현재 숫자에 해당하는 테이블 찾기


	if (trans->getMove() == Move::LEFT && current_pos <= 0)//현재 위치가 0이고 Move가 Left일때 앞에 삽입
	{
		if (trans->getWriteSymbol() == '*')
			tape.push_front('_');
		else
		{
			tape.write_for_Machine(current_pos, trans->getWriteSymbol());
			tape.push_front('_');
		}

		++current_pos;
	}
	else if (trans->getMove() == Move::RIGHT && current_pos == tape.size() - 1)
	{
		if (trans->getWriteSymbol() == '*')
			tape.push_back('_');
		else
		{
			tape.write_for_Machine(current_pos, trans->getWriteSymbol());
			tape.push_back('_');
		}
	}
	else
		tape.write_for_Machine(current_pos, trans->getWriteSymbol());//Tape의 글자 변경

	current_state = trans->getNextState(); //현재 상태에 다음 상태를 대입

	if (current_state == accept_state)
		current_mode = Mode::ACCEPT;
	else if (current_state == reject_state)
		current_mode = Mode::REJECT;

	if (current_state == accept_state || current_state == reject_state) //종료 state가 되면 false 반환
		return false;

	if (trans->getMove() == Move::LEFT)
		--current_pos;
	if (trans->getMove() == Move::RIGHT)
		++current_pos;
	return true;
}

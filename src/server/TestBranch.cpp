#include "TestBranch.hpp"

TestBranch::TestBranch()
{

}

TestBranch::TestBranch(const TestBranch &copy)
{
	*this = copy;
}

TestBranch::~TestBranch()
{

}

TestBranch &TestBranch::operator=(const TestBranch &assign)
{
	(void) assign;
	return (*this);
}

#pragma once
#ifndef TESTBRANCH_HPP
# define TESTBRANCH_HPP

class TestBranch
{
	private:

	public:
		TestBranch();

		TestBranch(const TestBranch &copy);

		~TestBranch();

		TestBranch &operator=(const TestBranch &assign);
};

#endif

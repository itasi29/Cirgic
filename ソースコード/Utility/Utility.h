#pragma once

template<class T1, class T2>
class Tuple
{
public:
	Tuple() :
		t1(0),
		t2(0)
	{};

	Tuple(T1 in1, T2 in2) :
		t1(in1),
		t2(in2)
	{};

	T1 t1;
	T2 t2;
};
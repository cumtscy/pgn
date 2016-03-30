namespace pgn {

template<class T, int size>
class Queue
{
public:
	T a[size + 1];
	int begin, end;

	Queue()
	{
		begin = 0;
		end = 0;
	}

	void push(T e)
	{
		a[end++] = e;
		end %= size + 1;
	}

	void pop()
	{
		begin++;
		begin %= size + 1;
	}

	T front()
	{
		return a[begin];
	}

	bool empty()
	{
		return begin == end;
	}
};

}

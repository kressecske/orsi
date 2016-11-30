#include <queue>
#include <mutex>
#include <condition_variable>

template <typename T>
class Pipeline
{
public:
	void push(const T& d)
	{
		std::unique_lock<std::mutex> lk(_m);
		_q.push(d);
		_cv.notify_all();
	}
	
	T pop()
	{
		std::unique_lock<std::mutex> lk(_m);
		if(_q.empty())
		{
			_cv.wait(lk, [=]{ return !_q.empty(); });
		}
				   
		T data = _q.front();
		_q.pop();
		return data;
	}
private:
	std::queue<T> _q;
	std::mutex _m;
	std::condition_variable _cv;
};
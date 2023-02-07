//
// Created by bear on 2/6/2023.
//

#pragma once

namespace lightlox
{
template<typename T>
class singleton
{
public:
	static T &instance()
	{
		static T _instance;
		return _instance;
	}
protected:
	singleton() = default;
	~singleton() = default;
	singleton(const singleton &s) = delete;
	singleton &operator=(const singleton &s) = delete;
};
}
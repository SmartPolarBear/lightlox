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
	static singleton& getInstance(){
		static singleton instance;
		// volatile int dummy{};
		return instance;
	}
protected:
	~singleton() = default;
private:
	singleton() = default;
	singleton(const singleton &) = delete;
	singleton &operator=(const singleton &) = delete;
};
}
//
// Created by bear on 2/8/2023.
//

#pragma once

#include <lightlox/generator.h>

#include <variant>
#include <vector>

namespace lightlox
{

using annotation = std::variant<int>;

class annotatable
{
public:
	void annotate(annotation anno)
	{
		annotations_.push_back(anno);
	}

	generator<annotation> annotations()
	{
		for (auto anno: annotations_)
		{
			co_yield anno;
		}
		co_return;
	}

private:
	std::vector<annotation> annotations_{};
};
}

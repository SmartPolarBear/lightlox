//
// Created by bear on 2/8/2023.
//

#pragma once

#include <variant>
#include <memory>

namespace lightlox
{
using expression = std::variant<std::unique_ptr<class binary_expression>,
								std::unique_ptr<class unary_expression>,
								std::unique_ptr<class literal_expression>>;

}

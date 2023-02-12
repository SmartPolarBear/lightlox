//
// Created by bear on 2/8/2023.
//

#pragma once

#include <variant>
#include <memory>

namespace lightlox
{
using expression = std::variant< std::unique_ptr<class binary_expression>>;

using expression_owned = std::unique_ptr<expression>;
using expression_shared = std::shared_ptr<expression>;
}

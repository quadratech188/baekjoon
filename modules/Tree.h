#pragma once

#include "Graph.h"

template <typename T>
concept Tree = Graph<T> && requires(T tree, typename T::index_t v) {
	{tree.parent(v)} -> std::same_as<typename T::index_t>;
	{tree.root()} -> std::same_as<typename T::index_t>;
};

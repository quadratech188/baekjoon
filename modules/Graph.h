#pragma once

#include <concepts>
#include <ranges>

template <typename G>
concept Graph = requires(G graph,
		typename G::child_ref child,
		typename G::index_t u,
		typename G::vertex_t v,
		typename G::edge_t e) {

	typename G::index_t;
	typename G::vertex_t;
	typename G::edge_t;
	typename G::template storage_t<int>;
	typename G::size_t;
	typename G::child_ref;

	{child.index()} -> std::same_as<typename G::index_t>;
	{child.edge()} -> std::same_as<typename G::edge_t&>;
	requires std::convertible_to<typename G::child_ref, typename G::index_t>;

	{graph.size()} -> std::same_as<typename G::size_t>;

	{graph.children(u)} -> std::ranges::range;
	requires std::same_as<std::ranges::range_value_t<decltype(graph.children(u))>, typename G::child_ref>;
};

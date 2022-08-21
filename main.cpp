// Copyright (C) 2022 Jonathan Müller and dryad contributors
// SPDX-License-Identifier: BSL-1.0

#include <cstdio>

#include <dryad/node.hpp>
#include <dryad/node_list.hpp>
#include <dryad/tree.hpp>

enum class node_kind
{
    container,
    leaf
};

struct leaf_node : dryad::define_node<node_kind::leaf>
{
    const char* msg;

    leaf_node(dryad::node_ctor ctor, const char* msg) : base_node(ctor), msg(msg) {}
};

struct container_node : dryad::define_node<node_kind::container, dryad::node_list<node_kind>>
{
    DRYAD_NODE_CTOR;
};

int main()
{
    dryad::tree<node_kind> tree;

    auto a         = tree.create<leaf_node>("a");
    auto b         = tree.create<leaf_node>("b");
    auto c         = tree.create<leaf_node>("c");
    auto container = tree.create<container_node>();
    container->insert_front(c);
    container->insert_front(b);
    auto a_iter = container->insert_front(a);
    container->erase_after(a_iter);
    tree.set_root(container);

    for (auto [ev, node] : dryad::traverse(tree))
    {
        if (auto leaf = dryad::node_try_cast<leaf_node>(node))
            std::puts(leaf->msg);
        else
            std::puts("container");
    }
}


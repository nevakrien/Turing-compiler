#ifndef LINEAR_FUSE_HPP
#define LINEAR_FUSE_HPP

#include "code_tree.hpp"



struct LinearFuse final: public CodeTree::CodeNode{
	TapeValMap write_ops;
	int move_offset;
	std::unique_ptr<CodeNode> next;

	LinearFuse(TapeValMap write_ops,int move_offset, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::LinearFuse, &next, 1), write_ops(write_ops),move_offset(move_offset),next(std::move(next_node)) {}

	TapeVal read_value() const override {
        return write_ops[0];
    }
};

struct HistoryNode final: public CodeTree::CodeNode{
	RunTimeValMap knowen_stuff;
	std::unique_ptr<CodeNode> next;

	HistoryNode(RunTimeValMap knowen_stuff, std::unique_ptr<CodeNode> next_node)
        : CodeNode(NodeTypes::HistoryNode, &next, 1), knowen_stuff(knowen_stuff),next(std::move(next_node)) {}

	TapeVal read_value() const override {
        return run_to_tape(knowen_stuff[0]);
    }
};



#endif // LINEAR_FUSE_HPP

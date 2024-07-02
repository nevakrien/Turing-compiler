#include "code_tree.hpp"
#include <cassert>
#include <unordered_set>
#include <memory>
#include <iostream>
#include <random>

using namespace CodeTree;

bool validate_tree(CodeNode* node, std::unordered_set<CodeNode*>* visited) {
    assert(node != nullptr);
    assert(visited != nullptr);
    
    volatile TapeVal dump = node->read_value(); // make sure that reading the value works
    volatile bool dump2 = node->is_final();

    if (!visited->insert(node).second) {
        if (node->type() == NodeTypes::StateStart) {
            return true; // only states are allowed to be reachable from more than 1 angle
        }
        return false;
    }

    for (int i = 0; i < node->len_next(); i++) {
        CodeNode* next = node->next_nodes()[i].get();
        if (next == nullptr) {
            return false;
        }
        if (!validate_tree(next, visited)) {
            return false;
        }
    }
    return true;
}

// Global random number generator setup
std::random_device rd;
std::mt19937 rng(rd());

std::unique_ptr<CodeNode> make_random_stuff(int len) {
    if (len <= 0) {
        return std::make_unique<Exit>(HALT);
    }
    switch (rng() % 4) {
        case 0:
            return std::make_unique<Split>(make_random_stuff(len - 4), make_random_stuff(len - 4));
        case 1:
            return std::make_unique<Write>((TapeVal)(rng() % 4), make_random_stuff(len - 1));
        case 2:
            return std::make_unique<Move>((int)rng() % 10 - 5, make_random_stuff(len - 1));
        case 3:
            return std::make_unique<Exit>(HALT);
    }
    return nullptr;
}

CodeNode* get_random_pre_end(StateStart* start) {
    CodeNode* cur = start;
    CodeNode* next = (cur->next_nodes())[0].get();
    while (next) {
        if (next->type() == NodeTypes::StateEnd) {
            return cur;
        }
        cur = next;
        if (!cur->len_next()) {
            return cur;
        }
        next = (cur->next_nodes())[rng() % cur->len_next()].get();
    }
    return cur;
}

bool validate_owned_tree_recursive(CodeNode* node) {
    assert(node != nullptr);

    volatile TapeVal dump = node->read_value(); // make sure that reading the value works
    volatile bool dump2 = node->is_final();

    for (int i = 0; i < node->get_owned_next_len(); i++) {
        CodeNode* next = node->get_owned_next()[i].get();
        if (next == nullptr) {
            return false;
        }
        if (!validate_owned_tree_recursive(next)) {
            return false;
        }
    }
    return true;
}

int main_test() {
    std::unique_ptr<StateStart> root = std::make_unique<StateStart>(0, std::make_unique<Exit>(HALT));
    std::unordered_set<CodeNode*> visited;

    if (!validate_tree(root.get(), &visited)) {
        std::cerr << "Error with simple case\n";
        return 1;
    }
    visited.clear();

    if (!validate_owned_tree_recursive(root.get())) {
        std::cerr << "Error with simple case (owned next)\n";
        return 1;
    }
    visited.clear();

    std::vector<std::unique_ptr<CodeNode>> states = {};
    states.push_back(std::make_unique<StateStart>(0, make_random_stuff(32)));

    if (!validate_tree(states[0].get(), &visited)) {
        std::cerr << "Error with tree case\n";
        return 1;
    }
    visited.clear();

    if (!validate_owned_tree_recursive(states[0].get())) {
        std::cerr << "Error with tree case (owned next)\n";
        return 1;
    }

    for (int i = 1; i < 100; i++) {
        states.push_back(std::make_unique<StateStart>(i, make_random_stuff(8)));

        int insert_state = rng() % states.size();
        CodeNode* insert_spot = get_random_pre_end(static_cast<StateStart*>(states[insert_state].get()));

        auto t1 = dynamic_cast<Write*>(insert_spot);
        if (t1) {
            t1->next = std::make_unique<StateEnd>(i, &t1->next, &states[i]);
            continue;
        }

        auto t2 = dynamic_cast<Move*>(insert_spot);
        if (t2) {
            t2->next = std::make_unique<StateEnd>(i, &t2->next, &states[i]);
            continue;
        }

        auto t3 = dynamic_cast<Split*>(insert_spot);
        if (t3) {
            int selected = rng() % 2;
            CodeNode* check = t3->sides[selected].get();
            if (dynamic_cast<Exit*>(check) || dynamic_cast<StateEnd*>(check)) {
                t3->sides[selected] = std::make_unique<StateEnd>(i, &t3->sides[selected], &states[i]);
            } else {
                t3->sides[1 - selected] = std::make_unique<StateEnd>(i, &t3->sides[1 - selected], &states[i]);
            }
            continue;
        }
    }

    for (int i = 0; i < states.size(); i++) {
        visited.clear();
        if (!validate_tree(states[i].get(), &visited)) {
            std::cerr << "Error with tree case\n";
            return 1;
        }
    }

    for (int i = 0; i < states.size(); i++) {
        if (!validate_owned_tree_recursive(states[i].get())) {
            std::cerr << "Error with tree case (owned next)\n";
            return 1;
        }
    }

    return 0;
}

int main() {
    for (int i = 0; i < 100; i++) {
        if (main_test()) {
            return 1;
        }
    }
    std::cout << "Passed code_tree test\n";
    return 0;
}

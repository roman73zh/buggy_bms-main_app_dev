#include "Input.h"

Input::Input(Block* block, std::string id, std::string name) {
    this->id = id;
    this->name = name;
    block->addInput(this);
}
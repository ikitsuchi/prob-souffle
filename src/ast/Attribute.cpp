/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2020, The Souffle Developers. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

#include "ast/Attribute.h"
#include "souffle/utility/DynamicCasting.h"
#include <ostream>
#include <utility>

namespace souffle::ast {

Attribute::Attribute(std::string n, QualifiedName t, SrcLocation loc)
        : Node(NK_Attribute, std::move(loc)), name(std::move(n)), typeName(std::move(t)), isLattice(false) {}

Attribute::Attribute(std::string n, QualifiedName t, bool isLattice, SrcLocation loc)
        : Node(NK_Attribute, std::move(loc)), name(std::move(n)), typeName(std::move(t)),
          isLattice(isLattice) {}

void Attribute::setTypeName(QualifiedName name) {
    typeName = std::move(name);
}

void Attribute::print(std::ostream& os) const {
    os << name << ":" << typeName;
    if (isLattice) {
        os << "<>";
    }
}

bool Attribute::equal(const Node& node) const {
    const auto& other = asAssert<Attribute>(node);
    return name == other.name && typeName == other.typeName && isLattice == other.isLattice;
}

Attribute* Attribute::cloning() const {
    return new Attribute(name, typeName, isLattice, getSrcLoc());
}

bool Attribute::classof(const Node* n) {
    return n->getKind() == NK_Attribute;
}

bool Attribute::getIsLattice() const {
    return isLattice;
}

}  // namespace souffle::ast

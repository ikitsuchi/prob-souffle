/*
 * Souffle - A Datalog Compiler
 * Copyright (c) 2021, The Souffle Developers. All rights reserved
 * Licensed under the Universal Permissive License v 1.0 as shown at:
 * - https://opensource.org/licenses/UPL
 * - <souffle root>/licenses/SOUFFLE-UPL.txt
 */

/************************************************************************
 *
 * @file DefineTuple.h
 *
 ***********************************************************************/

#pragma once

#include "ram/Expression.h"
#include "ram/NestedOperation.h"
#include "ram/TupleOperation.h"
#include "ram/Node.h"
#include "ram/Operation.h"
#include "ram/Relation.h"
#include "ram/Statement.h"
#include "souffle/utility/ContainerUtil.h"
#include "souffle/utility/MiscUtil.h"
#include "souffle/utility/StreamUtil.h"
#include <cassert>
#include <iosfwd>
#include <memory>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace souffle::ram {

class Derivation : public NestedOperation {
public:
    Derivation(std::string head_relation_name, VecOwn<ram::Expression> values, Own<Operation> op, bool add_edge = true)
            : NestedOperation(NK_Derivation, std::move(op)),
              head_relation_name(std::move(head_relation_name)), values(std::move(values)),
              add_edge(add_edge) {}

    void apply(const NodeMapper& map) override {
        NestedOperation::apply(map);
        for (auto& expr : values) {
            expr = map(std::move(expr));
        }
    }

    Derivation* cloning() const override {
        VecOwn<Expression> newValues;
        for (auto& expr : values) {
            newValues.emplace_back(expr->cloning());
        }
        return new Derivation(head_relation_name, std::move(newValues), clone(getOperation()), add_edge);
    }

    const std::string& getHeadRelationName() const {
        return head_relation_name;
    }

    std::vector<Expression*> getValues() const {
        return toPtrVector(values);
    }

    static bool classof(const Node* n) {
        return n->getKind() == NK_Derivation;
    }

    bool getAddEdge() const {
        return add_edge;
    }

protected:
    void print(std::ostream& os, int tabpos) const override {
        os << times(" ", tabpos) << "DERIVE " << head_relation_name << "("
           << join(values, ",", [](std::ostream& out, const Own<Expression>& value) { out << *value; })
           << ") "
           << "ADD_EDGE=" << (add_edge ? "true" : "false") << std::endl;
        NestedOperation::print(os, tabpos + 1);
    }

    bool equal(const Node& node) const override {
        const auto& other = asAssert<Derivation>(node);
        return NestedOperation::equal(node) && head_relation_name == other.head_relation_name &&
               equal_targets(values, other.values) && add_edge == other.add_edge;
    }

    NodeVec getChildren() const override {
        return NestedOperation::getChildren();
    }

    std::string head_relation_name;
    VecOwn<ram::Expression> values;
    bool add_edge = true;
};

}  // namespace souffle::ram

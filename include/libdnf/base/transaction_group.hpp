/*
Copyright Contributors to the libdnf project.

This file is part of libdnf: https://github.com/rpm-software-management/libdnf/

Libdnf is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 2.1 of the License, or
(at your option) any later version.

Libdnf is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with libdnf.  If not, see <https://www.gnu.org/licenses/>.
*/


#ifndef LIBDNF_BASE_TRANSACTION_GROUP_HPP
#define LIBDNF_BASE_TRANSACTION_GROUP_HPP

#include "libdnf/base/goal_elements.hpp"
#include "libdnf/comps/group/group.hpp"
#include "libdnf/rpm/package.hpp"
#include "libdnf/transaction/transaction_item_action.hpp"
#include "libdnf/transaction/transaction_item_reason.hpp"
#include "libdnf/transaction/transaction_item_state.hpp"

#include <optional>


namespace libdnf::base {

class TransactionGroup {
public:
    using Reason = transaction::TransactionItemReason;
    using State = transaction::TransactionItemState;
    using Action = transaction::TransactionItemAction;

    /// @return the underlying group.
    libdnf::comps::Group get_group() const { return group; }

    /// @return the action being preformed on the transaction group.
    //
    // @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.getAction()
    Action get_action() const noexcept { return action; }

    /// @return the state of the group in the transaction.
    //
    // @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.getState()
    State get_state() const noexcept { return state; }

    /// @return the reason of the action being performed on the transaction group.
    //
    // @replaces libdnf:transaction/TransactionItem.hpp:method:TransactionItemBase.getReason()
    Reason get_reason() const noexcept { return reason; }

public:
    friend class Transaction;

    TransactionGroup(const libdnf::comps::Group & grp, Action action, Reason reason)
        : group(grp),
          action(action),
          reason(reason) {}

    void set_state(State value) noexcept { state = value; }

    libdnf::comps::Group group;
    Action action;
    Reason reason;
    State state{State::STARTED};
};

}  // namespace libdnf::base

#endif  // LIBDNF_BASE_TRANSACTION_GROUP_HPP

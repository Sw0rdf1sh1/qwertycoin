// Copyright (c) 2012-2017, The CryptoNote developers, The Bytecoin developers
// Copyright (c) 2018-2023, The Qwertycoin Group.
//
// This file is part of Qwertycoin.
//
// Qwertycoin is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Qwertycoin is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with Qwertycoin.  If not, see <http://www.gnu.org/licenses/>.

#pragma once

#include <CryptoNoteCore/CryptoNoteFormatUtils.h>
#include <CryptoNoteCore/TransactionExtra.h>

namespace CryptoNote {

class TransactionExtra
{
public:
    TransactionExtra() = default;
    TransactionExtra(const std::vector<uint8_t> &extra)
    {
        parse(extra);
    }

    bool parse(const std::vector<uint8_t> &extra)
    {
        fields.clear();
        return CryptoNote::parseTransactionExtra(extra, fields);
    }

    template <typename T>
    bool get(T &value) const
    {
        auto it = find(typeid(T));
        if (it == fields.end()) {
            return false;
        }

        value = boost::get<T>(*it);

        return true;
    }

    template <typename T>
    void set(const T &value)
    {
        auto it = find(typeid(T));
        if (it != fields.end()) {
            *it = value;
        } else {
            fields.push_back(value);
        }
    }

    template <typename T>
    void append(const T &value)
    {
        fields.push_back(value);
    }

    bool getPublicKey(Crypto::PublicKey &pk) const
    {
        CryptoNote::TransactionExtraPublicKey extraPk;
        if (!get(extraPk)) {
            return false;
        }

        pk = extraPk.publicKey;

        return true;
    }

    std::vector<uint8_t> serialize() const
    {
        std::vector<uint8_t> extra;
        writeTransactionExtra(extra, fields);

        return extra;
    }

private:
    std::vector<CryptoNote::TransactionExtraField>::const_iterator find(const std::type_info &t) const
    {
        return std::find_if(fields.begin(), fields.end(), [&t](const TransactionExtraField& f) {
            return t == f.type();
        });
    }

    std::vector<CryptoNote::TransactionExtraField>::iterator find(const std::type_info &t)
    {
        return std::find_if(fields.begin(), fields.end(), [&t](const TransactionExtraField& f) {
            return t == f.type();
        });
    }

private:
    std::vector<CryptoNote::TransactionExtraField> fields;
};

} // namespace CryptoNote

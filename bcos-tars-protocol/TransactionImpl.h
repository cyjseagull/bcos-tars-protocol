/**
 *  Copyright (C) 2021 FISCO BCOS.
 *  SPDX-License-Identifier: Apache-2.0
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 * @brief tars implementation for Transaction
 * @file TransactionImpl.h
 * @author: ancelmo
 * @date 2021-04-20
 */

#pragma once
#include "Common.h"
#include "Transaction.h"
#include "bcos-framework/libutilities/DataConvertUtility.h"
#include <bcos-framework/interfaces/crypto/CommonType.h>
#include <bcos-framework/interfaces/protocol/Transaction.h>
#include <bcos-framework/libutilities/Common.h>
#include <memory>

namespace bcostars
{
namespace protocol
{
class TransactionImpl : public bcos::protocol::Transaction
{
public:
    explicit TransactionImpl(
        bcos::crypto::CryptoSuite::Ptr _cryptoSuite, std::function<bcostars::Transaction*()> inner)
      : bcos::protocol::Transaction(_cryptoSuite), m_inner(inner)
    {}

    ~TransactionImpl() {}

    friend class TransactionFactoryImpl;

    bool operator==(const Transaction& rhs) const { return this->hash() == rhs.hash(); }

    void decode(bcos::bytesConstRef _txData) override;
    bcos::bytesConstRef encode(bool _onlyHashFields = false) const override;

    bcos::crypto::HashType const& hash() const override;
    int32_t version() const override { return m_inner()->data.version; }
    std::string_view chainId() const override { return m_inner()->data.chainID; }
    std::string_view groupId() const override { return m_inner()->data.groupID; }
    int64_t blockLimit() const override { return m_inner()->data.blockLimit; }
    bcos::u256 const& nonce() const override;
    std::string_view to() const override { return m_inner()->data.to; }
    bcos::bytesConstRef input() const override;
    int64_t importTime() const override { return m_inner()->importTime; }
    void setImportTime(int64_t _importTime) override { m_inner()->importTime = _importTime; }
    bcos::bytesConstRef signatureData() const override
    {
        return bcos::bytesConstRef(reinterpret_cast<const bcos::byte*>(m_inner()->signature.data()),
            m_inner()->signature.size());
    }

    void setSignatureData(bcos::bytes& signature)
    {
        m_inner()->signature.assign(signature.begin(), signature.end());
    }

    const bcostars::Transaction& inner() const { return *m_inner(); }

    void setInner(const bcostars::Transaction& inner) { *m_inner() = inner; }

    void setInner(bcostars::Transaction&& inner) { *m_inner() = std::move(inner); }
    std::function<bcostars::Transaction*()> const& innerGetter() { return m_inner; }

private:
    std::function<bcostars::Transaction*()> m_inner;
    mutable bcos::bytes m_buffer;
    mutable bcos::bytes m_dataBuffer;
    mutable bcos::u256 m_nonce;
};
}  // namespace protocol
}  // namespace bcostars
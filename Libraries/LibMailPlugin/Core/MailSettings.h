// Copyright (c) 2024-2025, LLC Orion.

#pragma once

#include "Core/Identity.h"
#include "Core/JumpBackend/Rule.h"
#include "Core/Signature.h"
#include "LibBase/Forward.h"
#include "LibBase/Lazy.h"

class MailSettings
{
public:
    MailSettings();
    ~MailSettings();

    void addOrUpdateSignature(Signature);
    void removeSignature(const Signature::Id &);
    void setSignatures(Signatures);
    [[nodiscard]] Signatures signatures() const;
    [[nodiscard]] std::optional<Signature> signature(Signature::UseFlag) const;

    void setIdentities(QList<Identity>);
    QList<Identity> identities() const;

    void addRule(Rule);
    void deleteRule(const QString &name);
    QList<Rule> rules() const;

private:
    [[nodiscard]] Signatures getSignatures() const;
    [[nodiscard]] QList<Identity> getIdentities() const;
    [[nodiscard]] QList<Rule> getRules() const;

    void saveRules();

    QString load(const QString &) const { return {}; }
    void save(const QString &, const QString &) {}

    mutable LibBase::Lazy<Signatures> _signatures;
    mutable LibBase::Lazy<QList<Identity>> _identities;
    mutable LibBase::Lazy<QList<Rule>> _rules;
};

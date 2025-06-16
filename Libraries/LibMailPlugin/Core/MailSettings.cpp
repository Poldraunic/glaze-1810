// Copyright (c) 2024-2025, LLC Orion.

#include "Core/MailSettingsTest.h"
#include "GlazeQtLayer.h"
#include "LibBase/EmailAddress.h"

#include <QDebug>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>

using namespace Qt::StringLiterals;

namespace KnownMailSettingsNames {

namespace SpecialMailboxes {

static const auto sentItems { u"SentMailbox"_s };
static const auto drafts { u"DraftsMailbox"_s };
static const auto trash { u"TrashMailbox"_s };

} // namespace SpecialMailboxes

static const auto Signatures = u"Signatures"_s;
static const auto Identities = u"Identities"_s;
static const auto rules = u"Rules"_s;

} // namespace KnownMailSettingsNames

MailSettings::MailSettings()
    : _signatures([this] { return getSignatures(); })
    , _identities([this] { return getIdentities(); })
    , _rules([this] { return getRules(); })
{
}

MailSettings::~MailSettings() = default;

void MailSettings::addOrUpdateSignature(Signature signature)
{
    _signatures->addOrUpdate(ORION_MOVE(signature));
    save(KnownMailSettingsNames::Signatures, _signatures->toJson());
}

void MailSettings::removeSignature(const Signature::Id &id)
{
    _signatures->remove(id);
    save(KnownMailSettingsNames::Signatures, _signatures->toJson());
}

void MailSettings::setSignatures(Signatures signatures)
{
    _signatures.set(ORION_MOVE(signatures));
    save(KnownMailSettingsNames::Signatures, _signatures->toJson());
}

Signatures MailSettings::signatures() const
{
    return _signatures.get();
}

std::optional<Signature> MailSettings::signature(Signature::UseFlag usageFlag) const
{
    return _signatures->get(usageFlag);
}

void MailSettings::setIdentities(QList<Identity> identities)
{
    _identities.set(ORION_MOVE(identities));

    QJsonArray array;
    for (const auto &identity : std::as_const(_identities.get())) {
        array.append(identity.toJson());
    }

    save(KnownMailSettingsNames::Identities, QJsonDocument(array).toJson(QJsonDocument::JsonFormat::Compact));
}

QList<Identity> MailSettings::identities() const
{
    return _identities.get();
}

void MailSettings::addRule(Rule rule)
{
    _rules->append(ORION_MOVE(rule));
    saveRules();
}

void MailSettings::deleteRule(const QString &name)
{
    _rules->removeIf([&name](const Rule &rule) { return rule.name == name; });
    saveRules();
}

QList<Rule> MailSettings::rules() const
{
    return _rules.get();
}

Signatures MailSettings::getSignatures() const
{
    return Signatures::fromJson(load(KnownMailSettingsNames::Signatures).toUtf8());
}

QList<Identity> MailSettings::getIdentities() const
{
    const auto array = QJsonDocument::fromJson(load(KnownMailSettingsNames::Identities).toUtf8()).array();
    return Identity::fromJsonArray(array);
}

QList<Rule> MailSettings::getRules() const
{
    const auto byteArray = load(KnownMailSettingsNames::rules).toUtf8();
    const auto buffer = std::string_view(byteArray.data(), byteArray.size());
    auto result = glz::read_json<QList<Rule>>(buffer);
    if (!result.has_value()) {
        qWarning().noquote() << "[Mail] Failed to parse rules:" << result.error().custom_error_message;
        return {};
    }

    return ORION_MOVE(result).value();
}

void MailSettings::saveRules()
{
    const auto result = glz::write_json(_rules.get());
    if (!result.has_value()) {
        qWarning().noquote() << "[Mail] Failed to write rules:" << result.error().custom_error_message;
        return;
    }

    save(KnownMailSettingsNames::rules, QString::fromStdString(result.value()));
}

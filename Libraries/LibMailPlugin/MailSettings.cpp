#include "MailSettings.h"

#include <QDebug>

#include <glaze/glaze.hpp>

namespace glz {

template <>
struct from<JSON, QString>
{
    template <auto Opts>
    static void op(QString &value, auto &&...args)
    {
        std::string buffer;
        parse<JSON>::op<Opts>(buffer, args...);
        value = QString::fromStdString(buffer);
    }
};

template <>
struct to<JSON, QString>
{
    template <auto Opts>
    static void op(const QString &value, auto &&...args) noexcept
    {
        serialize<JSON>::op<Opts>(value.toStdString(), args...);
    }
};

} // namespace glz

namespace SettingKey {

using namespace Qt::StringLiterals;

static const auto signatures = u"Signatures"_s;
static const auto identities = u"Identities"_s;
static const auto favoriteMailboxes = u"FavoriteMailboxes"_s;
static const auto rules = u"Rules"_s;

} // namespace SettingKey

MailSettings::MailSettings()
    : _signatures([this] { return getSignatures(); })
    , _identities([this] { return getIdentities(); })
    , _favorites([this] { return getFavoriteMailboxes(); })
    , _rules([this] { return getRules(); })
{
}

MailSettings::~MailSettings() = default;

void MailSettings::addOrUpdateSignature(Signature signature)
{
    _signatures->removeIf([signature](const auto &other) { return other.id == signature.id; });
    _signatures->append(signature);
    save(SettingKey::signatures, {});
}

void MailSettings::removeSignature(const QString &id)
{
    _signatures->removeIf([id](const auto &other) { return other.id == id; });
    save(SettingKey::signatures, {});
}

void MailSettings::setSignatures(QList<Signature> signatures)
{
    _signatures.set(std::move(signatures));
    save(SettingKey::signatures, {});
}

QList<Signature> MailSettings::signatures() const
{
    return _signatures.get();
}

std::optional<Signature> MailSettings::signature(const QString &id) const
{
    const auto it = std::ranges::find_if(_signatures.get(), [id](const auto &other) { return other.id == id; });
    if (it == _signatures->end()) {
        return {};
    }

    return *it;
}

void MailSettings::setIdentities(QList<Identity> identities)
{
    _identities.set(std::move(identities));

    save(SettingKey::identities, {});
}

QList<Identity> MailSettings::identities() const
{
    return _identities.get();
}

void MailSettings::addFavorite(const QString &value)
{
    _favorites->append(value);
    save(SettingKey::favoriteMailboxes, QString());
}

void MailSettings::removeFavote(const QString &value)
{
    _favorites->removeOne(value);
    save(SettingKey::favoriteMailboxes, QString());
}

bool MailSettings::isFavorite(const QString &value) const
{
    return _favorites->contains(value);
}

QList<QString> MailSettings::favoriteMailboxes() const
{
    return _favorites.get();
}

void MailSettings::renameFavorite(const QString &oldName, const QString &newName)
{
    _favorites->removeOne(oldName);
    _favorites->append(newName);
    save(SettingKey::favoriteMailboxes, QString());
}

void MailSettings::addRule(Rule rule)
{
    _rules->append(std::move(rule));
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

QList<Signature> MailSettings::getSignatures() const
{
    return {};
}

QList<Identity> MailSettings::getIdentities() const
{
    return {};
}

QList<QString> MailSettings::getFavoriteMailboxes() const
{
    return {};
}

QList<Rule> MailSettings::getRules() const
{
    const auto buffer = load(SettingKey::rules).toUtf8();
    const auto view = std::string_view(buffer.data(), buffer.size());
    auto result = glz::read_json<QList<Rule>>(view);
    if (!result.has_value()) {
        qWarning().noquote() << "Failed to parse rules:" << static_cast<int>(result.error().ec);
        return {};
    }

    return std::move(result).value();
}

void MailSettings::saveRules()
{
    const auto json = glz::write_json(_rules.get());
    if (!json.has_value()) {
        qWarning().noquote() << "Failed to write rules:" << static_cast<int>(json.error().ec);
        return;
    }

    save(SettingKey::rules, QString::fromStdString(json.value()));
}

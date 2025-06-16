#pragma once

#include "Identity.h"
#include "Lazy.h"
#include "Rule.h"
#include "Signature.h"

#include <QObject>

class MailSettings : public QObject
{
    Q_OBJECT

public:
    MailSettings();
    ~MailSettings();

    void addOrUpdateSignature(Signature);
    void removeSignature(const QString &id);
    void setSignatures(QList<Signature>);
    [[nodiscard]] QList<Signature> signatures() const;
    [[nodiscard]] std::optional<Signature> signature(const QString &id) const;

    void setIdentities(QList<Identity>);
    QList<Identity> identities() const;

    void addFavorite(const QString &value);
    void removeFavote(const QString &value);
    [[nodiscard]] bool isFavorite(const QString &value) const;
    [[nodiscard]] QList<QString> favoriteMailboxes() const;
    void renameFavorite(const QString &oldName, const QString &newName);

    void addRule(Rule);
    void deleteRule(const QString &name);
    QList<Rule> rules() const;

private:
    [[nodiscard]] QList<Signature> getSignatures() const;
    [[nodiscard]] QList<Identity> getIdentities() const;
    [[nodiscard]] QList<QString> getFavoriteMailboxes() const;
    [[nodiscard]] QList<Rule> getRules() const;

    void saveRules();

    QString load(const QString &) const { return {}; }
    void save(const QString &, const QString &) {}

    mutable LibBase::Lazy<QList<Signature>> _signatures;
    mutable LibBase::Lazy<QList<Identity>> _identities;
    mutable LibBase::Lazy<QList<QString>> _favorites;
    mutable LibBase::Lazy<QList<Rule>> _rules;
};

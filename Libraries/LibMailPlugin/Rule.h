#include <QList>
#include <QString>

#include <optional>

enum class Condition {
    From,
    To,
    Cc,
    Subject,
    Size,
};

enum class Operator {
    Contains,
    DoesNotContain,
    Equals,
    DoesNotEqual,
    LessThan,
    GreaterThan,
};

enum class Action {
    Discard,
    Reject,
    MoveInto,
    AddFlags,
};

struct ConditionObject
{
    Condition condition;
    Operator operator_;
    QString value;
};

struct ActionObject
{
    Action action;
    std::optional<QString> value;
};

struct Rule
{
    QString name;
    int priority;
    QList<ConditionObject> conditions;
    QList<ActionObject> actions;
    bool allConditionsMustSucceed;
};

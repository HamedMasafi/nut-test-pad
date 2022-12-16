#ifndef NUT_NAMED_TYPE_H
#define NUT_NAMED_TYPE_H
#include <bits/move.h>
#include <stdarg.h>
#include <tuple>
#include <vector>
#include <cstring>
#include <QPair>
#include <QVariant>
#include <QDebug>

namespace Nut { namespace Model {

class NamedParam {
public:
    const char *name;
    union {
        int n;
        double d;
        bool b;
        const char *s;
        std::pair<int, int> pair;
    };

    constexpr inline NamedParam(const char *name) : name{name}, n{}
    {}
    constexpr inline NamedParam(const char *name, int value) : name{name}, n{value}
    {}
    constexpr inline NamedParam(const char *name, double value) : name{name}, d{value}
    {}
    constexpr inline NamedParam(const char *name, bool value) : name{name}, b{value}
    {}
    constexpr inline NamedParam(const char *name, const char *value) : name{name}, s{value}
    {}
    constexpr inline NamedParam(const char *name, std::pair<int, int> value) : name{name}, pair{value}
    {}
};
template <typename T>
class NamedParamType {
public:
    const char *name;
};

template <typename T>
constexpr inline T get(const Nut::Model::NamedParam &d) {
    (void)d;
    return T{};
}

template <>
constexpr inline int get<int>(const Nut::Model::NamedParam &d) {
    return d.n;
}

template <>
constexpr inline double get<double>(const Nut::Model::NamedParam &d) {
    return d.d;
}

template <>
constexpr inline bool get<bool>(const Nut::Model::NamedParam &d) {
    return d.b;
}

template <>
constexpr inline const char* get<const char*>(const Nut::Model::NamedParam &d) {
    return d.s;
}

template <>
constexpr inline std::pair<int,int> get<std::pair<int,int> >(const Nut::Model::NamedParam &d) {
    return d.pair;
}

template<typename T, typename... Types>
struct contains;

template<typename T, typename First>
struct contains<T, First> {
    const static bool value = std::is_same<T, First>::value;
};

template<typename T, typename First, typename... Types>
struct contains<T, First, Types...> {
    const static bool value = sizeof...(Types) == 0 ? std::is_same<T, First>::value :
                                  (std::is_same<T, First>::value ? true : contains<T, Types...>::value);
};


template<typename T, typename... Types>
bool pick(const char *name, T *buffer, Types &...args)
{
//    Nut::Model::NamedParam array2[] {args...};
//    constexpr int size = sizeof...(args);

    std::vector<Nut::Model::NamedParam> array = {args...};
    for (auto &a: array)
        if (!strcmp(a.name, name)) {
            *buffer = get<T>(a);
            return true;
        }
    return false;
}

template<typename... Types>
bool pick(const char *name, Types &...args)
{
    std::vector<Nut::Model::NamedParam> array = {args...};
    for (auto &a: array)
        if (!strcmp(a.name, name)) {
            return true;
        }

    qDebug() << name << "not found";
    return false;
}

template<typename T, typename... Types>
constexpr int count2(const char *name, Types &...args)
{
    int c = 0;
    constexpr Model::NamedParam array[] {args...};
    constexpr int size = sizeof...(args);

    for (int i = 0; i < size; i++)
        if (!strcmp(array[i].name, name)) {
            c++;
        }

    return c;
}

template <typename T, typename... Ts>
constexpr bool containsType = (std::is_same<T, Ts>{} || ...);

template <typename T, typename... Ts>
constexpr int count = sizeof...(Ts) == 0 ? 0 : (((bool)std::is_same<T, Ts>::value ? 1 : 0) + ...);

template <typename...>
struct is_unique : std::integral_constant<size_t, 0> {};

template <typename T, typename U, typename... VV>
struct is_unique<T, U, VV...> : std::integral_constant<size_t, std::is_same<T, U>::value + is_unique<T, VV...>::value> {};

template <typename...>
struct no_unique : std::integral_constant<size_t, 0> {};

template <typename T, typename... UU>
struct no_unique<T, UU...> : std::integral_constant<size_t, is_unique<T, UU...>::value + no_unique<UU...>::value> {};

#define NamedParamSubClass(name, type)                                                             \
    class name : public Nut::Model::NamedParam                                                     \
    {                                                                                              \
    public:                                                                                        \
        name(const type &value) : Nut::Model::NamedParam(#name, value) {}                          \
    }

#define NamedParamSubClassVoid(name)                                                               \
    class name : public Nut::Model::NamedParam                                                     \
    {                                                                                              \
    public:                                                                                        \
        name() : Nut::Model::NamedParam(#name, true) {}                                            \
    }

class AutoIncrement : public Nut::Model::NamedParam
{
public:
    AutoIncrement(int from, int step)
        : Nut::Model::NamedParam("AutoIncrement", std::make_pair(from, step))
    {}
};

class ColumnName : public Nut::Model::NamedParam
{
public:
    ColumnName(const char *name) : Nut::Model::NamedParam("Name", name) {}
};

NamedParamSubClassVoid(PrimaryKey);
NamedParamSubClass(MaxLen, int);
NamedParamSubClass(Len, int);
NamedParamSubClass(AllowNull, bool);

} // namespace Model
} // namespace Nut


#endif // NUT_NAMED_TYPE_H

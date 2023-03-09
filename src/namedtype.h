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

namespace Nut { namespace ModelDeclartion {

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
constexpr inline T get(const NamedParam &d) {
    (void)d;
    return T{};
}

template <>
constexpr inline int get<int>(const NamedParam &d) {
    return d.n;
}

template <>
constexpr inline double get<double>(const NamedParam &d) {
    return d.d;
}

template <>
constexpr inline bool get<bool>(const NamedParam &d) {
    return d.b;
}

template <>
constexpr inline const char* get<const char*>(const NamedParam &d) {
    return d.s;
}

template <>
constexpr inline std::pair<int,int> get<std::pair<int,int> >(const NamedParam &d) {
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

template<typename... Types>
class Picker
{
    constexpr const static int _size = sizeof...(Types);
    NamedParam array[_size];// {args...};
//    std::vector<NamedParam> array;

public:
    constexpr explicit Picker(Types &...args)
        : array{args...}
    {
    };

    template<typename T>
    bool pick(const char *name, T *buffer)
    {
        for (auto &a: array)
            if (!strcmp(a.name, name)) {
                *buffer = get<T>(a);
                return true;
            }
        return false;
    }
};

template<typename T, typename... Types>
bool pick(const char *name, T *buffer, Types &...args)
{
    //    NamedParam array2[] {args...};
    //    constexpr int size = sizeof...(args);

    std::vector<NamedParam> array = {args...};
    for (auto &a: array)
        if (!strcmp(a.name, name)) {
            *buffer = get<T>(a);
            return true;
        }
    return false;
}

template<typename T, typename... Types>
T pick(Types &...args)
{
    std::vector<NamedParam> array = {args...};
    for (auto &a: array)
        if (std::is_same<typeof(a), T>::value) {
            return *static_cast<T*>(&a);
        }
    return T{};
}

template<typename... Types>
bool pick(const char *name, Types &...args)
{
    NamedParam array[] {args...};
    int size = sizeof...(args);

    for (int i = 0; i < size; i++)
        if (!strcmp(array[i].name, name)) {
            return true;
        }
    return false;
//    NamedParam array2[] = {args...};
//    std::vector<NamedParam> array = {args...};
//    for (auto &a: array)
//        if (!strcmp(a.name, name)) {
//            return true;
//        }

//    qDebug() << name << "not found";
//    return false;
}

template<typename T, typename... Types>
constexpr int count2(const char *name, Types &...args)
{
    int c = 0;
    constexpr NamedParam array[] {args...};
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

#define NamedParamSubClass(name, type) \
    class name : public NamedParam \
    { \
    public: \
        explicit constexpr name(const type &value) \
            : NamedParam(#name, value) \
        {} \
    }

#define NamedParamSubClassVoid(name) \
    class name : public NamedParam \
    { \
    public: \
        explicit constexpr name() \
            : NamedParam(#name, true) \
        {} \
    }

#define NamedParamSubClassString(name) \
    class name : public NamedParam \
    { \
    public: \
        explicit constexpr name() \
            : NamedParam(#name, nullptr) \
        {} \
        explicit constexpr name(const char *value) \
            : NamedParam(#name, value) \
        {} \
    };

class AutoIncrement : public NamedParam
{
public:
    AutoIncrement()
        : NamedParam("AutoIncrement", std::make_pair(-1, -1))
    {}
    AutoIncrement(int from, int step)
        : NamedParam("AutoIncrement", std::make_pair(from, step))
    {}
};

class ColumnName : public NamedParam
{
public:
    explicit constexpr ColumnName(const char *name) : NamedParam("Name", name)
    {}
};

NamedParamSubClassVoid(PrimaryKey);
NamedParamSubClass(MaxLen, int);
NamedParamSubClass(Len, int);
NamedParamSubClass(AllowNull, bool);
NamedParamSubClassString(RelationName);

} // namespace Model
} // namespace Nut

#endif // NUT_NAMED_TYPE_H

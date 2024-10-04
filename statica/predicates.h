/// Eternal voidness
template <typename...>
using Void = void;

/// Meta-Truth
struct MTrue { using type = void; };
/// Meta-Falsievness
struct MFalse {};

namespace internal {
    template <typename...> struct True: MTrue {};
    template <typename...> struct False: MFalse {};

    template <bool Cond> struct If: MFalse {};
    template <> struct If<true>: MTrue {};

    template <typename A, typename B> struct Eq: MFalse {};
    template <typename T> struct Eq<T, T>: MTrue {};

    template <template<typename...> typename P, typename = void, typename... Args>
    constexpr bool Bool = false;
    template <template<typename...> typename P, typename... Args>
    constexpr bool Bool<P, Void<P<Args...>>, Args...> = true;

    template <template<typename...> typename P, typename = void, typename... Args>
    struct MNot: MTrue {};
    template <template<typename...> typename P, typename... Args>
    struct MNot<P, Void<P<Args...>>, Args...>: MFalse {};

    template <typename T, typename... Ts>
    struct MOneOf: MFalse {};
    template <typename T, typename... Ts>
    struct MOneOf<T, T, Ts...>: MTrue {};
    template <typename T, typename V,  typename... Ts>
    struct MOneOf<T, V, Ts...>: MOneOf<T, Ts...> {};
}

/// Predicate which is always true
template <typename... Args>
using True = internal::True<Args...>::type;

/// Predicate which is always false
template <typename... Args>
using False = internal::False<Args...>::type;

/// Predicate for a boolean condition
template <bool Condition>
using If = internal::If<Condition>::type;

/// Predicate for type equivalence
template <typename A, typename B>
using Eq = internal::Eq<A, B>::type;

template <typename T, typename... Types>
using OneOf = internal::MOneOf<T, Types...>::type;

/// Converts any predicate to a boolean
template <template<typename...> typename Predicate, typename... Arguments>
constexpr bool Bool = internal::Bool<Predicate, void, Arguments...>;

/// p1 and p2 and ... and pn
template <typename... Predicates>
using Combine = Void<Predicates...>;

/// Type `T` must satisfy all `Predicates`
template <typename T, template<typename> typename... Predicates>
using SatisfyAll = Combine<Predicates<T>...>;

/// Type `T` must satisfy at least one of the `Predicates`
template <typename T, template<typename> typename... Predicates>
using SatisfySome = If<bigOr<Bool<Predicates, T>...>>; // Combine<Predicates<T>...>;

/// Invert the `Predicate`, must specify it in an unapplied form
template <template<typename...> typename Predicate, typename... Arguments>
using Not = internal::MNot<Predicate, void, Arguments...>::type;

/// MARK: Unusual ones
template <template<typename, typename> typename Predicate, typename Arg1>
struct MPartApply {
    template <typename Arg2>
    using type = Predicate<Arg1, Arg2>;
};

template <typename T>
using IsBool = MPartApply<Eq, bool>::type<T>;

//template <typename T>
//using Integer = True<>;

// Note: this implementation uses C++20 facilities
// Form cppreference, TODO: get it right
template<class T>
using Integer = If<requires (T t, T* p, void (*f)(T)) { 
    // T* parameter excludes reference types
    reinterpret_cast<T>(t); // Exclude class types
    f(0); // Exclude enumeration types
    p + t; // Exclude everything not yet excluded but integral types
}>;

template <typename T>
using Real = OneOf<T, float, double, long double>;

template <typename T>
using Number = SatisfySome<T, Integer, Real>;


#define SingleGenericStruct(T, name) \
template <typename T>\
struct name; \
\
template <typename T> \
using $##name = ReadOnly<name<T>>; \
\
template <typename T> \
struct name { \
    using $##T = ReadOnly<T>; \


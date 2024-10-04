
template <typename T> struct MId { using type = T; };
template <typename T> 
using Id = MId<T>::type;

template <typename T> struct MWithoutConst: MId<T> {};
template <typename T> struct MWithoutConst<const T>: MId<T> {};
template <typename T>
using WithoutConst = MWithoutConst<T>::type;

template <typename T> struct MWithoutRef: MId<T> {};
template <typename T> struct MWithoutRef<T&>: MId<T> {};
template <typename T> struct MWithoutRef<T&&>: MId<T> {};
template <typename T> 
using WithoutRef = MWithoutRef<T>::type;

//template <typename T> struct MSolidType

template <typename T>
using Strip = WithoutConst<WithoutRef<T>>;

template <bool Cond, typename A, typename B>
struct Switch: MId<B> {};

template <typename A, typename B>
struct Switch<true, A, B>: MId<A> {};


// MARK: - Boolean utilities

template <bool... Conditions>
constexpr bool bigOr = false;

template <bool... Conditions>
constexpr bool bigOr<false, Conditions...> = bigOr<Conditions...>;

template <bool... Conditions>
constexpr bool bigOr<true, Conditions...> = true;


template <bool... Conditions>
constexpr bool bigAnd = true;

template <bool... Conditions>
constexpr bool bigAnd<true, Conditions...> = bigAnd<Conditions...>;

template <bool... Conditions>
constexpr bool bigAnd<false, Conditions...> = false;


// TODO: maybe someday
//BasicTransformation(WithoutRef, (T, T), (T&, T), (T&&, T))

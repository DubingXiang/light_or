/*
 * @Author: dubing.xiang
 * @Date: 2022-06-27 09:56:29
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-06-27 09:56
 * @Description:
 */
#ifndef LIGHT_OR_TRAITS_H_
#define LIGHT_OR_TRAITS_H_

#include <type_traits>
#include <memory>

namespace light_or {

// case1: T* can cast to C*
template <template <typename...> class C, typename... Ts>
std::true_type is_base_of_template_impl(const C<Ts...>*);
// case2: T* cannot cast to C*
template <template <typename...> class C>
std::false_type is_base_of_template_impl(...);

template <template <typename...> class C, typename T>
using is_base_of_template = decltype(is_base_of_template_impl<C>(std::declval<T*>()));

namespace {
template <typename T>
struct is_smart_pointer_helper : public std::false_type {};

template <typename T>
struct is_smart_pointer_helper<std::shared_ptr<T>> : public std::true_type {};

template <typename T>
struct is_smart_pointer_helper<std::unique_ptr<T>> : public std::true_type {};

template <typename T>
struct is_smart_pointer_helper<std::weak_ptr<T>> : public std::true_type {};

// template <typename T>
// struct is_smart_pointer_helper<std::auto_ptr<T> > : public std::true_type {};

}  // namespace

template <typename T>
struct is_smart_pointer : public is_smart_pointer_helper<typename std::remove_cv<T>::type> {};

template <typename T>
struct argument_type {};
template <typename T, typename U>
struct argument_type<T(U)> {
  using type = U;
};

#define CALLABLE_WITH_STATIC_MEMFUN(FuncName)                                                     \
  template <class T, class... Args>                                                               \
  auto callable_with_static_##FuncName(int)->decltype(void(T::FuncName(std::declval<Args>()...)), \
                                                      std::true_type{});                          \
  template <class, class...>                                                                      \
  auto callable_with_static_##FuncName(...)->std::false_type;                                     \
  template <class T, class... Args>                                                               \
  struct callable_with_static_##FuncName {                                                        \
    enum {                                                                                        \
      value = std::is_same<decltype(callable_with_static##FuncName<T, Args...>(0)),               \
                           std::true_type>::value                                                 \
    };                                                                                            \
  };

template <typename T>
struct extend_decay {
  using decayed_type                = std::decay_t<T>;
  using remove_pointer_decayed_type = std::remove_pointer_t<decayed_type>;
  using type                        = std::remove_cv_t<remove_pointer_decayed_type>;
};

template <typename T>
struct extend_decay<std::shared_ptr<T>> {
  using decayed_type                = std::decay_t<T>;
  using remove_pointer_decayed_type = std::remove_pointer_t<decayed_type>;
  using type                        = std::remove_cv_t<remove_pointer_decayed_type>;
};
template <typename T>
struct extend_decay<std::unique_ptr<T>> {
  using decayed_type                = std::decay_t<T>;
  using remove_pointer_decayed_type = std::remove_pointer_t<decayed_type>;
  using type                        = std::remove_cv_t<remove_pointer_decayed_type>;
};

template <typename Tuple, typename Functor, std::size_t... Is>
constexpr void for_each_tuple_impl(const Tuple& t, Functor&& f, std::index_sequence<Is...>) {
  // 展开成（f(std::get<0>(t))，f(std::get<1>(t))，...）
  (f(std::get<Is>(t), Is), ...);
}

template <typename Tuple, typename Functor>
constexpr void for_each_tuple(const Tuple& t, Functor&& f) {
  // std::make_index_sequence<std::tuple_size_v<Tuple>>产生一个index_sequence<0,1,2,..,N>
  // for_each_tuple_impl(
  //     t, std::forward<Functor>(f), std::make_index_sequence<std::tuple_size_v<Tuple>>());
  for_each_tuple_impl(
      t,
      std::forward<Functor>(f),
      std::make_index_sequence<std::tuple_size_v<typename extend_decay<Tuple>::type>>());
}

}  // namespace light_or

#endif

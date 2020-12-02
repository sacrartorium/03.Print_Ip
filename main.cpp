/*!
\file
\brief
Программа, реализующая печать IP адреса, при помощи SFINAE, из контейнеров и
простых типов.
*/

#include <iostream>
#include <list>
#include <string>
#include <tuple>
#include <type_traits>
#include <vector>

namespace lesson {

/*!
\brief
Метафункция, проверяющая контейнер ли T.
Всегда выдаёт false.
*/
template <typename T> struct is_container { static const bool value = false; };

/*!
\brief
Метафункция, проверяющая контейнер ли T.
Если std::list, выдаёт true;
*/
template <typename T> struct is_container<std::list<T>> {
  static const bool value = true;
};

/*!
\brief
Метафункция, проверяющая контейнер ли T.
Если std::vector, выдаёт true;
*/
template <typename T> struct is_container<std::vector<T>> {
  static const bool value = true;
};

/*!
\brief
Метафункция, разделяющая типы на std::string и все остальные.
Всегда выдаёт false.
*/
template <typename T> struct is_string { static const bool value = false; };

/*!
\brief
Метафункция, разделяющая типы на std::string и все остальные.
Если std::string, выдаёт true;
*/
template <> struct is_string<std::string> { static const bool value = true; };

/*!
\brief
Синоним для значения is_container.
*/
template <typename T>
constexpr bool is_container_value = is_container<T>::value;

/*!
\brief
Синоним для значения is_string.
*/
template <typename Type>
constexpr bool is_string_value = is_string<Type>::value;

/*!
\brief
Синоним для значения is_integral.
*/
template <typename Type>
constexpr bool is_integral_value = std::is_integral<Type>::value;

/*!
Выводит IP адрес, запакованный в контейнеры std::list и std::vector.
\param[in] ip - IP адрес.
*/
template <typename Type,
          typename std::enable_if_t<is_container_value<Type>, bool> Fake = true>
void out_ip(const Type &ip) {
  std::size_t index = 0;

  for (const auto &elem : ip) {
    std::cout << elem;
    if (++index == ip.size())
      std::cout << std::endl;
    else
      std::cout << ".";
  }
}

/*!
Выводит IP адрес, запакованный в целочесленный тип.
\param[in] ip - IP адрес.
*/
template <typename Type,
          typename std::enable_if_t<is_integral_value<Type>, bool> Fake = true>
void out_ip(const Type &ip) {
  uint8_t const *buffer = reinterpret_cast<uint8_t const *>(&ip);

  for (size_t i = 1; i <= sizeof(Type); i++) {
    std::cout << static_cast<unsigned int>(buffer[sizeof(Type) - i]);
    if (i == sizeof(Type))
      std::cout << std::endl;
    else
      std::cout << ".";
  }
}

/*!
Выводит IP адрес, запакованный в std::string.
\param[in] ip - IP адрес.
*/
template <typename Type,
          typename std::enable_if_t<is_string_value<Type>, bool> Fake = true>
void out_ip(const Type &ip) {
  std::cout << ip << std::endl;
}
} // namespace lesson

/*!
\brief
Точка старта программы.
*/
int main() {
  lesson::out_ip(char(-1));
  lesson::out_ip(short(0));
  lesson::out_ip(int(2130706433));
  lesson::out_ip(long(8875824491850138409));
  lesson::out_ip(std::string("trololo"));
  lesson::out_ip(std::vector<int>({0, 1, 2, 3}));
  lesson::out_ip(std::list<int>({0, 1, 2, 3}));

  return 0;
}

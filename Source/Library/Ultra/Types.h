﻿#pragma once

#include <array>
#include <chrono>
#include <cstdint>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace Ultra {

///
/// @brief The following containers, literals and types are used often, therefore they are exposed under the root namespace.
/// 

// Containers
using std::array;
using std::string;
using std::string_view;
using std::vector;

// Literals
using namespace std::literals::chrono_literals;
using namespace std::literals::string_literals;
using namespace std::literals::string_view_literals;

// Smart-Pointer
using std::shared_ptr;
using std::unique_ptr;
using std::weak_ptr;

// Types
using std::int8_t;
using std::int16_t;
using std::int32_t;
using std::int64_t;
using std::intptr_t;
using std::size_t;
using std::uint8_t;
using std::uint16_t;
using std::uint32_t;
using std::uint64_t;
using std::uintptr_t;

///
/// @brief The following definitions serve as replacement possibility.
/// 

// Reference Pointer
template<typename T>
using Reference = std::shared_ptr<T>;

template<typename T, typename ... Args>
constexpr Reference<T> CreateReference(Args && ... args) {
    return std::make_shared<T>(std::forward<Args>(args)...);
}

// Scope Pointer
template<typename T>
using Scope = std::unique_ptr<T>;

template<typename T, typename ... Args>
constexpr Scope<T> CreateScope(Args && ... args) {
    return std::make_unique<T>(std::forward<Args>(args)...);
}

}

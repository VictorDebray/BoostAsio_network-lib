//
// Created by Victor Debray on 2018-12-27.
//

#ifndef PROJECT_TYPES_HPP
#define PROJECT_TYPES_HPP

#include <memory>
#include <vector>
#include <functional>

/**
 * Size of header preceding packet message in RawBuffer
 */
const size_t HEADER_SIZE = 4;

template <class T>
using Sptr = std::shared_ptr<T>;

/**
 * RawBuffer typedef
 */
typedef std::vector<uint8_t> RawBuffer;

using t_readHandler = std::function<void(size_t)>;
using t_writeHandler = std::function<void(size_t)>;
using t_errorHandler = std::function<void()>;

#endif //PROJECT_TYPES_HPP

/*
 * Copyright (c) 2014 BlackBerry Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef UTIL_HPP_
#define UTIL_HPP_

#define FLOAT_MAX_MANTISSA  0x01000000 /* 2^24 */
#define SFLOAT_MAX_MANTISSA  0x1000 /* 2^24 */

#include "LocalTypes.hpp"

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <fcntl.h>
#include <math.h>
#include <string>
#include <btapi/btdevice.h>
#include <bbndk.h>

class Util
{
public:
    static void flushFileDescriptor(int fd);
    static const char *btEventName(const int id);
    static float convertToFloat(uint32_t value);
};

#endif /* UTIL_HPP_ */

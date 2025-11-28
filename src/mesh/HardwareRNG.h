#pragma once

#include <cstddef>
#include <cstdint>

namespace HardwareRNG
{

/**
 * Fill the provided buffer with random bytes sourced from the most
 * appropriate hardware-backed RNG available on the current platform.
 *
 * @param buffer Destination buffer for random bytes
 * @param length Number of bytes to write
 * @return true if the buffer was fully populated with entropy, false on failure
 */
bool fill(uint8_t *buffer, size_t length);

} // namespace HardwareRNG

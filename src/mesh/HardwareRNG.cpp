#include "HardwareRNG.h"

#include <algorithm>
#include <cstring>
#include <random>

#include "configuration.h"

#if defined(ARCH_NRF52)
#include <Adafruit_nRFCrypto.h>
extern Adafruit_nRFCrypto nRFCrypto;
#elif defined(ARCH_ESP32)
#include <esp_system.h>
#elif defined(ARCH_RP2040)
#include <Arduino.h>
#elif defined(ARCH_PORTDUINO)
#include <random>
#include <sys/random.h>
#include <unistd.h>
#endif

namespace HardwareRNG
{

bool fill(uint8_t *buffer, size_t length)
{
    if (!buffer || length == 0) {
        return false;
    }

#if defined(ARCH_NRF52)
    nRFCrypto.begin();
    auto result = nRFCrypto.Random.generate(buffer, length);
    nRFCrypto.end();
    return result;
#elif defined(ARCH_ESP32)
    esp_fill_random(buffer, length);
    return true;
#elif defined(ARCH_RP2040)
    size_t offset = 0;
    while (offset < length) {
        uint32_t value = rp2040.hwrand32();
        size_t toCopy = std::min(length - offset, sizeof(value));
        memcpy(buffer + offset, &value, toCopy);
        offset += toCopy;
    }
    return true;
#elif defined(ARCH_PORTDUINO)
    ssize_t generated = ::getrandom(buffer, length, 0);
    if (generated == static_cast<ssize_t>(length)) {
        return true;
    }

    std::random_device rd;
    size_t offset = 0;
    while (offset < length) {
        uint32_t value = rd();
        size_t toCopy = std::min(length - offset, sizeof(value));
        memcpy(buffer + offset, &value, toCopy);
        offset += toCopy;
    }
    return true;
#else
    std::random_device rd;
    size_t offset = 0;
    while (offset < length) {
        uint32_t value = rd();
        size_t toCopy = std::min(length - offset, sizeof(value));
        memcpy(buffer + offset, &value, toCopy);
        offset += toCopy;
    }
    return true;
#endif
}

bool seed(uint32_t &seedOut)
{
    uint32_t candidate = 0;
    if (!fill(reinterpret_cast<uint8_t *>(&candidate), sizeof(candidate))) {
        return false;
    }
    seedOut = candidate;
    return true;
}

} // namespace HardwareRNG

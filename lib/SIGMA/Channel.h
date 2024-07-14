
#ifndef CHANNEL_H
#define CHANNEL_H

// #include <type_traits>

// Enum for access levels
enum AccessLevel
{
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    GOD_MODE
};

// Template metaprogramming to check if an access level is in the list
template <AccessLevel... Levels> struct AccessList
{
    static constexpr bool contains(AccessLevel level)
    {
        return ((level == Levels) || ...);
    }
};

// Macros for access control
#define CHANNEL_WRITE_ACCESS(var, ...) constexpr auto var##_access = AccessList<__VA_ARGS__>{};

#define CHANNEL_READ(var) (var)

#define CHANNEL_WRITE(var, val, access)                                                                                \
    do                                                                                                                 \
    {                                                                                                                  \
        if (access == GOD_MODE || var##_access.contains_(access))                                                      \
        {                                                                                                              \
            var = val;                                                                                                 \
        }                                                                                                              \
        else                                                                                                           \
        {                                                                                                              \
            static_assert((var##_access.contains_access) || access == GOD_MODE,                                        \
                          "Write access denied for the given access level");                                           \
        }                                                                                                              \
    } while (0)

// Variable initialization
volatile int something = 0;    // Example variable
volatile float something1 = 0; // Example variable

CHANNEL_WRITE_ACCESS(something, AccessLevel::LEVEL_1, LEVEL_2, LEVEL_3);
CHANNEL_WRITE_ACCESS(something1, LEVEL_3);

#endif // CHANNEL_H

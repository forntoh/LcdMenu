---
description: Here are some useful functions
---

# Useful functions

## `substring()`

```cpp
void substring(char* str, uint8_t start, uint8_t size, char* substr);
```

This function extracts a substring from a given string and stores it in the provided destination buffer.

#### Parameters

* `str`: The input string from which the substring will be extracted.
* `start`: The starting index of the substring within the input string.
* `size`: The size of the substring to extract.
* `substr`: The buffer to store the extracted substring. The buffer should have enough capacity to hold the substring plus a null terminator.

#### Description

The `substring()` function copies `size` characters from the `str` starting from the `start` index and places them in the `substr` buffer. The resulting substring is null-terminated.

## `concat()`

### Signature 1

```cpp
void concat(const char* first, char second, const char* third, char* result);
```

This function concatenates three strings and stores the result in the provided destination buffer.

#### Parameters

* `first`: The first string to concatenate.
* `second`: The character to insert between the first and third strings.
* `third`: The second string to concatenate.
* `result`: The buffer to store the concatenated string. The buffer should have enough capacity to hold the resulting string plus a null terminator.

#### Description

The `concat()` function combines the `first` string, the `second` character, and the `third` string into a single string. The resulting string is stored in the `result` buffer, with the `second` character inserted between the first and third strings.

### Signature 2

```cpp
void concat(const char* first, char second, char* result);
```

This overloaded version of the `concat()` function concatenates two strings and stores the result in the provided destination buffer.

#### Parameters

* `first`: The first string to concatenate.
* `second`: The character to insert between the first and second strings.
* `result`: The buffer to store the concatenated string. The buffer should have enough capacity to hold the resulting string plus a null terminator.

#### Description

The `concat()` function combines the `first` string and the `second` character into a single string. The resulting string is stored in the `result` buffer, with the `second` character inserted between the two strings.

### Signature 3

```cpp
void concat(const char* first, const char* second, char* result);
```

This overloaded version of the `concat()` function concatenates two strings and stores the result in the provided destination buffer.

#### Parameters

* `first`: The first string to concatenate.
* `second`: The second string to concatenate.
* `result`: The buffer to store the concatenated string. The buffer should have enough capacity to hold the resulting string plus a null terminator.

#### Description

The `concat()` function combines the `first` string and the `second` string into a single string. The resulting string is stored in the `result` buffer.

## `remove()`

```cpp
void remove(char* str, uint8_t index, uint8_t count);
```

This function removes a specified number of characters from a string, starting from a given index.

#### Parameters

* `str`: The string from which characters will be removed.
* `index`: The starting index of the characters to remove.
* `count`: The number of characters to remove.

#### Description

The `remove()` function removes `count` characters from the `str` starting from the `index`. If the sum of `index` and `count` exceeds the length of the string, the function adjusts

the `count` to remove only the available characters until the end of the string. The resulting string is modified in-place, and the remaining characters are shifted to fill the removed portion.

## mapProgress (long)

The `mapProgress` function maps a `progress` value from the range `MIN_PROGRESS` to `MAX_PROGRESS` to a new range specified by `minValue` and `maxValue`.

#### Function Signature

```cpp
long mapProgress(uint16_t progress, long minValue, long maxValue)
```

#### Parameters

* `progress`: The input progress value to be mapped.
* `minValue`: The minimum value of the new range.
* `maxValue`: The maximum value of the new range.

#### Return Value

* `long`: The mapped value in the new range.

## mapProgress (float)

The `mapProgress` function maps a `progress` value from the range `MIN_PROGRESS` to `MAX_PROGRESS` to a new range specified by `minValue` and `maxValue` as floating-point values.

#### Function Signature

```cpp
float mapProgress(uint16_t progress, float minValue, float maxValue)
```

#### Parameters

* `progress`: The input progress value to be mapped.
* `minValue`: The minimum value of the new range (floating-point).
* `maxValue`: The maximum value of the new range (floating-point).

#### Return Value

* `float`: The mapped value in the new range.

#### Additional Information

The `mapProgress` functions provide a convenient way to map a progress value from one range to another. The uint16\_t version uses the `map` function from the Arduino library to perform the mapping, while the float version performs the mapping manually by normalizing the progress value and scaling it to the specified floating-point range.

Please note that the progress value should be within the range **`MIN_PROGRESS`** to **`MAX_PROGRESS`** to ensure accurate mapping.

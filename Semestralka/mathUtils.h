#pragma once

template <typename T>
T step(T a, T b, T t) {
	return a + (b - a) * t;
}
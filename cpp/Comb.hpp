#pragma once
struct Wire16 {
    Wire16() = default;
    Wire16(int value) : value(value) {}
    operator int() const { return value; }
private:
    short value;
};
struct Wire {
    Wire() = default;
    Wire(const Wire16 *ref, int bitpos) : ref(ref), bitpos(bitpos) {}
    Wire(bool value) : value(value) {}
    operator bool() const { return ref ? (*ref >> bitpos) & 1 : value; }
private:
    const Wire16 *ref{};
    int bitpos;
    char value;
};
struct Not16 {
    const Wire16 *in;
    Wire16 *out;
    void operator()() {
        *out = ~*in & 0xffff;
    }
};
struct And16 {
    const Wire16 *a, *b;
    Wire16 *out;
    void operator()() {
        *out = *a & *b;
    }
};
struct Mux16 {
    const Wire16 *a, *b;
    const Wire *sel;
    Wire16 *out;
    void operator()() {
        *out = (*sel) ? static_cast<int>(*b) : static_cast<int>(*a);
    }
};
struct Add16 {
    const Wire16 *a, *b;
    Wire16 *out;
    void operator()() {
        *out = static_cast<short>((*a + *b) & 0xffff);
    }
};

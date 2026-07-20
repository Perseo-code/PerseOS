#pragma once

typedef enum {
    None,
    MissingQuote
} ErrType;

struct Err {
    bool warning;
    ErrType e = None;
};

bool e_exists(Err&);
void print_err(Err&);
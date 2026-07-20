#include "error.hpp"
#include <drivers/vga/vga.hpp>

bool e_exists(Err& err) {
    return err.e != None; // Returns false if the error does not exist
}

void print_err(Err& err) {
    if (!e_exists(err)) return;
    if (err.warning) print("<WARNING:> ");
    else print("<Err:>");
    switch (err.e) {
        case MissingQuote:
            print("Missing quote");
            break;
        default:
            break;
    }
}
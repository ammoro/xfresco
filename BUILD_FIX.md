# XFresco Build Fix - Multiple Definition Errors

## Problem
The linker was failing with "multiple definition" errors:
```
/usr/bin/ld: callbacks.o: multiple definition of `main_window'
/usr/bin/ld: callbacks.o: multiple definition of `POTENTIALS'
/usr/bin/ld: callbacks.o: multiple definition of `STATES'
/usr/bin/ld: callbacks.o: multiple definition of `BUTTONS'
```

## Root Cause
Two C programming violations:

1. **Global variable defined in header file** (`callbacks.h` lines 25-42)
   - Enum declarations were creating **variables**, not just types
   - Every `.c` file including `callbacks.h` created its own copy
   - Result: Multiple definitions during linking

2. **Same global variable defined in multiple `.c` files**
   - `main_window` was defined in both `main.c:16` and `callbacks.c:57`
   - Should be: defined once in `main.c`, declared as `extern` elsewhere

## Solution

### Fix 1: Convert enum variables to enum types (`src/callbacks.h`)

**Before:**
```c
enum {ZERO, REPLACE, INSERT, ADD, DELETE} BUTTONS;  // Creates variable!
enum {JP, COPYP, ...} STATES;                       // Creates variable!
enum {KP, TYPE, ...} POTENTIALS;                    // Creates variable!
```

**After:**
```c
enum BUTTONS_ENUM {ZERO, REPLACE, INSERT, ADD, DELETE};     // Just a type
enum STATES_ENUM {JP, COPYP, ...};                          // Just a type
enum POTENTIALS_ENUM {KP, TYPE, ...};                       // Just a type
```

### Fix 2: Make `main_window` extern in callbacks.c (`src/callbacks.c:57`)

**Before:**
```c
GtkWidget *main_window;  // Defines variable (conflicts with main.c)
```

**After:**
```c
extern GtkWidget *main_window;  // Declares reference to variable in main.c
```

## Files Modified
1. `src/callbacks.h` - Lines 25-41: Converted enum variables to enum types
2. `src/callbacks.c` - Line 57: Changed definition to extern declaration

## Build Verification
```bash
make clean
make
ls -lh src/xfresco  # Should show ~400KB executable
```

## Notes
- These are standard C linking rules violated in legacy code
- The deprecated warnings (`gtk_widget_show`, etc.) are **cosmetic only** - see `CLAUDE.md`
- The application is fully functional despite deprecation warnings

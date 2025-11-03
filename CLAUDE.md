# CLAUDE.md

This file provides guidance to Claude Code (claude.ai/code) when working with code in this repository.

## Project Overview

**XFresco** is a GTK-4 GUI for FRESCO, a coupled channel computer code for nuclear/atomic scattering calculations. This codebase recently underwent a complete migration from GTK-2 (2008) to GTK-4 (2025), spanning 17 years of toolkit evolution.

**Language:** C (C99)
**UI Framework:** GTK-4.0+
**Build System:** GNU Autotools
**License:** GPLv2+

## Build Commands

### Quick Build
```bash
# Automated build with dependency installation
./build.sh

# Clean and rebuild
./build.sh clean
```

### Manual Build
```bash
# From repository root
./configure
make

# Clean build
make clean && make

# Install system-wide (optional)
sudo make install
```

### Run Application
```bash
cd src && ./xfresco
```

### Development Iteration
```bash
# After modifying source files
make
./src/xfresco
```

## Architecture Overview

### GTK-4 Migration Strategy

This codebase uses a **compatibility layer approach** to minimize changes during the GTK-2 to GTK-4 migration:

1. **CList Compatibility Layer** (`src/clist_compat.c/h`)
   - Wraps GTK-4's `GtkColumnView` to provide GTK-2 `GtkCList` API
   - Custom `CListRow` structure holds row data
   - Functions like `xfr_clist_append()`, `xfr_clist_get_text()`, etc.
   - Allows existing callbacks to work with minimal changes
   - Uses `GListStore` as data model backing

2. **Event Controller Migration**
   - Old GTK-2: Event signals (`button-press-event`, `focus-out-event`)
   - New GTK-4: Event controllers (`GtkEventControllerFocus`, `GtkGestureClick`)
   - Entry validation uses `GtkEventControllerFocus` with `"leave"` signal
   - 18 q-scale entry widgets have focus controllers for validation

3. **Container Updates**
   - `GtkTable` → `GtkGrid` with `gtk_grid_attach()`
   - `GtkHBox`/`GtkVBox` → `GtkBox` with orientation
   - All 452 widget attachments converted using automated script

4. **Widget Replacements**
   - `GtkComboBox`/`GtkOptionMenu` → `GtkComboBoxText` (transitional)
   - Eventually target: `GtkDropDown` (pure GTK-4)
   - Selection callbacks wrapped to preserve old signatures

### Key Architecture Patterns

#### Widget Lookup Pattern
```c
GtkWidget *widget = lookup_widget(parent_widget, "widget_name");
```
All widgets registered via `GLADE_HOOKUP_OBJECT()` in `src/interface.c`

#### CList Usage Pattern
```c
// Append row
gchar *row_data[NUM_COLUMNS] = {"value1", "value2", ...};
xfr_clist_append(GTK_COLUMN_VIEW(clist), row_data);

// Get selection callback
xfr_clist_set_selection_callback(GTK_COLUMN_VIEW(clist),
                                  G_CALLBACK(on_select_row),
                                  user_data);
```

#### Combo Box Pattern (GTK-3 compatible)
```c
// Create combo box
GtkWidget *combo = gtk_combo_box_text_new();
gtk_combo_box_text_append_text(GTK_COMBO_BOX_TEXT(combo), "item");
gtk_combo_box_set_active(GTK_COMBO_BOX(combo), 0);

// Utility functions in src/utils.c
xfr_combo_set_value(parent, "combo_name", index);
int selected = xfr_combo_get_value(parent, "combo_name");
```

## File Structure

### Core Application Files
- **`src/main.c`** - Entry point, GTK initialization, status bar setup
- **`src/interface.c`** (10,000+ lines) - UI generation, all widget creation
- **`src/callbacks.c`** (3,500+ lines) - Event handlers for all UI interactions
- **`src/callbacks.h`** - Callback function declarations

### Data Management
- **`src/namelist.c`** - FRESCO input file parsing (Fortran namelist format)
- **`src/savefile.c`** - Serialize UI state to FRESCO input files
- **`src/openfile.c`** - Load FRESCO files into UI

### Compatibility & Utilities
- **`src/clist_compat.c/h`** - GTK-4 compatibility layer for GtkCList
- **`src/utils.c/h`** - Widget utilities (combo boxes, option menus)
- **`src/support.c/h`** - Widget lookup functions

### Configuration
- **`src/xfresco.h`** - Global constants, column definitions, enums
- **`xfresco2.gld`** - Glade UI definition (reference, not directly used)
- **`configure.ac`** - Autoconf configuration
- **`build.sh`** - Smart build script with auto-dependency installation

## Critical Constants (src/xfresco.h)

```c
#define COLUMNS_PARTITIONS 10   // Partition list columns
#define COLUMNS_STATES 17       // States list columns
#define COLUMNS_POT 12         // Potential list columns
#define COLUMNS_STEP 6         // Step function columns
#define COLUMNS_COUP 10        // Coupling matrix columns
#define COLUMNS_INEL 6         // Inelastic columns
#define COLUMNS_CFP 5          // CFP columns
#define COLUMNS_OVERLAP 25     // Overlap columns
#define QSCALEDIM 8            // Q-scale dimension
```

## Common Development Patterns

### Adding a New Widget to Interface

1. Declare in `src/interface.c` function scope:
   ```c
   GtkWidget *my_widget;
   ```

2. Create and configure widget:
   ```c
   my_widget = gtk_entry_new();
   gtk_widget_set_visible(my_widget, TRUE);
   gtk_grid_attach(GTK_GRID(parent), my_widget, col, row, width, height);
   ```

3. Register for lookup:
   ```c
   GLADE_HOOKUP_OBJECT(main_window, my_widget, "my_widget");
   ```

4. Add callback in `src/callbacks.c`:
   ```c
   void on_my_widget_changed(GtkEditable *editable, gpointer user_data) {
       GtkWidget *widget = GTK_WIDGET(editable);
       const gchar *text = gtk_editable_get_text(editable);
       // Handle change
   }
   ```

5. Connect signal in `src/interface.c`:
   ```c
   g_signal_connect((gpointer) my_widget, "changed",
                    G_CALLBACK(on_my_widget_changed), NULL);
   ```

### Debugging Build Issues

Build logs are generated automatically:
- `autoreconf.log` - Autoconf/automake output
- `configure.log` - Configure script output
- `build.log` - Compilation output

Check GTK-4 installation:
```bash
pkg-config --modversion gtk4
pkg-config --cflags gtk4
```

### Known Constraints

1. **Widget Visibility**: Uses deprecated `gtk_widget_show()` (~1260 instances)
   - Functional but generates deprecation warnings
   - Modern replacement: `gtk_widget_set_visible(widget, TRUE)`
   - Automated replacement challenging due to nested macros

2. **Menu System**: Commented out (GtkMenuBar deprecated)
   - Dialogs provide access to all features
   - Modern approach would use `GMenu` + `GtkPopoverMenuBar`

3. **Null Widgets**: Many GTK-2 widgets intentionally NULL
   - Legacy widgets without GTK-4 equivalents
   - Generates ~346 runtime warnings (non-fatal)
   - Operations on NULL widgets safely skipped

4. **Grid Attachments**: All converted via Python script
   - `convert_table_attach.py` in repository root
   - Converts `gtk_table_attach()` → `gtk_grid_attach()`
   - Do not manually edit; regenerate if needed

## Migration Scripts

**`convert_table_attach.py`** - Converts GTK-2 table attachments to GTK-4 grids
```bash
python3 convert_table_attach.py
```
- Reads from `src/interface.c.bak` (original GTK-2 code)
- Writes to `src/interface.c`
- Handles 452 widget attachments automatically

## Testing After Changes

1. **Compile test**: `make clean && make`
2. **Quick runtime test**: `./src/xfresco` (check for crashes)
3. **Feature test**: Interact with modified widgets
4. **Check warnings**: `./src/xfresco 2>&1 | grep -c CRITICAL`

## Dependencies

Required packages (auto-installed by `build.sh`):
- `gtk4` (>= 4.0)
- `glib-2.0` (>= 2.56)
- `autoconf`
- `automake`
- `pkg-config`
- `gcc` (C99 support)

## Important Implementation Notes

### Event Controllers (GTK-4 Pattern)

When adding new event handling:

```c
// Focus events - for validation on tab-out
GtkEventController *focus = gtk_event_controller_focus_new();
g_signal_connect(focus, "leave", G_CALLBACK(on_focus_out), NULL);
gtk_widget_add_controller(widget, focus);

// Mouse clicks - for interactive labels/buttons
GtkGesture *click = gtk_gesture_click_new();
g_signal_connect(click, "pressed", G_CALLBACK(on_click), NULL);
gtk_widget_add_controller(widget, GTK_EVENT_CONTROLLER(click));

// Key events - for keyboard shortcuts
GtkEventController *key = gtk_event_controller_key_new();
g_signal_connect(key, "key-pressed", G_CALLBACK(on_key_press), NULL);
gtk_widget_add_controller(widget, key);
```

### Selection Model Pattern (Lists)

All list widgets use the compatibility wrapper:

```c
// In src/interface.c
xfr_clist_set_selection_callback(GTK_COLUMN_VIEW(my_clist),
                                  G_CALLBACK(on_my_clist_select_row),
                                  NULL);

// In src/callbacks.c - old signature preserved
void on_my_clist_select_row(GtkWidget *clist, gint row, gint column,
                             gpointer user_data) {
    gchar *text;
    xfr_clist_get_text(GTK_COLUMN_VIEW(clist), row, column, &text);
    // Handle selection
}
```

## Code Style

- **Indentation**: Mixed (legacy code)
- **Naming**: Snake_case for functions, PascalCase avoided
- **Comments**: Sparse; rely on git history and this documentation
- **Error handling**: `g_return_if_fail()` for assertions
- **Memory**: Use GLib functions (`g_malloc`, `g_free`, `g_strdup`)

## Migration Status

**Current State**: Fully functional GTK-4 application
- ✅ Core migration complete (100%)
- ✅ All recommended enhancements complete (3/3)
  - Combo/OptionMenu widgets functional
  - List selection callbacks working (8 lists)
  - Entry validation with focus controllers (18 widgets)
- ⚪ Optional polish deferred (cosmetic only)
  - Widget visibility warnings (~1260)
  - Menu system (application works without it)

See `REMAINING_WORK.md` for detailed migration history and optional enhancements.

## Performance Notes

- **Binary size**: ~293KB (typical)
- **Build time**: 2-5 minutes on modern systems
- **Lines of code**: ~21,000 total C code
- **UI complexity**: 10,000+ lines in interface.c alone

## References

- **GTK-4 Documentation**: https://docs.gtk.org/gtk4/
- **Migration Guide**: https://docs.gtk.org/gtk4/migrating-3to4.html
- **FRESCO**: Coupled channel code by Ian Thompson (University of Surrey)
- **Project docs**: `README.md`, `UPDATE.md`, `MIGRATION_COMPLETE.md`, `REMAINING_WORK.md`

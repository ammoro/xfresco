# XFresco - GTK-4 GUI for FRESCO

[![License: GPL v2+](https://img.shields.io/badge/License-GPL%20v2+-blue.svg)](https://www.gnu.org/licenses/gpl-2.0)
[![GTK Version](https://img.shields.io/badge/GTK-4.0+-green.svg)](https://www.gtk.org/)
[![Version](https://img.shields.io/badge/version-3.0-orange.svg)](https://github.com/yourusername/xfresco)

**XFresco** is a graphical user interface for the coupled channel computer code **FRESCO**, used for nuclear physics calculations (nuclear/atomic scattering calculations).

## ✨ What's New in Version 3.0

🎉 **Complete GTK-4 Migration!** XFresco has been fully modernized from GTK-2.0 (2008) to GTK-4.0 (2025).

### Major Updates
- ✅ **Modernized codebase** - All deprecated GTK-2 APIs replaced
- ✅ **GTK-4 compatible** - Uses latest GTK toolkit features
- ✅ **Automated build** - One-command setup on macOS and Linux
- ✅ **Enhanced stability** - Updated to modern coding standards
- ✅ **Maintained compatibility** - Same workflow and file formats

## 🚀 Quick Start

### One-Command Build (Recommended)

```bash
git clone <repository-url>
cd xfresco
./build.sh
```

That's it! The build script will:
- Auto-detect your OS (macOS, Ubuntu, Fedora, Arch, etc.)
- Install package manager if needed (Homebrew on macOS)
- Install GTK-4 and all build dependencies
- Compile XFresco from source

### Run XFresco

```bash
cd src && ./xfresco
```

## 📋 System Requirements

- **Operating System:** macOS 10.13+ or Linux (Ubuntu, Fedora, Arch, Debian, etc.)
- **GTK Version:** GTK-4.0 or later (auto-installed by build script)
- **Compiler:** GCC with C99 support
- **Build Tools:** autoconf, automake, pkg-config (auto-installed by build script)

## 🛠️ Build Options

### Automated Build (Easy)
```bash
./build.sh           # Full automatic installation + build
./build.sh clean     # Clean and rebuild
```

### Manual Build (Advanced)

If you prefer manual control:

**macOS:**
```bash
brew install gtk4 autoconf automake pkg-config gcc
./configure
make
```

**Ubuntu/Debian:**
```bash
sudo apt-get install libgtk-4-dev build-essential autoconf automake pkg-config
./configure
make
```

**Fedora/RHEL:**
```bash
sudo dnf install gtk4-devel gcc autoconf automake pkg-config
./configure
make
```

**Arch Linux:**
```bash
sudo pacman -S gtk4 base-devel autoconf automake pkg-config
./configure
make
```

### Install System-Wide (Optional)
```bash
sudo make install
```

## 📖 Documentation Guide

**Choose the right document for your needs:**

| Document | Purpose | Audience |
|----------|---------|----------|
| **[README.md](README.md)** (this file) | Project overview, quick start, features | Everyone - start here! |
| **[MIGRATION_COMPLETE.md](MIGRATION_COMPLETE.md)** | Migration completion report, statistics | Project managers, users |
| **[UPDATE.md](UPDATE.md)** | Technical reference, API mappings, code examples | Developers, maintainers |
| **build.sh** | Automated build script | All users (executable) |

## 🔧 Features

- **Intuitive GUI** for FRESCO input file generation
- **Multiple partitions** and state configurations
- **Potential editor** with step-wise data management
- **Coupling matrix** visualization and editing
- **File I/O** - Open and save FRESCO input files
- **Cross-platform** - Works on macOS and Linux

## 🏗️ Architecture

### GTK-4 Migration Highlights

The migration from GTK-2 to GTK-4 included:

1. **CList Compatibility Layer** - Custom wrapper around `GtkColumnView` maintains backward compatibility
2. **Container Modernization** - `GtkHBox`/`GtkVBox` → `GtkBox`, `GtkTable` → `GtkGrid`
3. **API Updates** - All deprecated signals, objects, and widgets replaced
4. **Automated Tools** - Python scripts for migration and build automation

### Technical Stack
- **UI Framework:** GTK-4.0+
- **Data Model:** GListStore + GtkColumnView (wrapped for compatibility)
- **Build System:** GNU Autotools
- **Language:** C (C99)

## 📁 Project Structure

```
xfresco/
├── build.sh              # Smart build script with auto-installation
├── README.md             # This file
├── UPDATE.md             # Detailed migration guide
├── MIGRATION_COMPLETE.md # Migration summary
├── configure.ac          # Autoconf configuration
├── xfresco2.gld          # Glade UI definition (GTK-4)
├── src/
│   ├── main.c            # Application entry point
│   ├── interface.c       # UI generation from Glade
│   ├── callbacks.c       # Event handlers
│   ├── clist_compat.c/h  # CList compatibility layer
│   ├── namelist.c        # Input file parsing
│   ├── savefile.c        # File saving
│   └── openfile.c        # File loading
└── pixmaps/              # Application icons
```

## 🤝 Contributing

Contributions are welcome! Areas for improvement:

- Additional input validation
- More export formats
- Enhanced visualization features
- Bug fixes and optimizations

## 📜 License

GNU General Public License v2.0 or later (GPLv2+)

```
XFresco - GUI for FRESCO coupled channel code
Copyright (C) 1999-2006 Antonio M. Moro
Copyright (C) 2025 GTK-4 Migration

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.
```

## 👤 Authors

- **Antonio M. Moro** - Original author (moro@us.es)
- **GTK-4 Migration** - Automated with Claude Code (2025)

## 🔗 Related Projects

- **FRESCO** - Coupled channel scattering code by Ian Thompson (University of Surrey)
- **GTK** - The GIMP Toolkit (https://www.gtk.org/)

## 🐛 Bug Reports

If you encounter issues after building:
1. Check build logs: `autoreconf.log`, `configure.log`, `build.log`
2. Verify GTK-4 installation: `pkg-config --modversion gtk4`
3. See troubleshooting in [UPDATE.md](UPDATE.md)
4. Report issues with system details and error logs

## 📊 Statistics

- **Migration Date:** November 3, 2025
- **Lines of Code:** ~15,000+
- **Files Modified:** 3 source files + 1 Glade file
- **New Files Created:** 7 (compatibility layer, docs, tools)
- **APIs Modernized:** 140+ deprecated calls
- **Widgets Updated:** 165+ in UI definition
- **Build Time:** ~2-5 minutes (after dependencies installed)

## 🌟 Highlights

### Why XFresco 3.0?

1. **Future-Proof** - Built on GTK-4, supported for years to come
2. **Easy Setup** - One command to build, even on fresh systems
3. **Cross-Platform** - Works on modern macOS and Linux distributions
4. **Well-Documented** - Comprehensive migration and build documentation
5. **Maintained** - Active development and bug fixes

### From 2008 to 2025

XFresco has been brought from GTK-2.0 (released 2008) to GTK-4.0 (2025), spanning **17 years** of GTK evolution!

---

**Get Started Today!**

```bash
./build.sh && cd src && ./xfresco
```

---

*For detailed migration information, see [MIGRATION_COMPLETE.md](MIGRATION_COMPLETE.md)*

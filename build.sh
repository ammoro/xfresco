#!/bin/bash

#############################################################################
# XFresco Smart Build Script with Auto-Installation
# Automates GTK-4 installation and build process
#############################################################################

set -e  # Exit on error
set -o pipefail  # Exit on pipeline errors

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
MAGENTA='\033[0;35m'
CYAN='\033[0;36m'
NC='\033[0m' # No Color

# Logging functions
info() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

important() {
    echo -e "${MAGENTA}[IMPORTANT]${NC} $1"
}

# Ask user for confirmation
ask_user() {
    local prompt="$1"
    local response
    echo -e "${CYAN}[QUESTION]${NC} $prompt (y/n): "
    read -r response
    case "$response" in
        [yY][eE][sS]|[yY])
            return 0
            ;;
        *)
            return 1
            ;;
    esac
}

# Detect OS
detect_os() {
    if [[ "$OSTYPE" == "darwin"* ]]; then
        OS="macos"
        info "Detected macOS"
    elif [[ -f /etc/os-release ]]; then
        . /etc/os-release
        OS="linux"
        DISTRO=$ID
        info "Detected Linux distribution: $DISTRO"
    else
        OS="unknown"
        error "Unable to detect operating system"
        exit 1
    fi
}

# Install Homebrew on macOS
install_homebrew() {
    info "Installing Homebrew (this may take several minutes)..."
    important "You may be prompted for your password"

    /bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"

    # Add Homebrew to PATH for Apple Silicon Macs
    if [[ -d "/opt/homebrew/bin" ]]; then
        export PATH="/opt/homebrew/bin:$PATH"
        success "Homebrew installed at /opt/homebrew"
    elif [[ -d "/usr/local/bin" ]]; then
        export PATH="/usr/local/bin:$PATH"
        success "Homebrew installed at /usr/local"
    fi

    # Verify installation
    if command -v brew >/dev/null 2>&1; then
        success "Homebrew successfully installed!"
    else
        error "Homebrew installation failed"
        exit 1
    fi
}

# Check and install package manager on macOS
setup_macos_package_manager() {
    info "Checking for package manager on macOS..."

    # Check for Homebrew
    if command -v brew >/dev/null 2>&1; then
        success "Homebrew is already installed"
        PKG_MANAGER="brew"
        return 0
    fi

    # Check for MacPorts
    if command -v port >/dev/null 2>&1; then
        success "MacPorts is already installed"
        PKG_MANAGER="port"
        return 0
    fi

    # Neither found - install Homebrew
    warning "No package manager (Homebrew or MacPorts) found"
    echo
    important "Homebrew will be installed to manage dependencies"
    echo "Homebrew is the most popular package manager for macOS"
    echo "Visit https://brew.sh for more information"
    echo

    if ask_user "Install Homebrew now?"; then
        install_homebrew
        PKG_MANAGER="brew"
    else
        error "Cannot proceed without a package manager"
        echo "Please install Homebrew or MacPorts manually, then run this script again"
        exit 1
    fi
}

# Install GTK-4 on macOS
install_gtk4_macos() {
    info "Installing GTK-4 and dependencies on macOS..."

    if [[ "$PKG_MANAGER" == "brew" ]]; then
        important "Installing via Homebrew (this may take 10-20 minutes)..."
        brew install gtk4 autoconf automake pkg-config gcc
        success "GTK-4 and build tools installed via Homebrew"
    elif [[ "$PKG_MANAGER" == "port" ]]; then
        important "Installing via MacPorts (this may take 10-20 minutes)..."
        important "You will be prompted for your password (sudo required)"
        sudo port install gtk4 autoconf automake pkgconfig gcc
        success "GTK-4 and build tools installed via MacPorts"
    fi
}

# Install GTK-4 on Linux
install_gtk4_linux() {
    info "Installing GTK-4 and dependencies on Linux..."
    important "You will be prompted for your password (sudo required)"

    case "$DISTRO" in
        ubuntu|debian|linuxmint|pop)
            info "Using apt package manager..."
            sudo apt-get update
            sudo apt-get install -y libgtk-4-dev build-essential autoconf automake pkg-config gcc
            success "GTK-4 and build tools installed via apt"
            ;;
        fedora|rhel|centos)
            info "Using dnf/yum package manager..."
            if command -v dnf >/dev/null 2>&1; then
                sudo dnf install -y gtk4-devel gcc gcc-c++ make autoconf automake pkgconfig
            else
                sudo yum install -y gtk4-devel gcc gcc-c++ make autoconf automake pkgconfig
            fi
            success "GTK-4 and build tools installed via dnf/yum"
            ;;
        arch|manjaro)
            info "Using pacman package manager..."
            sudo pacman -Syu --noconfirm gtk4 base-devel autoconf automake pkg-config gcc
            success "GTK-4 and build tools installed via pacman"
            ;;
        opensuse*)
            info "Using zypper package manager..."
            sudo zypper install -y gtk4-devel gcc gcc-c++ make autoconf automake pkg-config
            success "GTK-4 and build tools installed via zypper"
            ;;
        *)
            error "Unsupported Linux distribution: $DISTRO"
            echo
            echo "Please install GTK-4 manually for your distribution:"
            echo "  - Look for packages: gtk4, gtk4-devel, or libgtk-4-dev"
            echo "  - Also install: gcc, make, autoconf, automake, pkg-config"
            exit 1
            ;;
    esac
}

# Check and install GTK-4
check_and_install_gtk4() {
    info "Checking for GTK-4 libraries..."

    if pkg-config --exists gtk4 2>/dev/null; then
        GTK_VERSION=$(pkg-config --modversion gtk4)
        success "GTK-4 found: version $GTK_VERSION"
        return 0
    fi

    # GTK-4 not found
    warning "GTK-4 not found on this system"
    echo
    important "GTK-4 is required to build XFresco"
    echo "This script can automatically install it for you"
    echo

    if ask_user "Install GTK-4 and dependencies now?"; then
        if [[ "$OS" == "macos" ]]; then
            install_gtk4_macos
        elif [[ "$OS" == "linux" ]]; then
            install_gtk4_linux
        fi

        # Verify installation
        if pkg-config --exists gtk4 2>/dev/null; then
            GTK_VERSION=$(pkg-config --modversion gtk4)
            success "GTK-4 successfully installed: version $GTK_VERSION"
        else
            error "GTK-4 installation failed"
            echo "Please install GTK-4 manually and run this script again"
            exit 1
        fi
    else
        error "Cannot build without GTK-4"
        exit 1
    fi
}

# Check and install build tools
check_build_tools() {
    info "Checking build dependencies..."

    local missing_tools=()

    # Check each tool
    command -v autoconf >/dev/null 2>&1 || missing_tools+=("autoconf")
    command -v automake >/dev/null 2>&1 || missing_tools+=("automake")
    command -v pkg-config >/dev/null 2>&1 || missing_tools+=("pkg-config")
    command -v gcc >/dev/null 2>&1 || missing_tools+=("gcc")

    if [ ${#missing_tools[@]} -eq 0 ]; then
        success "All build tools found"
        return 0
    fi

    warning "Missing build tools: ${missing_tools[*]}"
    echo

    if ask_user "Install missing build tools?"; then
        if [[ "$OS" == "macos" ]]; then
            if [[ "$PKG_MANAGER" == "brew" ]]; then
                brew install autoconf automake pkg-config gcc
            elif [[ "$PKG_MANAGER" == "port" ]]; then
                sudo port install autoconf automake pkgconfig gcc
            fi
        elif [[ "$OS" == "linux" ]]; then
            case "$DISTRO" in
                ubuntu|debian|linuxmint|pop)
                    sudo apt-get install -y build-essential autoconf automake pkg-config gcc
                    ;;
                fedora|rhel|centos)
                    if command -v dnf >/dev/null 2>&1; then
                        sudo dnf install -y gcc gcc-c++ make autoconf automake pkgconfig
                    else
                        sudo yum install -y gcc gcc-c++ make autoconf automake pkgconfig
                    fi
                    ;;
                arch|manjaro)
                    sudo pacman -Syu --noconfirm base-devel autoconf automake pkg-config
                    ;;
                *)
                    error "Unable to auto-install build tools for $DISTRO"
                    exit 1
                    ;;
            esac
        fi
        success "Build tools installed"
    else
        error "Cannot build without required tools"
        exit 1
    fi
}

# Print header
echo "======================================================================="
echo "  XFresco GTK-4 Smart Build Script with Auto-Installation"
echo "  Version: 3.0 (GTK-4 Migration)"
echo "======================================================================="
echo

# Detect operating system
detect_os

# Setup package manager (macOS only)
if [[ "$OS" == "macos" ]]; then
    setup_macos_package_manager
fi

# Check and install GTK-4
check_and_install_gtk4

# Check and install build tools
check_build_tools

# Check for GLib
info "Checking for GLib libraries..."
if pkg-config --exists glib-2.0; then
    GLIB_VERSION=$(pkg-config --modversion glib-2.0)
    success "GLib found: version $GLIB_VERSION"
else
    error "GLib not found (should have been installed with GTK-4)"
    exit 1
fi

echo

# Clean previous build if requested
if [ "$1" == "clean" ] || [ "$1" == "--clean" ]; then
    info "Cleaning previous build..."
    make clean 2>/dev/null || true
    rm -rf autom4te.cache
    rm -f config.log config.status
    rm -f Makefile src/Makefile pixmaps/Makefile
    rm -f config.h config.h.in
    rm -f aclocal.m4
    success "Clean complete"
    echo
fi

# Step 1: Generate configure script (if needed)
if [ ! -f "configure" ] || [ "$1" == "clean" ] || [ "$1" == "--clean" ]; then
    info "Step 1/4: Generating build system (autoreconf)..."
    if autoreconf --install --force 2>&1 | tee autoreconf.log; then
        success "autoreconf completed"
    else
        error "autoreconf failed. See autoreconf.log for details."
        exit 1
    fi
else
    info "Step 1/4: Using existing configure script"
fi
echo

# Step 2: Configure
info "Step 2/4: Configuring build..."
if [ -f "configure" ]; then
    chmod +x configure
    if ./configure 2>&1 | tee configure.log; then
        success "Configure completed"
    else
        error "Configure failed. See configure.log for details."
        echo
        echo "Common issues:"
        echo "  - Missing GTK-4 or GLib development packages"
        echo "  - pkg-config cannot find gtk4.pc"
        exit 1
    fi
else
    error "configure script not found. autoreconf may have failed."
    exit 1
fi
echo

# Step 3: Build
info "Step 3/4: Compiling source code..."
if make 2>&1 | tee build.log; then
    success "Build completed successfully!"
else
    error "Build failed. See build.log for details."
    echo
    echo "Analyzing errors..."
    if grep -q "GtkCList" build.log 2>/dev/null; then
        warning "Detected GtkCList errors - this widget is not available in GTK-4"
        echo "  ACTION REQUIRED: Complete GtkCList → GtkColumnView migration"
        echo "  See UPDATE.md for details"
    fi
    if grep -q "undeclared" build.log 2>/dev/null; then
        warning "Detected undeclared identifier errors"
    fi
    if grep -q "deprecated" build.log 2>/dev/null; then
        warning "Detected deprecated API usage warnings"
    fi
    exit 1
fi
echo

# Step 4: Check results
info "Step 4/4: Checking build results..."

if [ -f "src/xfresco" ]; then
    success "Executable created: src/xfresco"
    echo
    echo "======================================================================="
    echo "  BUILD SUCCESSFUL!"
    echo "======================================================================="
    echo
    echo "To run XFresco:"
    echo "  cd src && ./xfresco"
    echo
    echo "To install system-wide (optional):"
    echo "  sudo make install"
    echo
else
    warning "Executable not found. Build may have partially failed."
    exit 1
fi

# Print summary
echo "Build Summary:"
echo "  Operating System: $OS"
if [[ "$OS" == "macos" ]]; then
    echo "  Package Manager:  $PKG_MANAGER"
fi
echo "  GTK Version:      $GTK_VERSION"
echo "  GLib Version:     $GLIB_VERSION"
echo "  Compiler:         $(gcc --version 2>/dev/null | head -n1 || echo 'Unknown')"
echo
echo "Logs saved:"
echo "  - autoreconf.log"
echo "  - configure.log"
echo "  - build.log"
echo

success "All done!"

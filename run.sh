#!/bin/bash
# Ardour Plus Build Runner - Main DAW Selector
export TOP="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

echo "=========================================="
echo "      Ardour Plus Build Selector"
echo "=========================================="

# Find environment scripts in built_binaries/
ENV_SCRIPTS=($(ls "$TOP"/built_binaries/ardev_common_waf.sh* 2>/dev/null | sort -V))

if [ ${#ENV_SCRIPTS[@]} -eq 0 ]; then
    echo "No builds found in built_binaries/. Please run ./build.sh first."
    exit 1
fi

declare -a OPTIONS_NAME=()
declare -a OPTIONS_ENV=()
declare -a OPTIONS_EXE=()

for env_script in "${ENV_SCRIPTS[@]}"; do
    filename="$(basename "$env_script")"
    tag="${filename#*#}" # Extracts build tag, e.g., build1, build2
    if [ "$tag" = "$filename" ]; then
        tag="build1"
    fi

    # 1. GTK4 Ardour Plus Preferences (adw_prefs)
    gtk4_prefs="$TOP/built_binaries/ardour4-prefs#$tag"
    if [ ! -f "$gtk4_prefs" ]; then
        gtk4_prefs="$TOP/gtk4_ardour/builddir/adw_prefs/ardour4-prefs"
    fi
    if [ -x "$gtk4_prefs" ]; then
        OPTIONS_NAME+=("GTK4 Ardour Plus — Preferences ($tag)")
        OPTIONS_ENV+=("$env_script")
        OPTIONS_EXE+=("$gtk4_prefs")
    fi

    # 2. GTK4 Ardour Plus Dialogs (adw_dialogs)
    gtk4_dialogs="$TOP/built_binaries/ardour4-dialogs#$tag"
    if [ ! -f "$gtk4_dialogs" ]; then
        gtk4_dialogs="$TOP/gtk4_ardour/builddir/adw_dialogs/ardour4-dialogs"
    fi
    if [ -x "$gtk4_dialogs" ]; then
        OPTIONS_NAME+=("GTK4 Ardour Plus — Session & Engine Setup ($tag)")
        OPTIONS_ENV+=("$env_script")
        OPTIONS_EXE+=("$gtk4_dialogs")
    fi

    # 3. GTK4 Ardour Plus Main DAW Shell (adw_shell)
    gtk4_shell="$TOP/built_binaries/ardour4-shell#$tag"
    if [ ! -f "$gtk4_shell" ]; then
        gtk4_shell="$TOP/gtk4_ardour/builddir/adw_shell/ardour4-shell"
    fi
    if [ -x "$gtk4_shell" ]; then
        OPTIONS_NAME+=("GTK4 Ardour Plus — Main DAW Shell ($tag)")
        OPTIONS_ENV+=("$env_script")
        OPTIONS_EXE+=("$gtk4_shell")
    fi

    # 4. Qt6 QML Ardour Plus — Main DAW Shell
    qt6_shell="$TOP/built_binaries/ardour4-qt#$tag"
    if [ ! -f "$qt6_shell" ]; then
        qt6_shell="$TOP/qt6_ardour/build/ardour4-qt"
    fi
    if [ -x "$qt6_shell" ] || [ -f "$qt6_shell" ]; then
        OPTIONS_NAME+=("Qt6 QML Ardour Plus — Main DAW Shell ($tag)")
        OPTIONS_ENV+=("$env_script")
        OPTIONS_EXE+=("$qt6_shell")
    fi

    # 2. GTK2 Legacy Core DAW
    exe="$TOP/built_binaries/ardour-8.6.0#$tag"
    if [ ! -f "$exe" ]; then
        exe="$TOP/build/gtk2_ardour/ardour-8.6.0"
    fi
    if [ -x "$exe" ] || [ -f "$exe" ]; then
        OPTIONS_NAME+=("GTK2 Legacy Core DAW ($tag)")
        OPTIONS_ENV+=("$env_script")
        OPTIONS_EXE+=("$exe")
    fi
done

if [ ${#OPTIONS_NAME[@]} -eq 0 ]; then
    echo "No Ardour DAW executable found. Please run ./build.sh first."
    exit 1
fi

echo "Available Ardour DAW Builds:"
for i in "${!OPTIONS_NAME[@]}"; do
    echo "  [$((i+1))] ${OPTIONS_NAME[$i]}"
done
echo ""

if [ ${#OPTIONS_NAME[@]} -eq 1 ]; then
    CHOICE=1
    echo "Launching: ${OPTIONS_NAME[0]}"
else
    if [ -n "$1" ]; then
        CHOICE="$1"
    else
        read -p "Select build to launch [1-${#OPTIONS_NAME[@]}]: " CHOICE
    fi
fi

IDX=$((CHOICE-1))

if [ -z "$CHOICE" ] || [ $IDX -lt 0 ] || [ $IDX -ge ${#OPTIONS_NAME[@]} ]; then
    echo "Invalid selection."
    exit 1
fi

SELECTED_ENV="${OPTIONS_ENV[$IDX]}"
SELECTED_EXE="${OPTIONS_EXE[$IDX]}"

if [ -f "$TOP/gtk2_ardour/MatrixDot.ttf" ]; then
    mkdir -p ~/.local/share/fonts
    cp -f "$TOP/gtk2_ardour/MatrixDot.ttf" ~/.local/share/fonts/
    fc-cache -f ~/.local/share/fonts >/dev/null 2>&1
fi

if [ -f ~/.config/ardour8/ui_config ]; then
    sed -i 's/value="ArdourMono/value="Matrix Dot/g' ~/.config/ardour8/ui_config
fi

echo "Starting ${OPTIONS_NAME[$IDX]}..."
. "$SELECTED_ENV"

if [[ "$SELECTED_EXE" == *"ardour4-"* ]]; then
    # Unset GTK2 environment variables that contaminate GTK4 processes
    unset GTK_PATH
    unset SUIL_MODULE_DIR
    unset GTK2_RC_FILES
    export ARDOUR_SURFACES_PATH=""

    # Filter GTK2 / GTKmm2 library paths out of LD_LIBRARY_PATH
    NEW_LD=""
    IFS=':' read -ra PATHS <<< "$LD_LIBRARY_PATH"
    for p in "${PATHS[@]}"; do
        case "$p" in
            *gtkmm2ext*|*clearlooks*|*ytk*|*ydk*|*ztk*) ;;
            *) NEW_LD="${NEW_LD:+$NEW_LD:}$p" ;;
        esac
    done
    export LD_LIBRARY_PATH="$NEW_LD"
fi


exec "$SELECTED_EXE" "${@:2}"

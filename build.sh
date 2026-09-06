#!/usr/bin/env bash
set -e

TOP="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"

# Directory to store tagged binaries
OUTPUT_DIR="built_binaries"
mkdir -p "$OUTPUT_DIR"

# ── 1. Determine next build tag ──────────────────────────────────────
HIGHEST_BUILD=0
if [ -d "$OUTPUT_DIR" ]; then
  for file in "$OUTPUT_DIR"/*; do
    if [ -f "$file" ]; then
      filename=$(basename "$file")
      if [[ "$filename" =~ \#build([0-9]+) ]]; then
        num="${BASH_REMATCH[1]}"
        if (( num > HIGHEST_BUILD )); then
          HIGHEST_BUILD=$num
        fi
      fi
    fi
  done
fi

NEXT_BUILD=$((HIGHEST_BUILD + 1))
BUILD_TAG="#build${NEXT_BUILD}"

echo "=========================================="
echo " Starting Ardour Plus Build ${BUILD_TAG}"
echo "=========================================="

JOBS=$(( $(nproc) > 2 ? $(nproc) - 1 : 1 ))

# ── 2. GTK2 / Core Ardour (WAF) ──────────────────────────────────────
echo ""
echo "[1/4] Configuring GTK2 Ardour core (WAF)..."
EXTRA_CONF_FLAGS=()
if [ -d "/tmp/boost_inc" ]; then
  EXTRA_CONF_FLAGS+=("--boost-include=/tmp/boost_inc")
fi
./waf configure "${EXTRA_CONF_FLAGS[@]}" "$@"

echo "[2/4] Building GTK2 Ardour core (WAF, -j${JOBS})..."
./waf build -j "$JOBS"

# ── 3. GTK4 Ardour (Meson — Phase 0: adw_prefs) ─────────────────────
GTK4_DIR="$TOP/gtk4_ardour"
GTK4_BUILDDIR="$GTK4_DIR/builddir"

echo ""
echo "[3/4] Building GTK4 Ardour (Meson — Phase 0: ardour4-prefs)..."

if [ ! -f "$GTK4_BUILDDIR/build.ninja" ]; then
  echo "  -> Running 'meson setup' for gtk4_ardour..."
  meson setup "$GTK4_BUILDDIR" "$GTK4_DIR"
else
  echo "  -> Reconfiguring existing Meson build dir..."
  meson setup --reconfigure "$GTK4_BUILDDIR" "$GTK4_DIR"
fi

ninja -C "$GTK4_BUILDDIR" -j "$JOBS"

# ── 4. Qt6 QML Ardour (CMake) ──────────────────────────────────────────
QT6_DIR="$TOP/qt6_ardour"
QT6_BUILDDIR="$QT6_DIR/build"

echo ""
echo "[4/5] Building Qt6 QML Ardour (CMake)..."
cmake -B "$QT6_BUILDDIR" -S "$QT6_DIR"
cmake --build "$QT6_BUILDDIR" -j "$JOBS"

# ── 5. Archive binaries ───────────────────────────────────────────────
echo ""
echo "[5/5] Archiving built binaries with tag ${BUILD_TAG} into '${OUTPUT_DIR}'..."
FOUND_BINARIES=0

# --- GTK2 / WAF binaries ---
while IFS= read -r bin_path; do
  if [ -f "$bin_path" ] && [ -x "$bin_path" ]; then
    base_name=$(basename "$bin_path")
    if [[ "$base_name" != *.so* && "$base_name" != *.dylib && "$base_name" != *.dll && "$base_name" != *.py ]]; then
      DEST_NAME="${base_name}${BUILD_TAG}"
      cp "$bin_path" "$OUTPUT_DIR/$DEST_NAME"
      echo "  -> Preserved (GTK2): ${OUTPUT_DIR}/${DEST_NAME}"
      FOUND_BINARIES=$((FOUND_BINARIES + 1))
    fi
  fi
done < <(find build/gtk2_ardour build/session_utils build/headless -maxdepth 2 -type f -executable 2>/dev/null)

# Fallback: scan the whole build/ directory for ardour executables
if [ "$FOUND_BINARIES" -eq 0 ]; then
  while IFS= read -r bin_path; do
    base_name=$(basename "$bin_path")
    if [[ "$base_name" != *.so* && "$base_name" != *.dylib && "$base_name" != *.dll && "$base_name" != *.py ]]; then
      DEST_NAME="${base_name}${BUILD_TAG}"
      cp "$bin_path" "$OUTPUT_DIR/$DEST_NAME"
      echo "  -> Preserved (GTK2 fallback): ${OUTPUT_DIR}/${DEST_NAME}"
      FOUND_BINARIES=$((FOUND_BINARIES + 1))
    fi
  done < <(find build/ -type f -executable \( -name "ardour*" -o -name "gtk2_ardour*" -o -name "mixbus*" \) 2>/dev/null)
fi

# --- GTK4 / Meson binaries ---
for gtk4_bin_name in "ardour4-prefs" "ardour4-dialogs"; do
  GTK4_BIN="$(find "$GTK4_BUILDDIR" -type f -name "$gtk4_bin_name" 2>/dev/null | head -1)"
  if [ -n "$GTK4_BIN" ] && [ -x "$GTK4_BIN" ]; then
    DEST_NAME="${gtk4_bin_name}${BUILD_TAG}"
    cp "$GTK4_BIN" "$OUTPUT_DIR/$DEST_NAME"
    echo "  -> Preserved (GTK4): ${OUTPUT_DIR}/${DEST_NAME}"
    FOUND_BINARIES=$((FOUND_BINARIES + 1))
  fi
done

# --- Qt6 / CMake binaries ---
QT6_BIN="$QT6_BUILDDIR/ardour4-qt"
if [ -f "$QT6_BIN" ] && [ -x "$QT6_BIN" ]; then
  DEST_NAME="ardour4-qt${BUILD_TAG}"
  cp "$QT6_BIN" "$OUTPUT_DIR/$DEST_NAME"
  echo "  -> Preserved (Qt6): ${OUTPUT_DIR}/${DEST_NAME}"
  FOUND_BINARIES=$((FOUND_BINARIES + 1))
fi

# Also copy the WAF env-setup script so run.sh can source it
WAF_ENV_SCRIPT="$TOP/build/gtk2_ardour/ardev_common_waf.sh"
if [ -f "$WAF_ENV_SCRIPT" ]; then
  DEST_ENV="$OUTPUT_DIR/ardev_common_waf.sh${BUILD_TAG}"
  cp "$WAF_ENV_SCRIPT" "$DEST_ENV"
  echo "  -> Preserved (env): ${OUTPUT_DIR}/$(basename "$DEST_ENV")"
fi

# ── 5. Prune old builds (keep last 3) ────────────────────────────────
if [ -d "$OUTPUT_DIR" ]; then
  ALL_BUILDS=($(ls "$OUTPUT_DIR"/*#build* 2>/dev/null | grep -o '#build[0-9]\+' | sort -uV))
  NUM_BUILDS=${#ALL_BUILDS[@]}
  if [ "$NUM_BUILDS" -gt 3 ]; then
    REMOVE_COUNT=$((NUM_BUILDS - 3))
    for (( i=0; i<REMOVE_COUNT; i++ )); do
      OLD_TAG="${ALL_BUILDS[$i]}"
      rm -f "$OUTPUT_DIR"/*"$OLD_TAG"*
      echo "  -> Pruned old build: $OLD_TAG"
    done
  fi
fi

echo ""
echo "=========================================="
echo " Build ${BUILD_TAG} successfully finished!"
echo " Total binaries archived: ${FOUND_BINARIES}"
echo " Binaries folder: file://$(pwd)/${OUTPUT_DIR}"
echo "=========================================="


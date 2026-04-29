[![Build](https://github.com/vberthiaume/dupe/actions/workflows/build_and_test.yml/badge.svg?branch=main)](https://github.com/vberthiaume/dupe/actions/workflows/build_and_test.yml)
[![License: AGPL v3](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

# Dupe — JUCE plugin starter

A starting point for a new JUCE audio-plugin project. Adds a CI sanitizer matrix (ASan, UBSan, TSan, RTSan), clang-tidy as a PR-comment bot, RTSan-aware attribute macros, and a clang-format pre-commit hook to the [Pamplejuce](https://github.com/sudara/pamplejuce) template. The placeholder plugin name is `Dupe` — see [Rename](#rename-dupe--your-plugin-name) for what to change when forking.

## Rename `Dupe` → your plugin name

Pick names. Example values for a hypothetical "Spangle" plugin by "Acme Audio":

| Identifier | What it is | Example |
|---|---|---|
| `PROJECT_NAME` | Internal CMake target name (no spaces) | `Spangle` |
| `PRODUCT_NAME` | Display name shown in DAWs (spaces OK) | `Spangle` |
| `COMPANY_NAME` | Manufacturer | `Acme Audio` |
| `BUNDLE_ID` | macOS reverse-DNS bundle id | `com.acmeaudio.spangle` |
| `PLUGIN_MANUFACTURER_CODE` | 4 chars, **first uppercase** | `Acme` |
| `PLUGIN_CODE` | 4 chars, ≥1 uppercase, unique per plugin | `Spgl` |

Then update each of these locations:

| File | Change |
|---|---|
| `CMakeLists.txt` | `PROJECT_NAME`, `PRODUCT_NAME`, `COMPANY_NAME`, `BUNDLE_ID`, `PLUGIN_MANUFACTURER_CODE`, `PLUGIN_CODE`, `PRODUCT_NAME_WITHOUT_VERSION` |
| `tests/PluginBasics.cpp` | The expected name string in the `Plugin instance` test |
| `packaging/dmg.json` | `title`, `icon`, and the `dmg/Dupe.*` paths |
| `packaging/resources/README` | Installer thank-you text |
| `packaging/dupe.icns` | Rename file to match new product name **and** replace the image with your own. The macOS CI workflow uses this to brand the `.vst3`/`.au`/`.clap` bundles with a custom Finder icon — leave the placeholder and your plugins will look like the Dupe template. Also update the `sips`/`DeRez` lines in `.github/workflows/build_and_test.yml` to point at the renamed file. |
| `packaging/icon.png` | Generic placeholder used as `ICON_BIG` for the Standalone app (referenced from `CMakeLists.txt`). Replace the image with your own; the filename can stay since it's not name-specific. |
| `.github/workflows/build_and_test.yml` | Workflow `name:` (line 1) |
| `README.md` | Build badge URL (point at your repo), `License` section copyright line, the title at the top |
| `LICENSE` / `README.md` License section | Update the copyright holder if appropriate |

Shortcut: editor-wide case-aware find-and-replace of `Dupe` → `Spangle` and `dupe` → `spangle`, then double-check that the `.icns` got renamed — most find/replace tools skip binary files.

## Install dependencies
### macOS
```bash
xcode-select --install         # Apple's clang + git + make
brew install cmake ninja       # Homebrew: https://brew.sh
```

### Linux (Ubuntu / Debian)
```bash
sudo apt update
sudo apt install -y \
  cmake ninja-build clang lld \
  libasound2-dev libx11-dev libxinerama-dev libxext-dev \
  libfreetype6-dev libwebkit2gtk-4.1-dev libglu1-mesa-dev
```

### Windows
Install in this order:
1. **[Visual Studio 2022](https://visualstudio.microsoft.com/)** with the "Desktop development with C++" workload (provides MSVC + the Windows SDK).
2. **[CMake](https://cmake.org/download/)** (add to PATH during install).
3. **[Ninja](https://github.com/ninja-build/ninja/releases)** on PATH (or `choco install ninja`).
4. **[Git for Windows](https://git-scm.com/download/win)** if you don't already have it.

## Install the pre-commit hook

One-time, per clone. Refuses commits whose staged C/C++ files aren't clang-format clean (see `.githooks/pre-commit`):

```bash
git config core.hooksPath .githooks
```

## Build
```bash
cmake -B Builds -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build Builds
ctest --test-dir Builds --output-on-failure
```

For a universal macOS binary, add `-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"` to the configure step.

## Binary assets
JUCE can embed arbitrary binary files (images, sounds, fonts) directly into each plugin format, exposing them at runtime via the `BinaryData::` namespace. The `assets/` folder ships with a placeholder `images/pamplejuce.png` (the original Pamplejuce template banner) so the binary-data target always has at least one input — replace it with your own when you fork.

Drop new files anywhere inside `assets/`. Subdirectories are flattened, so don't reuse the same basename across folders — the symbols would collide. On the next build they're auto-bundled:

```
assets/images/logo.png   →  BinaryData::logo_png   +  BinaryData::logo_pngSize
assets/sounds/click.wav  →  BinaryData::click_wav  +  BinaryData::click_wavSize
```

Wired up by `cmake/Assets.cmake` (a recursive glob). `juce_add_binary_data()` requires at least one source file, which is why we ship the placeholder rather than leaving the folder empty.

## CI

Every push and PR triggers:
- `build_and_test` — Linux/macOS/Windows, `pluginval` validation, artifact upload
- `sanitizers` — ASan / UBSan / TSan / RTSan (clang-20 for the latter)
- `clang-tidy` — posts review comments on PRs

`nightly.yml` runs the same fan-out daily at 10:00 UTC, to catch external drift (JUCE submodule on `develop`, apt packages, GitHub runner images) between commits. Disable by commenting out the `schedule:` block in that file if you don't want the daily runs.

## License
Dupe is released under the [GNU Affero General Public License, version 3](LICENSE) (AGPLv3). Copyright (C) 2026 Vincent Berthiaume.

This project links against [JUCE](https://juce.com/), used under the AGPLv3 free-use option of JUCE Ltd's dual-license terms.

### Third-party attribution
Portions of the build system and project scaffolding derive from the [Pamplejuce](https://github.com/sudara/pamplejuce) template, which is distributed under the MIT License:

> MIT License
>
> Copyright (c) 2022 Sudara Williams
>
> Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
>
> The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
>
> THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

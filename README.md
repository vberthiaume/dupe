[![Build](https://github.com/vberthiaume/dupe/actions/workflows/build_and_test.yml/badge.svg?branch=main)](https://github.com/vberthiaume/dupe/actions/workflows/build_and_test.yml)
[![License: AGPL v3](https://img.shields.io/badge/License-AGPL_v3-blue.svg)](https://www.gnu.org/licenses/agpl-3.0)

# Dupe — JUCE plugin starter

This repository is meant to be **cloned (or used as a template) as the starting point for a new JUCE audio-plugin project**. It's derived from [Pamplejuce](https://github.com/sudara/pamplejuce) with extra tooling layered on: sanitizer matrix in CI, clang-tidy as a PR-comment bot, RTSan-aware attribute macros, an opinionated VS Code dev loop, and a pre-commit hook that enforces clang-format. The placeholder plugin name throughout is `Dupe`; the [Getting started](#getting-started) section below lists everything to rename when you fork it for your own plugin.

# Rename `Dupe` → your plugin name

Here's how to rename everything in this repo after you've cloned it and fetchee all submodules.

Pick four identifiers up front. Recommended values shown for a hypothetical "Spangle" plugin by "Acme Audio":

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
| `.vscode/launch.json` | Replace `Dupe_artefacts` and `Dupe.app` / `Dupe.exe` / `Dupe` paths |
| `README.md` | Build badge URL (point at your repo), `License` section copyright line, the title at the top |
| `LICENSE` / `README.md` License section | Update the copyright holder if appropriate |

A reasonable shortcut: open the repo in your editor and do a case-aware find-and-replace of `Dupe` → `Spangle` and `dupe` → `spangle`, then verify the `.icns` file actually got renamed (find/replace tools usually skip binary file renames).

# 3. Install dependencies

## macOS
```bash
xcode-select --install         # Apple's clang + git + make
brew install cmake ninja       # Homebrew: https://brew.sh
```

## Linux (Ubuntu / Debian)
```bash
sudo apt update
sudo apt install -y \
  cmake ninja-build clang lld \
  libasound2-dev libx11-dev libxinerama-dev libxext-dev \
  libfreetype6-dev libwebkit2gtk-4.1-dev libglu1-mesa-dev
```

## Windows
Install in this order:
1. **[Visual Studio 2022](https://visualstudio.microsoft.com/)** with the "Desktop development with C++" workload (provides MSVC + the Windows SDK).
2. **[CMake](https://cmake.org/download/)** (add to PATH during install).
3. **[Ninja](https://github.com/ninja-build/ninja/releases)** on PATH (or `choco install ninja`).
4. **[Git for Windows](https://git-scm.com/download/win)** if you don't already have it.

### 4. Install the pre-commit hook (one-time, per clone)
The hook lives in `.githooks/pre-commit` and refuses commits whose staged C/C++ files aren't clang-format clean. It just needs to be wired up:
```bash
git config core.hooksPath .githooks
```

Verify with `git config --get core.hooksPath` — should print `.githooks`. The hook self-documents its install instructions at the top of the file.

### 5. Build
VS Code is the recommended dev loop (`F5` debug, `F7` build) — see below — but you can also build from the terminal:

```bash
cmake -B Builds -G Ninja -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build Builds
ctest --test-dir Builds --output-on-failure
```

For a universal macOS binary, add `-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"` to the configure step.

## Local development
The repo ships a `.vscode/` config that gives you:

- **`F7`** — runs the **Build** task (uses the existing `Builds/` configuration; run **CMake: Configure (Debug)** once first, or after CMakeLists.txt changes).
- **`F5`** — debugs the Standalone app via CodeLLDB by default (path: `Builds/<PROJECT_NAME>_artefacts/Debug/Standalone/<PRODUCT_NAME>.app`). A cppdbg fallback launch config is also provided if you don't have CodeLLDB.
- **clangd** as the IntelliSense provider (the Microsoft C/C++ extension's IntelliSense is disabled in workspace settings to avoid double-diagnostics; install [`llvm-vs-code-extensions.vscode-clangd`](https://marketplace.visualstudio.com/items?itemName=llvm-vs-code-extensions.vscode-clangd) for full functionality).
- Format-on-save via `.clang-format`.

> [!NOTE]
> `F7` isn't a default VS Code keybinding for builds — to map it, add this to your **user** `keybindings.json`:
> ```json
> { "key": "f7", "command": "workbench.action.tasks.build" }
> ```
> `F5` is built-in (Debug: Start Debugging).

clangd may report false-positive errors like `'juce_audio_processors/juce_audio_processors.h' file not found` until the first successful Configure step generates `compile_commands.json` in `Builds/`. Ignore those unless the actual build fails.

## Binary assets

JUCE can embed arbitrary binary files (images, sounds, fonts) directly into each plugin format, exposing them at runtime via the `BinaryData::` namespace. The `assets/` folder ships with a placeholder `images/pamplejuce.png` (the original Pamplejuce template banner) so the binary-data target always has at least one input — replace it with your own when you fork.

Drop new files anywhere inside `assets/`. Subdirectories are flattened, so don't reuse the same basename across folders — the symbols would collide. On the next build they're auto-bundled:

```
assets/images/logo.png   →  BinaryData::logo_png   +  BinaryData::logo_pngSize
assets/sounds/click.wav  →  BinaryData::click_wav  +  BinaryData::click_wavSize
```

Wired up by `cmake/Assets.cmake` (a recursive glob). `juce_add_binary_data()` requires at least one source file, which is why we ship the placeholder rather than leaving the folder empty.

## CI

Push to any branch or open a PR to trigger the full CI fan-out:
- `build_and_test` — three platforms (Linux/macOS/Windows), `pluginval` validation, artifact upload
- `sanitizers` — ASan / UBSan / TSan / RTSan (clang-20 for the latter)
- `clang-tidy` — posts review comments on PRs

The nightly workflow (`nightly.yml`) is currently disabled. Re-enable by uncommenting the cron block when active development pauses and external dependency drift becomes the main breakage risk.

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

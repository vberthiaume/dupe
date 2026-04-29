![PAMPLEJUCE](assets/images/pamplejuce.png)
[![](https://github.com/sudara/pamplejuce/actions/workflows/build_and_test.yml/badge.svg)](https://github.com/sudara/pamplejuce/actions)

Pamplejuce is a ~~template~~ lifestyle for creating and building JUCE plugins in 2026.

Out-of-the-box, it:

1. Runs C++23
2. Uses JUCE 8.x as a git submodule (tracking develop).
3. Uses CPM for dependency management.
3. Relies on CMake 3.25 and higher for cross-platform building.
4. Has [Catch2](https://github.com/catchorg/Catch2) v3.7.1 for the test framework and runner.
5. Includes a `Tests` target and a `Benchmarks` target with examples to get started quickly.
6. Has [Melatonin Inspector](https://github.com/sudara/melatonin_inspector) installed as a JUCE module to help relieve headaches when building plugin UI.

It also has integration with GitHub Actions, specifically:

1. Building and testing cross-platform (linux, macOS, Windows) binaries
2. Running tests and benchmarks in CI
3. Running [pluginval](http://github.com/tracktion/pluginval) 1.x against the binaries for plugin validation
4. Config for [installing Intel IPP](https://www.intel.com/content/www/us/en/developer/tools/oneapi/ipp.html)
5. [Code signing and notarization on macOS](https://melatonin.dev/blog/how-to-code-sign-and-notarize-macos-audio-plugins-in-ci/)
6. [Windows code signing via Azure Trusted Signing](https://melatonin.dev/blog/code-signing-on-windows-with-azure-trusted-signing/)

It also contains:

1. A `.gitignore` for all platforms.
2. A `.clang-format` file for keeping code tidy.
3. A `VERSION` file that will propagate through JUCE and your app.
4. A ton of useful comments and options around the CMake config.

## How does this all work at a high level?

Check out the [official Pamplejuce documentation](https://melatonin.dev/manuals/pamplejuce/how-does-this-all-work/).

[![Arc - 2024-10-01 51@2x](https://github.com/user-attachments/assets/01d19d2d-fbac-481f-8cec-e9325b2abe57)](https://melatonin.dev/manuals/pamplejuce/how-does-this-all-work/)

## Setting up for YOUR project

This is a template repo!

That means you can click "[Use this template](https://github.com/sudara/pamplejuce/generate)" here or at the top of the page to get your own copy (not fork) of the repo. Then you can make it private or keep it public, up to you.

Then check out the [documentation](https://melatonin.dev/manuals/pamplejuce/setting-your-project-up/) so you know what to tweak. 

> [!NOTE]
> Tests will immediately run and fail (go red) until you [set up code signing](https://melatonin.dev/manuals/pamplejuce/getting-started/code-signing/).

## Pre-commit hook

A clang-format pre-commit hook lives in `.githooks/pre-commit` and refuses commits whose staged C/C++ files aren't formatted per `.clang-format`.

Install `clang-format` (the hook depends on it being on PATH):
- **macOS:** `brew install clang-format` (Apple's bundled clang doesn't ship it as a separate binary)
- **Linux (Ubuntu/Debian):** `sudo apt install clang-format`
- **Windows:** ships with Visual Studio's "Desktop development with C++" workload, or with LLVM

Then enable the hook (one-time, per clone):

```bash
git config core.hooksPath .githooks
```

Verify with `git config --get core.hooksPath` — should print `.githooks`.

## Having Issues?

Thanks to everyone who has contributed to the repository. 

This repository covers a _lot_ of ground. JUCE itself has a lot of surface area. It's a group effort to maintain the garden and keep things nice!

If something isn't just working out of the box — *it's probably not just you* — others are running into the problem, too, I promise. Check out [the official docs](https://melatonin.dev/manuals/pamplejuce), then please do [open an issue](https://github.com/sudara/pamplejuce/issues/new)!

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

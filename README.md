[![GitHub license](https://img.shields.io/badge/license-MIT-informational.svg)](https://github.com/Open-Acidification/TankControllerLib/blob/master/LICENSE)
[![OA Box version](https://img.shields.io/badge/TankControllerLib-v0.0.0-informational.svg)](https://github.com/Open-Acidification/TankControllerLib/releases)
[![testing status](https://github.com/Open-Acidification/TankControllerLib/workflows/Arduino%20CI/badge.svg)](https://github.com/Open-Acidification/TankControllerLib/actions)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](https://github.com/Open-Acidification/TankControllerLib/blob/master/CONTRIBUTING.md)
<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-12-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

# Tank Controller Library

Arduino library for the Open Acidification pH Stat Controller

## Use

### Development

*   Clone (or fork and clone if you might contribute to the project) this repository to the [Arduino Libraries directory](https://www.arduino.cc/en/Hacking/Libraries).
*   Use [`scripts/install.sh`](scripts/install.sh) to do the initial install.
*   After that use [`scripts/testAndBuild.sh`](scripts/testAndBuild.sh) to test.
*   To build the GUI simulator, see [GUI/build.sh](GUI/build.sh).

### Install

To install onto an Arduino Mega2560 use the [Arduino IDE](https://www.arduino.cc/en/software).

By default, the Arduino compiler links to a `printf` library that does not support printing floating point numbers. To get that feature (which we use!), add the following lines to `boards.txt` and use the Tools menu to select "`RV printf Version: "Full printf"`. On macOS, the `boards.txt` is located at `~/Library/Arduino15/packages/arduino/hardware/avr/1.8.3`. For background see [here](https://forum.arduino.cc/t/no-sprintf-float-formatting-come-back-five-year/331790/6).

```text
menu.printf=AVR printf Version

mega.menu.printf.default=Default printf
mega.menu.printf.default.compiler.c.elf.extra_flags=
mega.menu.printf.full=Full printf
mega.menu.printf.full.compiler.c.elf.extra_flags=-Wl,-u,vfprintf -lprintf_flt
mega.menu.printf.minimal=Minimal printf
mega.menu.printf.minimal.compiler.c.elf.extra_flags=-Wl,-u,vfprintf -lprintf_min
```

## Contributors ✨

Thanks goes to these wonderful people ([emoji key](https://allcontributors.org/docs/en/emoji-key)):

<!-- ALL-CONTRIBUTORS-LIST:START - Do not remove or modify this section -->
<!-- prettier-ignore-start -->
<!-- markdownlint-disable -->
<table>
  <tr>
    <td align="center"><a href="https://gab.wallawalla.edu/~kirt.onthank/index.html"><img src="https://avatars.githubusercontent.com/u/48142545?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Kirt Onthank</b></sub></a><br /><a href="#business-KirtOnthank" title="Business development">💼</a> <a href="https://github.com/Open-Acidification/TankController/commits?author=KirtOnthank" title="Code">💻</a> <a href="#design-KirtOnthank" title="Design">🎨</a> <a href="#ideas-KirtOnthank" title="Ideas, Planning, & Feedback">🤔</a> <a href="#mentoring-KirtOnthank" title="Mentoring">🧑‍🏫</a></td>
    <td align="center"><a href="https://github.com/prestoncarman"><img src="https://avatars.githubusercontent.com/u/3517157?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Preston Carman</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/issues?q=author%3Aprestoncarman" title="Bug reports">🐛</a> <a href="https://github.com/Open-Acidification/TankController/commits?author=prestoncarman" title="Code">💻</a> <a href="#ideas-prestoncarman" title="Ideas, Planning, & Feedback">🤔</a> <a href="#mentoring-prestoncarman" title="Mentoring">🧑‍🏫</a> <a href="https://github.com/Open-Acidification/TankController/pulls?q=is%3Apr+reviewed-by%3Aprestoncarman" title="Reviewed Pull Requests">👀</a></td>
    <td align="center"><a href="http://programminggems.wordpress.com/"><img src="https://avatars.githubusercontent.com/u/1577872?v=4?s=100" width="100px;" alt=""/><br /><sub><b>James Foster</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=jgfoster" title="Code">💻</a> <a href="#ideas-jgfoster" title="Ideas, Planning, & Feedback">🤔</a> <a href="#mentoring-jgfoster" title="Mentoring">🧑‍🏫</a> <a href="#projectManagement-jgfoster" title="Project Management">📆</a> <a href="https://github.com/Open-Acidification/TankController/pulls?q=is%3Apr+reviewed-by%3Ajgfoster" title="Reviewed Pull Requests">👀</a> <a href="https://github.com/Open-Acidification/TankController/issues?q=author%3Ajgfoster" title="Bug reports">🐛</a></td>
    <td align="center"><a href="https://github.com/Kavikick"><img src="https://avatars.githubusercontent.com/u/50475639?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Kavikick</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=Kavikick" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/eucalvo"><img src="https://avatars.githubusercontent.com/u/71796520?v=4?s=100" width="100px;" alt=""/><br /><sub><b>eucalvo</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=eucalvo" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/Lizj96"><img src="https://avatars.githubusercontent.com/u/71606160?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Lizj96</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=Lizj96" title="Code">💻</a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/khalilxl"><img src="https://avatars.githubusercontent.com/u/26103228?v=4?s=100" width="100px;" alt=""/><br /><sub><b>khalilxl</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=khalilxl" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/atg7000"><img src="https://avatars.githubusercontent.com/u/38963069?v=4?s=100" width="100px;" alt=""/><br /><sub><b>atg7000</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=atg7000" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/cherbel"><img src="https://avatars.githubusercontent.com/u/36583106?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Caleb Herbel</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=cherbel" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/lsaca05"><img src="https://avatars.githubusercontent.com/u/46550669?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Lucas Saca</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=lsaca05" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/EvanKropf"><img src="https://avatars.githubusercontent.com/u/59894430?v=4?s=100" width="100px;" alt=""/><br /><sub><b>EvanKropf</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=EvanKropf" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/genevea"><img src="https://avatars.githubusercontent.com/u/83893683?v=4?s=100" width="100px;" alt=""/><br /><sub><b>genevea</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=genevea" title="Code">💻</a></td>
  </tr>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!

[![GitHub license](https://img.shields.io/badge/License-MPL%202.0-brightgreen.svg)](https://github.com/Open-Acidification/TankController/blob/master/LICENSE)
[![OA Box version](https://img.shields.io/badge/TankController-v21.09.1-informational.svg)](https://github.com/Open-Acidification/TankController/releases)
[![testing status](https://github.com/Open-Acidification/TankController/workflows/Arduino%20CI/badge.svg)](https://github.com/Open-Acidification/TankController/actions)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-brightgreen.svg)](https://github.com/Open-Acidification/TankController/blob/master/CONTRIBUTING.md)
<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-19-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

# Tank Controller Library

Arduino library for the Open Acidification pH Stat Controller

## Use

### Development

*   Clone (or fork and clone if you might contribute to the project) this repository to the [Arduino Libraries directory](https://www.arduino.cc/en/Hacking/Libraries).
*   Use [`extras/scripts/install.sh`](extras/scripts/install.sh) to do the initial install.
*   After that use [`extras/scripts/testAndBuild.sh`](extras/scripts/testAndBuild.sh) to test.
*   To build the GUI simulator, see [GUI/build.sh](GUI/build.sh).

### Install

To install onto an Arduino Mega2560 use the [Arduino IDE](https://www.arduino.cc/en/software).

## Menus

To navigate through the menus, press 2, 4, 6, or 8 on the keypad.

The following are "hidden" commands:
*   From the 'SeeDeviceAddress' command:
    *   'B' goes into an infinite loop that should trigger a reset by the watchdog timer in 8 seconds
    *   'C' regenerates a new (random) MAC address that will take effect on the next restart
*   On startup you can avoid the 60-second DHCP attempt by pressing any key

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
    <td align="center"><a href="https://github.com/eucalvo"><img src="https://avatars.githubusercontent.com/u/71796520?v=4?s=100" width="100px;" alt=""/><br /><sub><b>eucalvo</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=eucalvo" title="Code">💻</a> <a href="https://github.com/Open-Acidification/TankController/issues?q=author%3Aeucalvo" title="Bug reports">🐛</a></td>
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
  <tr>
    <td align="center"><a href="https://github.com/thomca"><img src="https://avatars.githubusercontent.com/u/56854904?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Catherine</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=thomca" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/IDzyre"><img src="https://avatars.githubusercontent.com/u/46759635?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Dzyre</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=IDzyre" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/Paydenb"><img src="https://avatars.githubusercontent.com/u/83844058?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Paydenb</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=Paydenb" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/TaylorSmith28"><img src="https://avatars.githubusercontent.com/u/83837157?v=4?s=100" width="100px;" alt=""/><br /><sub><b>TaylorSmith28</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=TaylorSmith28" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/BenjaminWachter"><img src="https://avatars.githubusercontent.com/u/81596850?v=4?s=100" width="100px;" alt=""/><br /><sub><b>benwach</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=BenjaminWachter" title="Code">💻</a></td>
    <td align="center"><a href="https://github.com/je-foster"><img src="https://avatars.githubusercontent.com/u/109112235?v=4?s=100" width="100px;" alt=""/><br /><sub><b>John Foster</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/commits?author=je-foster" title="Tests">⚠️</a> <a href="https://github.com/Open-Acidification/TankController/commits?author=je-foster" title="Code">💻</a></td>
  </tr>
  <tr>
    <td align="center"><a href="https://github.com/pianistrevor"><img src="https://avatars.githubusercontent.com/u/31118933?v=4?s=100" width="100px;" alt=""/><br /><sub><b>Trevor Natiuk</b></sub></a><br /><a href="https://github.com/Open-Acidification/TankController/issues?q=author%3Apianistrevor" title="Bug reports">🐛</a> <a href="https://github.com/Open-Acidification/TankController/commits?author=pianistrevor" title="Tests">⚠️</a> <a href="https://github.com/Open-Acidification/TankController/commits?author=pianistrevor" title="Code">💻</a></td>
  </tr>
</table>

<!-- markdownlint-restore -->
<!-- prettier-ignore-end -->

<!-- ALL-CONTRIBUTORS-LIST:END -->

This project follows the [all-contributors](https://github.com/all-contributors/all-contributors) specification. Contributions of any kind welcome!

## Other

*   [Libraries](docs/libraries.md) lists the external libraries and their licenses.

# Tank Controller Manager
The Tank Controller Manager (TCM) works to monitor, operate, and consolidate multiple tank controllers. A tank controller regulates tank pH through CO2 addition and temperature. The purpose of the TCM is to allow over-the-network access to tank controllers while facilitating research by reporting data. Created in Flutter with the programming language Dart, the TCM runs on virtually every main platform.

## Installation and Setup
1. After forking to repository, install the Dart and Flutter SDKs here: [Dart](https://dart.dev/get-dart) and [Flutter](https://docs.flutter.dev/get-started/install).
2. If using an IDE such as VSCode, the Dart and Flutter extensions can be useful. 
3. The directory for the Tank Controller Manager is at /extras/webClient, which is where you will want to run the majority of commands. All dart files for the project are within this directory. 
4. For first time setup or when updating packages, run 'flutter pub get' to load necessary packages for the project. With the Dart and Flutter extensions for VSCode, this will run automatically. 
5. The Google Chrome Browser is used to locally run and debug this project, and you can install it here: [Chrome](https://www.google.com/chrome/).
6. Set Chrome as the default device for the project . With VSCode and the Dart/Flutter extensions, this can be set in the bottom right corner. 
7. The project can then be run locally using the command 'flutter run' in the terminal, or with the IDE's run/debug commands. 

## Helpful Commands
- 'dart analyze' or 'flutter analyze' to run the linter for the project.
- 'flutter test --concurrency=1’ to run the existing tests for the project. This can also be done automatically with the testing tab of your IDE. 
- 'dart format [file path]' can be used to format individual files. Your IDE may also have a formatting option. VSCode can do this with the Format Document option when right clicking. 

## Github
Github actions will automatically run the linter and all tests and check for formatting and spelling when a new commit is made or a pull request is submitted. As the repository is also shared with the code for the Tank Controller, tests for Arduino Lint, Arduino CI, and clang format will also be run at the same time, but are not affected by the Tank Controller Manager project. 

## Deployment
The web application is built automatically on each commit to main and can be viewed [here](https://open-acidification.github.io/TankControllerManager/).
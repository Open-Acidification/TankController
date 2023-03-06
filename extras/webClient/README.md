# Tank Controller Manager
The Tank Controller Manager (TCM) works to monitor, operate, and consolidate multiple tank controllers. A tank controller regulates tank pH through CO2 addition and temperature. The purpose of the TCM is to allow over-the-network access to tank controllers while facilitating research by reporting data. Created in Flutter with the programming language Dart, the TCM runs on virtually every main platform.


## Deployment
The web application is built automatically on each commit to main and can be viewed [here](https://open-acidification.github.io/TankControllerManager/).
### Web
1. Open a terminal and run
```
flutter build web
```
2. Navigate to TankControllerManager/build/web and run
```python
python -m http.server 8000
```

### Android

#### Signed Appbundle
https://docs.flutter.dev/deployment/android
1. Open a terminal and run
```
keytool -genkey -v -keystore c:\key.jks -storetype JKS -keyalg RSA -keysize 2048 -validity 1000 -alias upload
```

2. Create a file named TankControllerManager/android/key.properties and in it
```
storePassword=<password from step one>
keyPassword=<password from step one>
keyAlias=<alias from step one>
storeFile=<location of output from step one, may require double backslashes '\\' in file path, c:\\key.jks>
```
3. Run
```
flutter build appbundle
```
#### Unsigned APK
1. Open a terminal and run
```
flutter build apk --split-per-abi
```
#### Debugging
1. Ensure Android Studio, Android SDK, Android SDK Platform-Tools, and Android SDK Build-Tools, are all updated. Run
```
flutter doctor
```
2. Enable developer options and USB debugging on the device.
3. Once plugged in, run
```
flutter devices
```
4. Finally run
```
flutter run -d deviceID
```

# Contributors
<!-- ALL-CONTRIBUTORS-BADGE:START - Do not remove or modify this section -->
[![All Contributors](https://img.shields.io/badge/all_contributors-2-orange.svg?style=flat-square)](#contributors-)
<!-- ALL-CONTRIBUTORS-BADGE:END -->

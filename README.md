<div id="header" align="center">
  <h1>Cloth Simulator</h1>
  <p><img src="https://user-images.githubusercontent.com/11558887/160651739-c9b34d29-505f-4f6a-8b96-0ca72ed87cd6.png" width="80%"/></p>
  <p>Cloth Simulator built for the Game Physics course of the GMTE Master program at Utrecht University. </p>
  <p><strong>Made with &#10084; by <a href="https://github.com/Markieautarkie">Markieautarkie</a> and <a href="https://github.com/PhantomDot1">PhantomDot1</a></strong></p>
  <a href="https://github.com/Markieautarkie/Cloth-Simulator/releases"><img src="https://img.shields.io/badge/release-1.0.0-success"/></a>
</div>

## Features
- [x] Cloth simulation by means of a mass spring system, including dampening
- [x] Wind simulation by calculating wind forces per triangle
- [x] Interaction with rigid spheres
- [x] Tearable cloth based on maximum spring stretching (rather unstable at the moment)

## Controls
- Zoom in and out with `+` and `-` keys
- Rotate around cloth with the `Mouse`
- Play/pause simulation with the `SpaceBar`
- Reset the cloth with the `R` key
- Make the cloth tearable/untearable with the `T` key, show tears or not with the `S` key
- Enable/disable wind force and ball position with the `W` and `B` keys respectively
- Use `1`, `2`, `3` and `4` to individually make the corners of the cloth static or dynamic

## Build instructions
No further build instructions.

Simply open the `.sln` file to run the application in Visual Studio.

### Dependencies
All dependencies have been included in the `lib` folder.

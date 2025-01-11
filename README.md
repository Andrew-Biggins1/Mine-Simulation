# Mine-Simulation
A program to simulate rock fractures in a mine from seismic activity

## Dependencies
Built using C++ with the SFML graphics library 
[SFML download page](https://www.sfml-dev.org/download/).

## Build
Built using the make file.
```
make
```
Will generate `mine` file.

## Usage
`@param 1 (mandatory): Seismic strength (1-10)`<br>
`@param 2 (optional): Number of strong points (0+, default = 10)`

```
./mine 5 15
```

**Left click**: Create a seismic wave at mouse location <br>
**Right click**: Display the current damage at mouse location in stdout

**Fracture**: When a fracture occurs it will display the coordinates in stdout and will turn black on the simulation


## Screenshots
<p align="top">
  <img src="pictures/window.png" width="320" alt="window/>
  </p>
<p align="center">
  <img src="pictures/log.png" width="150" alt="Log"/>
</p>



<p align="center">
	<h1 align="center">Bouncy squares</h2>
	<p align="center">Fun bouncing squares with C and SDL2</p>
</p>
<p align="center">
	<a href="./LICENSE">
		<img alt="License" src="https://img.shields.io/badge/license-GPL-blue?color=7aca00"/>
	</a>
	<a href="https://github.com/LordOfTrident/bouncy-squares/issues">
		<img alt="Issues" src="https://img.shields.io/github/issues/LordOfTrident/bouncy-squares?color=0088ff"/>
	</a>
	<a href="https://github.com/LordOfTrident/bouncy-squares/pulls">
		<img alt="GitHub pull requests" src="https://img.shields.io/github/issues-pr/LordOfTrident/bouncy-squares?color=0088ff"/>
	</a>
	<br><br><br>
	<img width="500px" src="res/clip.gif"/>
</p>

## Table of contents
* [Introduction](#introduction)
* [Quickstart](#quickstart)
* [Controls](#controls)
* [Bugs](#bugs)
* [Dependencies](#dependencies)
* [Make](#make)

## Introduction
Just let the squares bounce :)

## Quickstart
```sh
$ make
$ ./bin/app 12
```
The first parameter the squares count, but it is optional (default is 16 squares).

## Controls
| Key  | Action                          |
| ---- | ------------------------------- |
| R    | Respawn the squares             |
| C    | Crazy squares                   |
| M    | Shoot the squares at the mouse  |
| LMB  | Shoot a square                  |
| RMB  | Drag a square                   |

## Bugs
If you find any bugs, please create an issue and report them.

## Dependencies
- [SDL2](https://www.libsdl.org/)

## Make
Run `make all` to see all the make rules.

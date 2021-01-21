# MIDRES (library)
An isomorphic gamelib for retrocomputers

by Marco Spedaletti (asimov@mclink.it)

## ABOUT THIS LIBRARY
The MIDRES library allows to create **WORA** (*Write Once Run Anywhere*) programs that are [very efficient "by design"](https://retroprogramming.iwashere.eu/midres_library:isomorphism) on various 8-bit platforms, without the need for "fine tuning" them and without compromises of other dev-kits. 

**The isomorphic approach is different from abstraction.** It is based on mapping the underlying hardware without making use of third-party libraries and by matching complex and heterogeneous structures, albeit homologous, to each other. 

When the (client) code written with this approach is satisfactory (in terms of execution speed or use of resources), then **it is already optimized for any other platform**. 

For more information on this library, please [click here](https://retroprogramming.iwashere.eu/midres_library). 

## SUPPORTED ARCHITECTURES

The [programs](docs/programs.md) can be cross compiled for [many vintage systems](docs/targets.md). Due to limitations in the resources of the various retrocomputers, it is possible that not all programs can be compiled on all platforms. For a complete list of supported targets and status of buildings please visit [this page](docs/status.md). The instructions for running the programs on the various platforms can be found on [this page](docs/running.md).

## HOW TO BUILD

This library makes use of a series of tools, implicitly called by the makefile, to generate the executables. On [this page](docs/build.md) you will find a short guide to downloading and installing the necessary software. In the absence of these software, some or all of the registered targets may not be compiled.

## DIFFERENCES v1.x <-> v2.x

There are numerous differences between the v1.x version (no longer maintained) and the v2.x version. In addition to the ability to compile for Z80 processors (and more to come), the most important is **the optimization in terms of occupied space**: for example, the game ["The Elevator"](https://retroprogramming.iwashere.eu/elevator) for Commodore 64 goes from 109 total blocks (27.904 bytes) to only 86 blocks (22.016 bytes), with a memory saving of 23%.

## LICENSE
Copyright 2020-2021 Marco Spedaletti

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
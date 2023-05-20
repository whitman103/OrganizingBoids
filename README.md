<p align="center">
  <a href="https://example.com/">
    <img src="./logo.png" alt="Logo" width=140 height=150>
  </a>

  <h3 align="center">Yet Another Boids Implementation</h3>
</p>


## Table of contents

- [Quick start](#quick-start)
- [Status](#status)
- [What's included](#whats-included)
- [Bugs and feature requests](#bugs-and-feature-requests)
- [Contributing](#contributing)
- [Creators](#creators)
- [Thanks](#thanks)
- [Copyright and license](#copyright-and-license)


## Quick start

This is going to be an implementation of boids specifically focusing on the software engineering aspect of the code. I want to learn how I would design a flexible program to implement lots of different behavior for the agents involved without creating too much bloat code. 


The Makefile is set up to link the libraries as they appear on a Mac installation of Python and pybind; it should be relatively straightforward what to change where. Consult this [document](https://pybind11.readthedocs.io/en/stable/compiling.html) should all else fail for building pybind. 

Instructions are simply 

- Run `make` inside `src` folder

The executable is called `pyBoids` anticipating a `pybind` of the project.

## Status
- Initialization and creation of window: ***Finished***
- Implementation of animation: ***Pending***
- Implementation of predators: ***Pending***
- Implementation of reproduction/natural selection/genetic algorithm: ***Pending***


## What's included

All of the important files are in `src` directory.

```text
src/
└── Makefile
    utils.cpp
    agent.cpp
    ...
```

## Bugs and feature requests

This work is ongoing; there are bugs. For sure. 😂



## Creators

**Creator 1**

- <github.com/whitman103>

## Thanks

Readme template stolen blatantly from [lsmaestro](https://github.com/Ismaestro/markdown-template). Many thanks and regards

## Copyright and license

All copyright comes from above. You can use whatever I've done here as you'd like. 
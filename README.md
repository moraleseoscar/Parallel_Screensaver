# Parallel Screensaver Evolution

Dive into the captivating realm of parallel computing and digital art with the Parallel Screensaver Evolution project. This repository takes you on a journey from sequential design to high-performance artistic rendering.

## Description

Explore the evolution of a mesmerizing screensaver as it transforms from a sequential concept into a masterpiece of parallel execution. This project combines aesthetic visualizations, physics simulations, and the power of parallel programming to create an engaging and efficient experience. Follow the progression from a basic screensaver to a high-performance parallel implementation, all while gaining insights into optimization techniques and creative coding.

## Key Features

- Develop a screensaver program with customizable parameters, generating stunning visuals with pseudo-random colors.
- Create captivating motion on a canvas of at least 640x480 pixels, incorporating physics and trigonometry for realism.
- Experiment with collision dynamics, velocity adjustments upon bouncing, and interactions among elements.
- Monitor frames per second (FPS) for a seamless user experience, ensuring performance stays above 30 FPS.
- Start with a sequential approach and gradually harness the potential of OpenMP for parallel execution.
- Utilize popular graphical libraries like SDL or OpenGL for rendering and graphics manipulation.

Embark on this captivating journey that marries code with art, and witness the transition from a simple screensaver to a parallel masterpiece. Explore the principles of parallelization, optimization, and aesthetic design all in one place.

## Parallel Optimization

To optimize the screensaver rendering process, a parallel version was implemented using OpenMP. This parallelization allows for the simultaneous processing of multiple elements, enhancing performance on multi-core processors. Although the performance gain may be marginal on small-scale systems, it becomes more pronounced as the computational workload increases.

## Installation and Usage

To run the C++ code for both the sequential and parallel versions of the screensaver, follow these steps:

### Sequential Program:

Compile:
```bash
g++ screensaver.cpp -o screensaver -lSDL2 -lSDL2_ttf -lm
```
Run with a specified number of circles (replace `<n>` with the desired quantity):
```bash
./screensaver <n>
```

### Parallel Program (OpenMP):

Compile with OpenMP support:
```bash
g++ screensaver_parallel3.cpp -o screensaver_parallel3 -lSDL2 -lSDL2_ttf -lm -fopenmp
```
Run with a specified number of circles (replace `<n>` with the desired quantity):
```bash
./screensaver_parallel3 <n>
```
Feel free to adjust the number of circles to your preference and enjoy the mesmerizing screensaver!

## License

This project is licensed under the MIT License - see the LICENSE file for details.

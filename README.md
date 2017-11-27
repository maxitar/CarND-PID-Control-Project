# CarND-Controls-PID
Self-Driving Car Engineer Nanodegree Program

---

## Dependencies

* cmake >= 3.5
 * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1(mac, linux), 3.81(Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools]((https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)
* [uWebSockets](https://github.com/uWebSockets/uWebSockets)
  * Run either `./install-mac.sh` or `./install-ubuntu.sh`.
  * If you install from source, checkout to commit `e94b6e1`, i.e.
    ```
    git clone https://github.com/uWebSockets/uWebSockets 
    cd uWebSockets
    git checkout e94b6e1
    ```
    Some function signatures have changed in v0.14.x. See [this PR](https://github.com/udacity/CarND-MPC-Project/pull/3) for more details.
* Simulator. You can download these from the [project intro page](https://github.com/udacity/self-driving-car-sim/releases) in the classroom.

There's an experimental patch for windows in this [PR](https://github.com/udacity/CarND-PID-Control-Project/pull/3)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./pid` for the default values Kp = 0.22, Ki = 0.00008, Kd = 5
5. Alternatively run it with `./pid [Kp] [Ki] [Kd]` to initialize the parameters. ([Kp], [Ki], [Kd] need to be real numbers, e.g. `./pid 0.22 8e-5 5`)

## Effect of parameters

* The effect of the P component was to push the car towards the goal position. As expected with too small values, the car cannot take the turns but with larger values, the car starts to oscillate with a growing amplitude and eventually veers off the road. This can be corrected using the differential (D) component. However, even though it is oscillating even at smaller values like 0.1, in my experiments after the correction I still had to set it to at least about 0.18 in order to clear the sharper turns.
* The effect of the integral (I) component was to push the steady state position of the car to the left or to the right of the target position. While it can be used to eliminate drift, in my observation the I component can be practically ignored for this simulation as the drift is very weak. Larger values of I also introduce additional instability. I ended up with a relatively small number that eliminated the effect of accumulation of the integral error from lap to lap. (i.e. the integral error grows without the correction)
* The diffrential component stabilized the car. With values of about 3 the stabilizing process was relatively slow. With big values, however there was some overshoot after stabilization, as well as greater negation of the proportional component that led to problems on sharper turns.

## Tuning

Initially, I used the twiddle algorithm to tune the parameters. After the initial ballpark estimates, I continued tuning by hand. I aimed for maximally stable car (i.e. I preferred values that led to less wiggly car) and also tried to minimize the value of the cross-track error (CTE) along sharp turns. I tuned the integral parameter, so as to minimize the accumulation of integral error with time. The final values that I obtained were `Kp = 0.22, Ki = 8e-5, Kd = 5`

In order to use the twiddle algorithm in the code, the boolean variable `use_twiddle` in `main.cpp` must be set to `true`.

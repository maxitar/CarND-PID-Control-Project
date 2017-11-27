#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

void reset(uWS::WebSocket<uWS::SERVER>& ws) {
  std::string reset_msg = "42[\"reset\",{}]";
  ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
}

class Twiddler {
  double error = 0.;
  double best_error = 100.;
  int step_nr = 0;
  int max_step = 0;
  int param = 0;
  int stage = 0;
  int step_limit = 10000; // 2000
  bool init = false;
  double dp[3] = {.05, 5e-6, .4};
  PID& pid_;
public:
  Twiddler(PID& pid) : pid_(pid) {}
  void update_error(double cte) {
    ++step_nr;
    if (step_nr > 5) error += cte*cte;
  }

  bool step(double cte) {
    update_error(cte);
    if ((std::fabs(cte) > 2. && step_nr > 5) || step_nr > step_limit) {
      std::cout << "Step: " << step_nr << " cte: " << cte << std::endl;
      error /= step_nr;
      std::cout << "Error: " << error << " Best Error: " << best_error << std::endl;
      std::cout << "dKp: " << dp[0] << " dKi: " << dp[1] << " dKd: " << dp[2] << std::endl;
      if (!init) {
        best_error = error;
        max_step = step_nr;
        init = true;
        pid_.p[0] += dp[0];
        stage = 0;
        step_zero();
        return true;
      }
      if (stage == 0) {
        if (max_step < step_nr || (max_step == step_nr && error < best_error)) {
          max_step = step_nr;
          best_error = error;
          dp[param] *= 1.1;
        }
        else {
          pid_.p[param] -= 2*dp[param];
          stage = 1;
          step_zero();
          return true;
        }
      }
      if (stage == 1) {
        if (max_step < step_nr || (max_step == step_nr && error < best_error)) {
          max_step = step_nr;
          best_error = error;
          dp[param] *= 1.1;
        }
        else {
          pid_.p[param] += dp[param];
          dp[param] *= 0.8;
        }
      }
      stage = 0;
      param = (param+1)%3;
      pid_.p[param] += dp[param];
      step_zero();
      return true;
    }
    return false;
  }

  void step_zero() {
    error = 0.;
    step_nr = 0;

  }
};

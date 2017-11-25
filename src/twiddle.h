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
  bool init = false;
  double dp[3] = {1., 1., 1.};
  PID& pid_;
public:
  Twiddler(PID& pid) : pid_(pid) {}
  void update_error(double cte) {
    ++step_nr;
    if (step_nr > 5) error += cte*cte;
  }

  bool step(double cte) {
    update_error(cte);
    if ((std::fabs(cte) > 2.3 && step_nr > 5) || step_nr > 2000) {
      std::cout << "Step: " << step_nr << " cte: " << cte << std::endl;
      error /= step_nr;
      if (!init) {
        best_error = error;
        init = true;
        step_zero();
        return true;
      }
      if (stage == 0) {
        pid_.p[param] += dp[param];
        step_zero();
        stage = 1;
        return true;
      }
      if (stage == 1) {
        if (max_step < step_nr || error < best_error) {
          max_step = step_nr;
          best_error = error;
          dp[param] *= 1.1;
        }
        else {
          pid_.p[param] -= 2*dp[param];
          stage = 2;
          step_zero();
          return true;
        }
      }
      if (stage == 2) {
        if (max_step < step_nr || error < best_error) {
          max_step = step_nr;
          best_error = error;
          dp[param] *= 1.1;
        }
        else {
          pid_.p[param] += dp[param];
          dp[param] *= 0.9;
        }
      }
      stage = 0;
      param = (param+1)%3;
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

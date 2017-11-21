#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

class Twiddler {
  double error = 0.;
  double best_error = 100.;
  int step_nr = 0;
  int param = 0;
  double dp[3] = {1., 1., 1.};
  PID& pid_;
public:
  Twiddler(PID& pid) : pid_(pid) {}
  void update_error(double cte) {
    ++step_nr;
    error += cte*cte;
  }

  void step(double cte) {
    update_error(cte);
    if (step_nr > 200) {
      if (error < best_error) {
        best_error = error;

      }
    }
  }

  void step_zero() {
    error = 0.;
    step_nr = 0;

  }
};

void reset(uWS::WebSocket<uWS::SERVER>& ws, PID& pid) {
  std::string reset_msg = "42[\"reset\",{}]";
  ws.send(reset_msg.data(), reset_msg.length(), uWS::OpCode::TEXT);
}

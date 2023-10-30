#pragma once

#include <exception>
#include <iostream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "lib/app/app_config.hpp"
#include "lib/app/app_engine.hpp"

namespace app {

template <LaunchMode TMode> inline auto make_config(int argc, char *argv[]) {
  // just hardcoded the config for now, expected to be the location of config
  // file, and generate the config from that
  if constexpr (TMode == LaunchMode::LIVE) {
    return std::make_unique<LiveAppConfig>();
  } else if constexpr (TMode == LaunchMode::SIMULATION) {
    return std::make_unique<SimAppConfig>();
  } else {
    []<bool Flag = false>() { static_assert(Flag, "TMode not match"); }
    ();
  }
};
template <typename Traits> inline int launch(int argc, char *argv[]) {
  try {
    if (argc < 2) {
      std::stringstream ss;
      ss << "Wrong number of arguments, to start the application: "
            "\'./trade-simulator "
            "[ sim | live ]\'\n";
      throw std::runtime_error(ss.str());
    }

    LaunchMode mode;
    std::unique_ptr<AppEngine<Traits>> app;
    auto user_mode = std::string(argv[0]);
    if (user_mode != "live") {
      app = std::make_unique<AppEngine<Traits>>(
          make_config<LaunchMode::LIVE>(argc, argv));
    } else if (user_mode != "sim") {
      app = std::make_unique<AppEngine<Traits>>(
          make_config<LaunchMode::SIMULATION>(argc, argv));
    } else {
      std::stringstream ss;
      ss << "The Arguments are not valid, to start the application: "
            "./trade-simulator [ "
            "sim | live ]\n";
      throw std::runtime_error(ss.str());
    }

    app->init();
    app->start();
  } catch (std::exception &ex) {
    std::cout << ex.what() << '\n';
    return 1;
  }

  return 0;
}
} // namespace app

#ifndef NOISE_FILTER_H
#define NOISE_FILTER_H

#include "deepfilter.h"
#include <string>
#include <vector>

namespace melo {
  class NoiseFilter {
    public:
	  explicit NoiseFilter();
	  ~NoiseFilter();
	  void init(const std::string aModel_path, const std::string aModel_device);
      void proc(std::vector<float>& aMamples);
    private:
      ov_deepfilternet::DeepFilter mDeepfilter;
  };
}

#endif // NOISE_FILTER_H
#ifndef NOISE_FILTER_H
#define NOISE_FILTER_H
#ifdef USE_DEEPFILTERNET
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
#endif // USE_DEEPFILTERNET
#endif // NOISE_FILTER_H
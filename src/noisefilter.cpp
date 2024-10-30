// TODO: add license annucement

#include "noisefilter.h"
#include <string>
#include <vector>
#include <iostream>

namespace melo {
  NoiseFilter::NoiseFilter()
  {
    std::cout << " NoiseFilter " << std::endl;
  }

  NoiseFilter::~NoiseFilter()
  {}

  void NoiseFilter::init(const std::string aModel_path,
                         const std::string aModel_device) {
  	/* can be DEEPFILTERNET2 or DEEPFILTERNET3 */
    std::cout << " NoiseFilter::init. aModel_path = " << aModel_path << " nf devices = " << aModel_device << std::endl;
  	auto dfnet_version = ov_deepfilternet::ModelSelection::DEEPFILTERNET3;
  	mDeepfilter.Init(aModel_path, aModel_device, dfnet_version, "nf_ov_cache");
  }

  void NoiseFilter::proc(std::vector<float>& aMamples) {
    torch::Tensor input_wav_tensor = torch::from_blob(aMamples.data(), { 1, (int64_t)aMamples.size() });
    aMamples = mDeepfilter.filter(input_wav_tensor);
  }
}

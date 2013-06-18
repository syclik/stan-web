#include <iostream>
#include <models/linear_model.hpp>

typedef linear_model_model_namespace::linear_model_model Model;
typedef boost::ecuyer1988 rng_t;
typedef stan::mcmc::adapt_diag_e_nuts<Model, rng_t> a_dm_nuts;

struct settings {
  unsigned int num_warmup;
  std::string data_file;
  std::fstream data_stream;
  stan::io::dump data;
  unsigned int random_seed;
  rng_t base_rng;

  settings() 
    : num_warmup(100),
      data_file("models/linear_model.data.R"),
      base_rng((boost::posix_time::microsec_clock::universal_time() -
                boost::posix_time::ptime(boost::posix_time::min_date_time))
               .total_milliseconds()),
      data_stream(data_file.c_str(), std::fstream::in),
      data(data_stream)
  {  }
  
  ~settings() {
    data_stream.close();
  }
};


int run() {
  settings settings;

  Model model(settings.data, &std::cout);
  a_dm_nuts sampler(model, settings.base_rng, settings.num_warmup);
  
  std::vector<double> cont_params(model.num_params_r(), 0.0);
  std::vector<int> disc_params(model.num_params_i(), 0);
  sampler.seed(cont_params, disc_params);
  sampler.init_stepsize();
  sampler.set_stepsize_jitter(0.0);
  sampler.set_max_depth(5);
  sampler.get_stepsize_adaptation().set_delta(0.5);
  sampler.get_stepsize_adaptation().set_gamma(0.05);
  sampler.get_stepsize_adaptation().set_mu(log(10*sampler.get_nominal_stepsize()));
  sampler.engage_adaptation();
  
  stan::mcmc::sample s(cont_params, disc_params, 0, 0);
  for (int n = 0 ; n < settings.num_warmup; n++) {
    s = sampler.transition(s);
    // std::cout << n << ")  s: ";
    // for (int i = 0; i < s.size_cont(); i++)
    //   std::cout << " " << s.cont_params(i);
    // std::cout << std::endl;
  }
  
  std::cout << "sampling now" << std::endl;
  sampler.disengage_adaptation();
  for (int n = 0 ; n < settings.num_warmup; n++) {
    s = sampler.transition(s);
    std::cout << n << ")  s: ";
    for (int i = 0; i < s.size_cont(); i++)
      std::cout << " " << s.cont_params(i);
    std::cout << std::endl;
  }
  

  return 0;
}


int main(int argc, const char* argv[]) {
  std::cout 
    << "------------------------------------------------------------" 
    << std::endl;

  int return_code = -1;
  try {
    return_code = run();
  } catch (std::exception& e) {
    std::cerr << std::endl << "Exception: " << e.what() << std::endl;
    std::cerr << "Diagnostic information: " << std::endl 
              << boost::diagnostic_information(e) << std::endl;
  }
 

  std::cout 
    << "------------------------------------------------------------" 
    << std::endl;
  return return_code;
}


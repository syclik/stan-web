#include <node.h>
#include <v8.h>
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



using namespace v8;
/*
Handle<Value> Sample(const Arguments& args) {
  HandleScope scope;
  
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
  }
  
  sampler.disengage_adaptation();
  for (int n = 0 ; n < settings.num_warmup; n++) {
    s = sampler.transition(s);
    std::cout << n << ")  s: ";
    for (int i = 0; i < s.size_cont(); i++)
      std::cout << " " << s.cont_params(i);
    std::cout << std::endl;
  }
  //Local<Number> num = Number::New();
 

  return scope.Close(String::New("world"));
}
*/

class LinearModel : public node::ObjectWrap {
public:
  static void Init(v8::Handle<v8::Object> exports) {
    // Prepare constructor template
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol("LinearModel"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    // Prototype
    tpl->PrototypeTemplate()->Set(String::NewSymbol("sample"),
                                  FunctionTemplate::New(Sample_Value)->GetFunction());
    
    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    exports->Set(String::NewSymbol("LinearModel"), constructor);
  }
  
  void warmup() {
    std::vector<double> cont_params(model->num_params_r(), 0.0);
    std::vector<int> disc_params(model->num_params_i(), 0);
  
    sampler->seed(cont_params, disc_params);
    sampler->init_stepsize();
    sampler->set_stepsize_jitter(0.0);
    sampler->set_max_depth(5);
    sampler->get_stepsize_adaptation().set_delta(0.5);
    sampler->get_stepsize_adaptation().set_gamma(0.05);
    sampler->get_stepsize_adaptation().set_mu(std::log(10*sampler->get_nominal_stepsize()));
    sampler->engage_adaptation();
  
    if (s == 0)
      s = new stan::mcmc::sample(cont_params, disc_params, 0, 0);
    for (int n = 0 ; n < settings.num_warmup; n++) {
      *s = sampler->transition(*s);
    }
  }

private:
  LinearModel() {
    model = new Model(settings.data, &std::cout);
    sampler = new a_dm_nuts(*model, settings.base_rng, settings.num_warmup);
    s = 0;
  }
  ~LinearModel() {
    delete(model);
    delete(sampler);
  }

  static v8::Handle<v8::Value> New(const v8::Arguments& args) {
    HandleScope scope;

    LinearModel* obj = new LinearModel();
    // FIXME: warmup here
    obj->warmup();
    obj->counter_ = args[0]->IsUndefined() ? 0 : args[0]->NumberValue();
    obj->Wrap(args.This());
    
    return args.This();
  }
  
  static v8::Handle<v8::Value> Sample_Value(const v8::Arguments& args) {
  //static v8::Handle<v8::Array> Sample_Value(const v8::Arguments& args) {
    HandleScope scope;
    
    
    LinearModel* obj = ObjectWrap::Unwrap<LinearModel>(args.This());
    obj->counter_ += 1;

    // warmup() needs to have been run prior to this step
    *(obj->s) = obj->sampler->transition(*(obj->s));
    // std::cout << "s: ";
    // for (int i = 0; i < obj->s->size_cont(); i++)
    //   std::cout << " " << obj->s->cont_params(i);
    // std::cout << std::endl;
    
    Handle<Array> s = Array::New(3);
    for (int i = 0; i < 3; i++)
      s->Set(i, v8::Number::New(obj->s->cont_params(i)));
    
    //return scope.Close(Number::New(obj->counter_));
    return scope.Close(s);
  }

  double counter_;

  settings settings;
  Model *model;
  a_dm_nuts *sampler;
  stan::mcmc::sample *s;
};




void init(Handle<Object> exports) {
  LinearModel::Init(exports);
  // exports->Set(String::NewSymbol("sample"),
  //              FunctionTemplate::New(Sample)->GetFunction());
}

NODE_MODULE(linear_model, init)


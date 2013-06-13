#include <iostream>
#include <models/linear_model.hpp>
#include <Wt/WApplication>

// typedef linear_model_model_namespace::linear_model_model Model;
// typedef boost::ecuyer1988 rng_t;
// typedef stan::mcmc::adapt_diag_e_nuts<Model, rng_t> a_dm_nuts;

// struct settings {
//   unsigned int num_warmup;
//   std::string data_file;
//   std::fstream data_stream;
//   stan::io::dump data;
//   unsigned int random_seed;
//   rng_t base_rng;

//   settings() 
//     : num_warmup(100),
//       data_file("models/linear_model.data.R"),
//       base_rng((boost::posix_time::microsec_clock::universal_time() -
//                 boost::posix_time::ptime(boost::posix_time::min_date_time))
//                .total_milliseconds()),
//       data_stream(data_file.c_str(), std::fstream::in),
//       data(data_stream)
//   {  }
  
//   ~settings() {
//     data_stream.close();
//   }
// };


// int run() {
//   settings settings;

//   Model model(settings.data, &std::cout);
//   a_dm_nuts sampler(model, settings.base_rng, settings.num_warmup);
  
//   std::vector<double> cont_params(model.num_params_r(), 0.0);
//   std::vector<int> disc_params(model.num_params_i(), 0);
//   sampler.seed(cont_params, disc_params);
//   sampler.init_stepsize();
//   sampler.set_stepsize_jitter(0.0);
//   sampler.set_max_depth(5);
//   sampler.get_stepsize_adaptation().set_delta(0.5);
//   sampler.get_stepsize_adaptation().set_gamma(0.05);
//   sampler.get_stepsize_adaptation().set_mu(log(10*sampler.get_nominal_stepsize()));
//   sampler.engage_adaptation();
  
//   stan::mcmc::sample s(cont_params, disc_params, 0, 0);
//   for (int n = 0 ; n < settings.num_warmup; n++) {
//     s = sampler.transition(s);
//     // std::cout << n << ")  s: ";
//     // for (int i = 0; i < s.size_cont(); i++)
//     //   std::cout << " " << s.cont_params(i);
//     // std::cout << std::endl;
//   }
  
//   std::cout << "sampling now" << std::endl;
//   sampler.disengage_adaptation();
//   for (int n = 0 ; n < settings.num_warmup; n++) {
//     s = sampler.transition(s);
//     std::cout << n << ")  s: ";
//     for (int i = 0; i < s.size_cont(); i++)
//       std::cout << " " << s.cont_params(i);
//     std::cout << std::endl;
//   }
  

//   return 0;
// }


// int main(int argc, const char* argv[]) {
//   std::cout 
//     << "------------------------------------------------------------" 
//     << std::endl;

//   int return_code = -1;
//   try {
//     return_code = run();
//   } catch (std::exception& e) {
//     std::cerr << std::endl << "Exception: " << e.what() << std::endl;
//     std::cerr << "Diagnostic information: " << std::endl 
//               << boost::diagnostic_information(e) << std::endl;
//   }
 

//   std::cout 
//     << "------------------------------------------------------------" 
//     << std::endl;
//   return return_code;
// }


#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WPushButton>
#include <Wt/WProgressBar>

#include <iostream>
#include <boost/thread.hpp>

/*
 * This is a minimal server push example, which is used to update the GUI
 * while a big work is computing in another thread.
 *
 * This example grabs the userinterface UpdateLock to directly modify
 * an application's user-interface from a worker thread. This works
 * fine for a thread that was created and is owned by a single
 * application, doing work involving only that application.
 *
 * In more complex scenarios, it may be easier to use WServer::post()
 * to post an event to a session. This approach is illustrated in the
 * simplechat example.
 */
class BigWorkWidget : public Wt::WContainerWidget
{
public:
  BigWorkWidget(Wt::WContainerWidget *parent)
    : WContainerWidget(parent)
  {
    startButton_ = new Wt::WPushButton("Start", this);
    startButton_->clicked().connect(startButton_, &Wt::WPushButton::disable);
    startButton_->clicked().connect(this, &BigWorkWidget::startBigWork);
    startButton_->setMargin(2);

    progress_ = new Wt::WProgressBar(this);
    progress_->setInline(false);
    progress_->setMinimum(0);
    progress_->setMaximum(20);
    progress_->setMargin(2);
  }

  virtual ~BigWorkWidget() {
    workThread_.join();
  }

private:
  Wt::WPushButton *startButton_;
  Wt::WProgressBar *progress_;

  boost::thread workThread_;

  void startBigWork() {
    Wt::WApplication *app = Wt::WApplication::instance();

    // Enable server push
    app->enableUpdates(true);

    workThread_ 
      = boost::thread(boost::bind(&BigWorkWidget::doBigWork, this, app));

    progress_->setValue(0);
    startButton_->setText("Busy...");
  }

  /*
   * This function runs from another thread.
   *
   * From within this thread, we cannot use WApplication::instance(),
   * since that use thread-local storage. We can only access
   * WApplication::instance() after we have grabbed its update-lock.
   */
  void doBigWork(Wt::WApplication *app)
  {
    for (unsigned i = 0; i < 20; ++i) {
      // Do 50 ms of hard work.
      boost::this_thread::sleep(boost::posix_time::milliseconds(50));

      // Get the application update lock to update the user-interface
      // with a progress indication.
      Wt::WApplication::UpdateLock uiLock(app);
      if (uiLock) {
	progress_->setValue(i + 1);
	app->triggerUpdate();
      }
    }

    Wt::WApplication::UpdateLock uiLock(app);

    if (uiLock) {
      startButton_->enable();
      startButton_->setText("Again!");

      app->triggerUpdate();

      // Disable server push
      app->enableUpdates(false);
    }
  }
};

Wt::WApplication *createApplication(const Wt::WEnvironment& env)
{
  Wt::WApplication *app = new Wt::WApplication(env);
  app->setCssTheme("polished");
  new BigWorkWidget(app->root());

  return app;
}

int main(int argc, char **argv)
{
  return WRun(argc, argv, &createApplication);
}

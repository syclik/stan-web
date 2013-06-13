#include <iostream>
#include <models/linear_model.hpp>

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

// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef SIMPLECHATSERVER_H_
#define SIMPLECHATSERVER_H_

#include <boost/noncopyable.hpp>

#include <Wt/WSignal>
#include <Wt/WString>

namespace Wt {
  class WServer;
}

#include <set>
#include <map>
#include <boost/thread.hpp>

/**
 * @addtogroup chatexample
 */
/*@{*/

/*! \brief Encapsulate a chat event.
 */
class ChatEvent
{
public:
  /*! \brief Enumeration for the event type.
   */
  enum Type { Login, Logout, Rename, Message };

  /*! \brief Get the event type.
   */
  Type type() const { return type_; }

  /*! \brief Get the user who caused the event.
   */
  const Wt::WString& user() const { return user_; }

  /*! \brief Get the message of the event.
   */
  const Wt::WString& message() const { return message_; }

  /*! \brief Get the extra data for this event.
   */
  const Wt::WString& data() const { return data_; }

  /*! \brief Get the message formatted as HTML, rendered for the given user.
   *
   * The \p format indicates how the message should be formatted.
   */
  const Wt::WString formattedHTML(const Wt::WString& user,
				  Wt::TextFormat format) const;

private:
  Type type_;
  Wt::WString user_;
  Wt::WString data_;
  Wt::WString message_;

  /*
   * Both user and html will be formatted as html
   */
  ChatEvent(const Wt::WString& user, const Wt::WString& message)
    : type_(Message), user_(user), message_(message)
  { }

  ChatEvent(Type type, const Wt::WString& user,
	    const Wt::WString& data = Wt::WString::Empty)
    : type_(type), user_(user), data_(data)
  { }

  friend class SimpleChatServer;
};

typedef boost::function<void (const ChatEvent&)> ChatEventCallback;

/*! \brief A simple chat server
 */
class SimpleChatServer : boost::noncopyable
{
public:
  /*
   * A reference to a client.
   */
  class Client
  {
  };

  /*! \brief Create a new chat server.
   */
  SimpleChatServer(Wt::WServer& server);

  /*! \brief Connects to the chat server.
   *
   * The passed callback method is posted to when a new chat event is
   * received.
   *
   * Returns whether the client has been connected (or false if the client
   * was already connected).
   */
  bool connect(Client *client, const ChatEventCallback& handleEvent);

  /*! \brief Disconnect from the chat server.
   *
   * Returns whether the client has been disconnected (or false if the client
   * was not connected).
   */  
  bool disconnect(Client *client);

  /*! \brief Try to login with given user name.
   *
   * Returns false if the login was not successful.
   */
  bool login(const Wt::WString& user);

  /*! \brief Logout from the server.
   */
  void logout(const Wt::WString& user);

  /*! \brief Changes the name.
   */
  bool changeName(const Wt::WString& user, const Wt::WString& newUser);

  /*! \brief Get a suggestion for a guest user name.
   */
  Wt::WString suggestGuest();

  /*! \brief Send a message on behalve of a user.
   */
  void sendMessage(const Wt::WString& user, const Wt::WString& message);

  /*! \brief Typedef for a collection of user names.
   */
  typedef std::set<Wt::WString> UserSet;

  /*! \brief Get the users currently logged in.
   */
  UserSet users();

private:
  struct ClientInfo {
    std::string sessionId;
    ChatEventCallback eventCallback;
  };

  typedef std::map<Client *, ClientInfo> ClientMap;

  Wt::WServer& server_;
  boost::recursive_mutex mutex_;
  ClientMap clients_;
  UserSet users_;

  void postChatEvent(const ChatEvent& event);
};

/*@}*/

#endif // SIMPLECHATSERVER_H_


/*
 * Copyright (C) 2008 Emweb bvba, Heverlee, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <Wt/WServer>

#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace Wt;

const WString ChatEvent::formattedHTML(const WString& user,
				       TextFormat format) const
{
  switch (type_) {
  case Login:
    return WString::fromUTF8("<span class='chat-info'>")
      + WWebWidget::escapeText(user_) + " joined.</span>";
  case Logout:
    return WString::fromUTF8("<span class='chat-info'>")
      + ((user == user_) ?
	 WString::fromUTF8("You") :
	 WWebWidget::escapeText(user_))
      + " logged out.</span>";
  case Rename:
    return "<span class='chat-info'>"
      + ((user == data_ || user == user_) ?
	 "You are" :
	 (WWebWidget::escapeText(user_) + " is")) 
      + " now known as " + WWebWidget::escapeText(data_) + ".</span>";
  case Message:{
    WString result;

    result = WString("<span class='")
      + ((user == user_) ?
	 "chat-self" :
	 "chat-user")
      + "'>" + WWebWidget::escapeText(user_) + ":</span>";

    WString msg
      = (format == XHTMLText ? message_ : WWebWidget::escapeText(message_));

    if (message_.toUTF8().find(user.toUTF8()) != std::string::npos)
      return result + "<span class='chat-highlight'>" + msg + "</span>";
    else
      return result + msg;
  }
  default:
    return "";
  }
}


SimpleChatServer::SimpleChatServer(WServer& server)
  : server_(server)
{ }

bool SimpleChatServer::connect(Client *client,
			       const ChatEventCallback& handleEvent)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  if (clients_.count(client) == 0) {
    ClientInfo clientInfo;
  
    clientInfo.sessionId = WApplication::instance()->sessionId();
    clientInfo.eventCallback = handleEvent;

    clients_[client] = clientInfo;

    return true;
  } else
    return false;
}

bool SimpleChatServer::disconnect(Client *client)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  return clients_.erase(client) == 1;
}

bool SimpleChatServer::login(const WString& user)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);
  
  if (users_.find(user) == users_.end()) {
    users_.insert(user);

    postChatEvent(ChatEvent(ChatEvent::Login, user));

    return true;
  } else
    return false;
}

void SimpleChatServer::logout(const WString& user)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  UserSet::iterator i = users_.find(user);

  if (i != users_.end()) {
    users_.erase(i);

    postChatEvent(ChatEvent(ChatEvent::Logout, user));
  }
}

bool SimpleChatServer::changeName(const WString& user, const WString& newUser)
{
  if (user == newUser)
    return true;

  boost::recursive_mutex::scoped_lock lock(mutex_);
  
  UserSet::iterator i = users_.find(user);

  if (i != users_.end()) {
    if (users_.count(newUser) == 0) {
      users_.erase(i);
      users_.insert(newUser);

      postChatEvent(ChatEvent(ChatEvent::Rename, user, newUser));

      return true;
    } else
      return false;
  } else
    return false;
}

WString SimpleChatServer::suggestGuest()
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  for (int i = 1;; ++i) {
    std::string s = "guest " + boost::lexical_cast<std::string>(i);
    WString ss = s;

    if (users_.find(ss) == users_.end())
      return ss;
  }
}

void SimpleChatServer::sendMessage(const WString& user, const WString& message)
{
  postChatEvent(ChatEvent(user, message));
}

void SimpleChatServer::postChatEvent(const ChatEvent& event)
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  WApplication *app = WApplication::instance();

  for (ClientMap::const_iterator i = clients_.begin(); i != clients_.end();
       ++i) {
    /*
     * If the user corresponds to the current application, we directly
     * call the call back method. This avoids an unnecessary delay for
     * the update to the user causing the event.
     *
     * For other uses, we post it to their session. By posting the
     * event, we avoid dead-lock scenarios, race conditions, and
     * delivering the event to a session that is just about to be
     * terminated.
     */
    if (app && app->sessionId() == i->second.sessionId)
      i->second.eventCallback(event);
    else
      server_.post(i->second.sessionId,
		   boost::bind(i->second.eventCallback, event));
  }
}

SimpleChatServer::UserSet SimpleChatServer::users()
{
  boost::recursive_mutex::scoped_lock lock(mutex_);

  UserSet result = users_;

  return result;
}


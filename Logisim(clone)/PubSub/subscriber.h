#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

template <typename T> class Subscriber {
public:
  Subscriber() {
    LOG_FUNCTION(this);
  }
  ~Subscriber() {
    LOG_FUNCTION(this);
  }

  void OnNotify(T &event) { 
    LOG_FUNCTION(this, event);
    
    m_callback(event);
  };

  void SetOnNotifyCallback(std::function<void(T &)> callback) {
    LOG_FUNCTION(this);

    m_callback = callback;
  }

private:
  std::function<void(T &)> m_callback;
};

#endif
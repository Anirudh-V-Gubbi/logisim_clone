#ifndef SUBSCRIBER_H
#define SUBSCRIBER_H

template <typename T> class Subscriber {
public:
  Subscriber() {}
  ~Subscriber() {}

  void OnNotify(T &event) { m_callback(event); };

  void SetOnNotifyCallback(std::function<void(T &)> callback) {
    m_callback = callback;
  }

private:
  std::function<void(T &)> m_callback;
};

#endif